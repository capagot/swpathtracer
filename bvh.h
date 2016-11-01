#ifndef BVH_H_
#define BVH_H_

#include <iostream>
#include <iomanip>
#include <deque>
#include <queue>
#include <algorithm>

#include "primitive.h"

class BVH
{
public:

    // TODO: refactor this: there is no need to keep precomputed stuff that wont be used anywhere else!!
    struct BVHPrimitive
    {
        std::size_t primitive_id_;
        AABB aabb_;
    };

    struct PrimitiveAABBArea
    {
        std::size_t primitive_id_;
        glm::dvec3 centroid_;
        AABB aabb_;
        double left_area_;
        double right_area_;
    };

    struct BVHNode
    {
        std::size_t first_;
        std::size_t last_;
        AABB aabb_;                               // AABB represeted by the current node.
        BVHNode *left_ = nullptr;                 // Pointer to the left child node (if the current node is a inner node).
        BVHNode *right_ = nullptr;                // Pointer to right inner node (if the current node is a inner node).
    };

    BVH( const std::vector< Primitive::PrimitiveUniquePtr > &primitives );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const;

    void dump( void ) const;

    void dumpPrimitives( void ) const;

    ~BVH( void );

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

    double area( const std::deque< PrimitiveAABBArea > &s ) const;

    double SAH( std::size_t s1_size,
                double s1_area,
                std::size_t s2_size,
                double s2_area,
                double s_area  );

    void splitNode( BVHNode **node,
                    std::deque< PrimitiveAABBArea > &s,
                    std::size_t first,
                    std::size_t last,
                    double s_area );

    bool traverse( const BVHNode *node,
                   const Ray &ray,
                   IntersectionRecord &intersection_record,
                   std::string path_str ) const;

    BVHNode *root_node_ = nullptr;

    double cost_intersec_tri_ = 0.8;

    double cost_intersec_aabb_ = 0.2;

    std::deque< BVHPrimitive > s_;

    // TODO: refactor this!
    const std::vector< Primitive::PrimitiveUniquePtr > &primitives_;
};

#endif /* BVH_H_ */
