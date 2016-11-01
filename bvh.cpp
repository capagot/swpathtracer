#include "bvh.h"

BVH::BVH( const std::vector< Primitive::PrimitiveUniquePtr > &primitives ) :
        primitives_( primitives )
{
    if ( primitives_.size() > 0 )
    {
        std::deque< PrimitiveAABBArea > s( primitives_.size() );
        s_.resize( primitives_.size() );

        AABB root_aabb;

        for( std::size_t i = 0; i < primitives_.size(); i++ )
        {
            AABB aabb = primitives_[i]->getAABB();

            // compute the AABB area for the root node
            if ( !i )
                root_aabb = aabb;
            else
                root_aabb = root_aabb + aabb;

            s[i].primitive_id_ = i;
            s[i].centroid_ = aabb.centroid_;
            s[i].aabb_ = aabb;
        }

        splitNode( &root_node_, s, 0, s.size() - 1, root_aabb.getArea() );
    }
}

bool BVH::intersect( const Ray &ray,
                     IntersectionRecord &intersection_record,
                     long unsigned int &num_intersection_tests_,
                     long unsigned int &num_intersections_ ) const
{
    return traverse( root_node_, ray, intersection_record, "-" );
}

BVH::~BVH( void )
{}

double BVH::area( const std::deque< PrimitiveAABBArea > &s ) const
{
    if ( s.size() == 0 )
        return std::numeric_limits< double >::infinity();
    else
    {
        AABB acc_aabb = s[0].aabb_;

        for( std::size_t i = 1; i < s.size(); i++ )
            acc_aabb = acc_aabb + s[i].aabb_;

        return acc_aabb.getArea();
    }
}

double BVH::SAH( std::size_t s1_size,
                 double s1_area,
                 std::size_t s2_size,
                 double s2_area,
                 double s_area )
{
    return 2.0 * cost_intersec_aabb_ + ( ( s1_area / s_area ) * s1_size * cost_intersec_tri_ ) +
                                       ( ( s2_area / s_area ) * s2_size * cost_intersec_tri_ );
}

void BVH::splitNode( BVHNode **node,
                     std::deque< PrimitiveAABBArea > &s,
                     std::size_t first,
                     std::size_t last,
                     double s_area )
{
    /* BVH construction based on the algorithm presented in the paper:
     *
     *     "Ray Tracing Deformable Scenes Using Dynamic Bounding Volume Hierarchies".
     *     Ingo Wald, Solomon Boulos, and Peter Shirley
     *     ACM Transactions on Graphics.
     *     Volume 26 Issue 1, 2007.
     */

    (*node) = new BVHNode();
    (*node)->first_ = first;
    (*node)->last_ = last;
    (*node)->left_ = nullptr;
    (*node)->right_ = nullptr;

    std::deque< PrimitiveAABBArea > s1;
    std::deque< PrimitiveAABBArea > s2;

    double best_cost = cost_intersec_tri_ * ( last - first + 1 );
    int best_axis = -1;
    int best_event = -1;

    for ( int axis = 1; axis <= 3; axis++ )
    {
        switch( axis )
        {
        case 1:
            std::sort( s.begin() + first, s.begin() + last + 1, Comparator::sortInX );
            break;
        case 2:
            std::sort( s.begin() + first, s.begin() + last + 1, Comparator::sortInY );
            break;
        case 3:
            std::sort( s.begin() + first, s.begin() + last + 1, Comparator::sortInZ );
            break;
        }

        // Sweep from left
        s1.clear();
        s2 = std::deque< PrimitiveAABBArea >( s.begin() + first, s.begin() + last + 1 );

        for ( std::size_t i = first; i <= last; i++ )
        {
            s[i].left_area_ = area( s1 );
            s1.push_back( s2.front() );
            s2.pop_front();
        }

        // Sweep from right
        s1 = std::deque< PrimitiveAABBArea >( s.begin() + first, s.begin() + last + 1 );
        s2.clear();

        for ( long int i = last; i >= static_cast< long int >( first ); i-- )
        {
            s[i].right_area_ = area( s2 );

            double this_cost = SAH( s1.size(),
                                    s[ ( i + 1 ) % ( s.size() + 1 ) ].left_area_, // Fixes an indexing problem from the original paper.
                                    s2.size(),
                                    s[i].right_area_,
                                    s_area );

            s2.push_front( s1.back() );
            s1.pop_back();

            if ( this_cost < best_cost )
            {
                best_cost = this_cost;
                best_event = i;
                best_axis = axis;
            }
        }
    }

    if ( best_axis == -1 ) // This is a leaf node
    {
        for ( long unsigned int i = first; i <= last; i++ )
        {
            s_[i].primitive_id_ = s[i].primitive_id_;

            if ( i == first )
                (*node)->aabb_ = s[i].aabb_;
            else
            {
                // build the AABB of the leaf node
                (*node)->aabb_.min_ = glm::min( (*node)->aabb_.min_, s[i].aabb_.min_ );
                (*node)->aabb_.max_ = glm::max( (*node)->aabb_.max_, s[i].aabb_.max_ );
            }
        }
    }
    else // This is an inner node
    {
        // Make inner node with best_axis / best_event
        switch( best_axis )
        {
        case 1:
            std::sort( s.begin() + first, s.begin() + last + 1, Comparator::sortInX );
            break;
        case 2:
            std::sort( s.begin() + first, s.begin() + last + 1, Comparator::sortInY );
            break;
        case 3:
            std::sort( s.begin() + first, s.begin() + last + 1, Comparator::sortInZ );
            break;
        }

        splitNode( &(*node)->left_, s, first, best_event, s_area );
        splitNode( &(*node)->right_, s, best_event + 1, last, s_area );

        // build the AABB of the inner node
        (*node)->aabb_.min_ = glm::min( (*node)->left_->aabb_.min_, (*node)->right_->aabb_.min_ );
        (*node)->aabb_.max_ = glm::max( (*node)->left_->aabb_.max_, (*node)->right_->aabb_.max_ );
    }
}

bool BVH::traverse( const BVHNode *node,
                    const Ray &ray,
                    IntersectionRecord &intersection_record,
                    std::string path_str ) const
{
    bool primitive_intersect = false;

    // DEBUG
    //std::cerr << ">>> " << path_str << "\n";

    if ( ( node ) && ( node->aabb_.intersect( ray ) ) )
    {
        if ( ( !node->left_ ) && ( !node->right_ ) ) // is a leaf node
        {
            IntersectionRecord tmp_intersection_record;

            for ( std::size_t primitive_id = node->first_; primitive_id <= node->last_; primitive_id++ )
            {
                if ( primitives_[s_[primitive_id].primitive_id_]->intersect( ray, tmp_intersection_record ) )
                {
                    if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
                    {
                        intersection_record = tmp_intersection_record;
                        primitive_intersect = true;
                    }
                }
            }
        }
        else
        {
            if ( traverse( node->left_, ray, intersection_record, path_str + "L" ) )
                primitive_intersect = true;

            if ( traverse( node->right_, ray, intersection_record, path_str + "R" ) )
                primitive_intersect = true;
        }
    }

    return primitive_intersect;

    /*
    bool intersection_result = false;

    if ( ( !node->left_ ) && ( !node->right_ ) ) // is a leaf node
    {
        IntersectionRecord tmp_intersection_record;

        for ( std::size_t primitive_id = node->first_; primitive_id <= node->last_; primitive_id++ )
        {
            if ( primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
            {
                if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
                {
                    intersection_record = tmp_intersection_record;
                    intersection_result = true;
                }
            }
        }
    }
    else
    {
        assert( node->left_ );
        assert( node->right_ );

        if ( node->left_->aabb_.intersect( ray ) )
            if ( traverse( node->left_, ray, intersection_record ) )
                intersection_result = true;

        if ( node->right_->aabb_.intersect( ray ) )
            if ( traverse( node->right_, ray, intersection_record ) )
                intersection_result = true;
    }

    return intersection_result;
    //*/
}

void BVH::dump( void ) const
{
    std::queue< BVHNode* > queue;
    BVHNode *node = nullptr;
    int depth = 0;

    if ( root_node_ )
    {
        queue.push( root_node_ );
        queue.push( nullptr );

        while( !queue.empty() )
        {
            node = queue.front();
            queue.pop();

            if ( node )
            {
                if ( ( !node->left_ ) && ( !node->right_ ) )
                    std::cerr << "l";
                else
                    std::cerr << "i";

                std::cerr << " " << node->aabb_.min_.x << " " << node->aabb_.min_.y << " " << node->aabb_.min_.z;
                std::cerr << " " << node->aabb_.max_.x << " " << node->aabb_.max_.y << " " << node->aabb_.max_.z;
                std::cerr << "\n";

                if ( node->left_ )
                    queue.push( node->left_ );
                if ( node->right_ )
                    queue.push( node->right_ );
            }
            else
                if ( !queue.empty() )
                {
                    queue.push( nullptr );
                    std::cerr << "\n";
                    depth++;
                }
        }
    }
}

void BVH::dumpPrimitives( void ) const
{
    std::queue< BVHNode* > queue;
    BVHNode *node = nullptr;
    int depth = 0;

    if ( root_node_ )
    {
        queue.push( root_node_ );
        queue.push( nullptr );

        while( !queue.empty() )
        {
            node = queue.front();
            queue.pop();

            if ( node )
            {
                for ( long unsigned int i = node->first_; i <= node->last_; i++  )
                {
                    primitives_[i]->printData();
                }

                std::cerr << "\n";

                if ( node->left_ )
                    queue.push( node->left_ );
                if ( node->right_ )
                    queue.push( node->right_ );
            }
            else
                if ( !queue.empty() )
                {
                    queue.push( nullptr );
                    std::cerr << "\n";
                    depth++;
                }
        }
    }
}
