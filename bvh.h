#ifndef BVH_H_
#define BVH_H_

#include <sstream>
#include <iostream>
#include <iomanip>
#include <deque>
#include <queue>
#include <algorithm>

#include "primitive.h"
#include "timer.h"

class BVH
{
public:

    struct BVHNode
    {
        ~BVHNode( void )
        {
            if ( left_ )
            {
                delete left_;
                left_ = nullptr;
            }

            if ( right_ )
            {
                delete right_;
                right_ = nullptr;
            }
        }

        std::size_t first_;                       // index of the first primitive
        std::size_t last_;                        // number of primitives into this node (whose index start at "first_").
        AABB aabb_;                               // AABB represeted by the current node.
        BVHNode *left_ = nullptr;                 // Pointer to the left child node (if the current node is a inner node).
        BVHNode *right_ = nullptr;                // Pointer to right inner node (if the current node is a inner node).
    };

    struct PrimitiveAABBArea
    {
        std::size_t primitive_id_;
        glm::vec3 centroid_;
        AABB aabb_;
        float left_area_;
        float right_area_;

        AABB left_aabb_;
        AABB right_aabb_;

    };

    BVH( const std::vector< Primitive::PrimitiveUniquePtr >& primitives );

    ~BVH( void );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const;

    void dump( void ) const;

private:

    struct Comparator
    {
        static bool sortInX( const PrimitiveAABBArea &lhs, const PrimitiveAABBArea &rhs )
        {
            return lhs.centroid_.x < rhs.centroid_.x;
        }

        static bool sortInY( const PrimitiveAABBArea &lhs, const PrimitiveAABBArea &rhs )
        {
            return lhs.centroid_.y < rhs.centroid_.y;
        }

        static bool sortInZ( const PrimitiveAABBArea &lhs, const PrimitiveAABBArea &rhs )
        {
            return lhs.centroid_.z < rhs.centroid_.z;
        }
    };

    float SAH( std::size_t s1_size,
                float s1_area,
                std::size_t s2_size,
                float s2_area,
                float s_area  );

    void splitNode( BVHNode **node,
                    std::deque< PrimitiveAABBArea > &s,
                    std::size_t first,
                    std::size_t last,
                    float s_area );

    bool traverse( const BVHNode *node,
                   const Ray &ray,
                   IntersectionRecord &intersection_record,
                   long unsigned int &num_intersection_tests_,
                   long unsigned int &num_intersections_ ) const;

    BVHNode *root_node_ = nullptr;

    float cost_intersec_tri_ = 0.8f;

    float cost_intersec_aabb_ = 0.2f;

    std::deque< long unsigned int > primitive_id_;

    const std::vector< Primitive::PrimitiveUniquePtr > &primitives_;

    std::size_t primitives_inserted_ = 0;
};

#endif /* BVH_H_ */

