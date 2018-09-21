#ifndef SBVH_H_
#define SBVH_H_

#include <sstream>
#include <iostream>
#include <iomanip>
#include <deque>
#include <queue>
#include <algorithm>

#include "primitive.h"
#include "timer.h"

class SBVH
{
public:
    struct PrimitiveRef {
        PrimitiveRef(long int id, const AABB& aabb, const glm::vec3& centroid) : id_(id),
                                                      aabb_(aabb),
                                                      poly_centroid_(centroid) {}
        long int id_;           // index of the actual primitive
        AABB aabb_;             // polygon AABB
        glm::vec3 poly_centroid_;    // polygon centroid (polygon = the primitive clipped against node AABBs)
    };

    struct SBVHNode {
        using PrimitiveRefListUniquePtr = std::unique_ptr<std::vector<PrimitiveRef>>;
        using UniquePtr = std::unique_ptr<SBVHNode>;

        //SBVHNode() : primitive_ref_list_(new std::vector<PrimitiveRef>()) {}
        SBVHNode() {}

        PrimitiveRefListUniquePtr primitive_ref_list_;
        AABB aabb_;
        SBVHNode::UniquePtr left_node_;
        SBVHNode::UniquePtr right_node_;
    };

/*    struct PrimitiveAABBArea
    {
        std::size_t primitive_id_;
        glm::vec3 centroid_;
        AABB aabb_;
        float left_area_;
        float right_area_;

        AABB left_aabb_;
        AABB right_aabb_;

    };
*/
    SBVH(const std::vector< Primitive::PrimitiveUniquePtr >& primitives);

    //~SBVH( void );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const;

    void dump( void ) const;

private:

    struct Comparator {
        static bool sortInX(const PrimitiveRef& a, const PrimitiveRef& b) {
            //return a.aabb_.getCentroid().x < b.aabb_.getCentroid().x;
            return a.poly_centroid_.x < b.poly_centroid_.x;
        }

        static bool sortInY(const PrimitiveRef& a, const PrimitiveRef& b) {
            //return a.aabb_.getCentroid().y < b.aabb_.getCentroid().y;
            return a.poly_centroid_.y < b.poly_centroid_.y;
        }

        static bool sortInZ(const PrimitiveRef& a, const PrimitiveRef& b) {
            //return a.aabb_.getCentroid().z < b.aabb_.getCentroid().z;
            return a.poly_centroid_.z < b.poly_centroid_.z;
        }
    };

    float SAH(std::size_t s1_size,
              float s1_area,
              std::size_t s2_size,
              float s2_area,
              float root_aabb_area);

    //AABB ComputeAABB(const Primitive& primitive, float p0_offset, float p1_offset, int axis);

    void splitNode(SBVHNode::UniquePtr& node,
                   float root_aabb_area,
                   const std::string& ident,
                   const std::string& split_method,
                   const std::string& child_side);

    bool traverse( const SBVHNode::UniquePtr& node,
                   const Ray &ray,
                   IntersectionRecord &intersection_record,
                   long unsigned int &num_intersection_tests_,
                   long unsigned int &num_intersections_ ) const;

    SBVHNode::UniquePtr root_node_;
    float cost_intersec_tri_ = 0.8f;
    float cost_intersec_aabb_ = 0.2f;

/*    std::deque< long unsigned int > primitive_id_;*/

    const std::vector<Primitive::PrimitiveUniquePtr>& primitives_;

    std::size_t primitives_inserted_ = 0;
};

#endif /* SBVH_H_ */

