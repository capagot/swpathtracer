#include "sbvh.h"

SBVH::SBVH(const std::vector<Primitive::PrimitiveUniquePtr> &primitives) : primitives_(primitives) {
    Timer t;
    t.start();

    if (primitives_.size() > 0) {
        root_node_ = SBVHNode::UniquePtr(new SBVHNode());
        root_node_->primitive_ref_list_ = SBVHNode::PrimitiveRefListUniquePtr(new std::vector<PrimitiveRef>());

        for (long int i = 0; i < primitives_.size(); i++) {
            root_node_->primitive_ref_list_->emplace_back(i, primitives_[i]->getAABB());

            if (!i)
                root_node_->aabb_ = (*root_node_->primitive_ref_list_)[i].aabb_;
            else
                root_node_->aabb_ = root_node_->aabb_ + (*root_node_->primitive_ref_list_)[i].aabb_;
        }

/*
        std::cout << "root AABB: " << root_node_->aabb_.min_[0] << ", "
                  << root_node_->aabb_.min_[1] << ", "
                  << root_node_->aabb_.min_[2] << " --- "
                  << root_node_->aabb_.max_[0] << ", "
                  << root_node_->aabb_.max_[1] << ", "
                  << root_node_->aabb_.max_[2] << "\n";
        std::cout << "root num primitives: " << root_node_->primitive_ref_list_->size() << "\n";
*/

        splitNode(root_node_, root_node_->aabb_.getArea());
    }

    //dump();

    //exit(1);

    t.stop();

    std::cout << "  total SBVH construction time ......: "
              << t.getElapsedSeconds() << " sec, "
              << t.getElapsedNanoSeconds() << " nsec.";
}

bool SBVH::intersect( const Ray &ray,
                     IntersectionRecord &intersection_record,
                     long unsigned int &num_intersection_tests_,
                     long unsigned int &num_intersections_ ) const
{
    return traverse( root_node_, ray, intersection_record, num_intersection_tests_, num_intersections_ );
    //return true;
}

float SBVH::SAH(std::size_t s1_size,
                float s1_area,
                std::size_t s2_size,
                float s2_area,
                float root_aabb_area) {
    if ((s2_area == std::numeric_limits<float>::infinity()) && (s2_size == 0))
        return std::numeric_limits<float>::infinity();

    return 2.0f * cost_intersec_aabb_ + ((s1_area / root_aabb_area) * s1_size * cost_intersec_tri_) +
           ((s2_area / root_aabb_area) * s2_size * cost_intersec_tri_);
}

void SBVH::splitNode(SBVHNode::UniquePtr& node, float root_aabb_area) {
    ///////////////////////////////////////////////////////////////////////////
    // object split evaluation
    ///////////////////////////////////////////////////////////////////////////

    float best_cost = cost_intersec_tri_ * node->primitive_ref_list_->size();
    int best_axis = -1;
    int best_event = -1;

    for (int axis = 1; axis <= 3; axis++) {
        switch (axis) {
            case 1:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInX);
                break;
            case 2:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInY);
                break;
            case 3:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInZ);
                break;
        }

        AABB aux_aabb;
        std::vector<float> left_area(node->primitive_ref_list_->size());

        for (long int i = 0; i < node->primitive_ref_list_->size(); ++i) {
            if (!i) {
                left_area[i] = std::numeric_limits<float>::infinity();
                aux_aabb = (*node->primitive_ref_list_)[i].aabb_;
            } else {
                left_area[i] = aux_aabb.getArea();
                aux_aabb = aux_aabb + (*node->primitive_ref_list_)[i].aabb_;
            }
        }

        std::vector<float> right_area(node->primitive_ref_list_->size());

        for (long int i = node->primitive_ref_list_->size() - 1; i >= 0; --i) {
            if (i == node->primitive_ref_list_->size() - 1) {
                right_area[i] = std::numeric_limits<float>::infinity();
                aux_aabb = (*node->primitive_ref_list_)[i].aabb_;
            } else {
                right_area[i] = aux_aabb.getArea();
                aux_aabb = aux_aabb + (*node->primitive_ref_list_)[i].aabb_;
            }

            float this_cost = SAH(i + 1,
                                  left_area[i],
                                  node->primitive_ref_list_->size() - 1 - i,
                                  right_area[i],
                                  root_aabb_area);

            if (this_cost < best_cost) {
                best_cost = this_cost;
                best_event = i;
                best_axis = axis;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // spatial split evaluation
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // select the better one: object or spatial split
    ///////////////////////////////////////////////////////////////////////////

    if ( best_axis == -1 ) // This is a leaf node
    {
        primitives_inserted_ += node->primitive_ref_list_->size();
        std::stringstream progress_stream;
        progress_stream << "\r  SBVH building progress ............: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0f * static_cast< float >( primitives_inserted_ ) / primitives_.size()
                        << "%";
        std::clog << progress_stream.str();
    } else { // This is an inner node
        // Make inner node with best_axis / best_event
        switch (best_axis) {
            case 1:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInX);
                break;
            case 2:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInY);
                break;
            case 3:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInZ);
                break;
        }

        node->left_node_ = SBVHNode::UniquePtr(new SBVHNode());
        node->left_node_->primitive_ref_list_ =
            SBVHNode::PrimitiveRefListUniquePtr(
                new std::vector<PrimitiveRef>(node->primitive_ref_list_->begin(),
                                              node->primitive_ref_list_->begin() + best_event));

        for ( long int i = 0; i < node->left_node_->primitive_ref_list_->size(); ++i) {
            if ( !i )
                node->left_node_->aabb_ = (*node->left_node_->primitive_ref_list_)[i].aabb_;
            else
                node->left_node_->aabb_ = node->left_node_->aabb_ + (*node->left_node_->primitive_ref_list_)[i].aabb_;
        }
        node->left_node_->right_node_ = nullptr;
        node->left_node_->left_node_ = nullptr;

        node->right_node_ = SBVHNode::UniquePtr(new SBVHNode());
        node->right_node_->primitive_ref_list_ =
            SBVHNode::PrimitiveRefListUniquePtr(
                new std::vector<PrimitiveRef>(node->primitive_ref_list_->begin() + best_event,
                                              node->primitive_ref_list_->end()));

/*
        // DEBUG
        std::cout << "left (" << node->left_node_->primitive_ref_list_->size() << "): ";
        for (long int i = 0; i < node->left_node_->primitive_ref_list_->size(); ++i) {
            std::cout << (*node->left_node_->primitive_ref_list_)[i].id_ << ", ";
        }

        std::cout << "right (" << node->right_node_->primitive_ref_list_->size() << "): ";
        for (long int i = 0; i < node->right_node_->primitive_ref_list_->size(); ++i) {
            std::cout << (*node->right_node_->primitive_ref_list_)[i].id_ << ", ";
        }

        exit(1);
*/

        for ( long int i = 0; i < node->right_node_->primitive_ref_list_->size(); ++i) {
            if ( !i )
                node->right_node_->aabb_ = (*node->right_node_->primitive_ref_list_)[i].aabb_;
            else
                node->right_node_->aabb_ = node->right_node_->aabb_ + (*node->right_node_->primitive_ref_list_)[i].aabb_;
        }
        node->right_node_->right_node_ = nullptr;
        node->right_node_->left_node_ = nullptr;

        splitNode(node->left_node_, root_aabb_area);
        splitNode(node->right_node_, root_aabb_area);
    }
}

bool SBVH::traverse(const SBVHNode::UniquePtr& node,
                    const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const
{
    bool primitive_intersect = false;

//#define NO_LEAF_TRI_BOX

#ifdef NO_LEAF_TRI_BOX

    if (node) {
        if ((!node->left_node_) && (!node->right_node_)) { // is a leaf node
            IntersectionRecord tmp_intersection_record;

            if (node->primitive_ref_list_->size() == 1) { // node contains 1 triangle
                num_intersection_tests_++;
                if (primitives_[(*node->primitive_ref_list_)[0].id_]->intersect(ray, tmp_intersection_record)) { // if intersects...
                    num_intersections_++;
                    if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                        intersection_record = tmp_intersection_record;
                        primitive_intersect = true;
                    }
                }
            } else { // node contains more than one triangle
                num_intersection_tests_++;
                if (node->aabb_.intersect(ray)) {
                    num_intersections_++;
                    for (std::size_t primitive_id = 0; primitive_id < node->primitive_ref_list_->size(); primitive_id++) {
                        num_intersection_tests_++;
                        if (primitives_[(*node->primitive_ref_list_)[primitive_id].id_]->intersect(ray, tmp_intersection_record)) {
                            num_intersections_++;
                            if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                                intersection_record = tmp_intersection_record;
                                primitive_intersect = true;
                            }
                        }
                    }
                }
            }
        } else { // this is not a leaf node
            num_intersection_tests_++;
            if (node->aabb_.intersect(ray)) {
                num_intersections_++;

                if (traverse(node->left_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;

                if (traverse(node->right_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;
            }
        }
    }

#else

    if (node) {
        num_intersection_tests_++;

        if (node->aabb_.intersect(ray)) {
            num_intersections_++;

            if ((!node->left_node_) && (!node->right_node_)) { // is a leaf node
                IntersectionRecord tmp_intersection_record;

                for (std::size_t primitive_id = 0; primitive_id < node->primitive_ref_list_->size(); primitive_id++) {
                    num_intersection_tests_++;

                    if (primitives_[(*node->primitive_ref_list_)[primitive_id].id_]->intersect(ray, tmp_intersection_record)) {
                        num_intersections_++;

                        if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                            intersection_record = tmp_intersection_record;
                            primitive_intersect = true;
                        }
                    }
                }
            } else {
                if (traverse(node->left_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;

                if (traverse(node->right_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;
            }
        }
    }

#endif // NO_LEAF_TRI_BOX

    return primitive_intersect;
}

void SBVH::dump( void ) const {
    std::queue<SBVHNode*> queue;
    SBVHNode* node = nullptr;
    int depth = 0;

    if ( root_node_ ) {
        queue.push( root_node_.get() );
        queue.push( nullptr );

        while( !queue.empty() ) {
            node = queue.front();
            queue.pop();

            if ( node ) {
                if ( ( !node->left_node_ ) && ( !node->right_node_ ) )
                    std::cout << "l";
                else
                    std::cout << "i";

                std::cout << " " << node->aabb_.min_.x << " " << node->aabb_.min_.y << " " << node->aabb_.min_.z;
                std::cout << " " << node->aabb_.max_.x << " " << node->aabb_.max_.y << " " << node->aabb_.max_.z;
                std::cout << "\n";

                if ( node->left_node_ )
                    queue.push( node->left_node_.get() );
                if ( node->right_node_ )
                    queue.push( node->right_node_.get() );
            } else
                if ( !queue.empty() ) {
                    queue.push( nullptr );
                    std::cout << "\n";
                    depth++;
                }
        }
    }

    /*
    std::queue< SBVHNode* > queue;
    SBVHNode *node = nullptr;
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
                    std::cout << "l";
                else
                    std::cout << "i";

                std::cout << " " << node->aabb_.min_.x << " " << node->aabb_.min_.y << " " << node->aabb_.min_.z;
                std::cout << " " << node->aabb_.max_.x << " " << node->aabb_.max_.y << " " << node->aabb_.max_.z;
                std::cout << "\n";

                if ( node->left_ )
                    queue.push( node->left_ );
                if ( node->right_ )
                    queue.push( node->right_ );
            }
            else
                if ( !queue.empty() )
                {
                    queue.push( nullptr );
                    std::cout << "\n";
                    depth++;
                }
        }
    }
    //*/
}
