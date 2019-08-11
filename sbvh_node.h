#ifndef SBVH_NODE_H
#define SBVH_NODE_H

#include <algorithm>
#include <memory>
#include <vector>

#include "primitive_ref.h"

/*
 * A node of the SBVH is described by its aabb, the list of references (i.e. primitives)
 * that are (eventually partially) contained in the aabb and the two pointers to its child nodes.
 */
struct SBVHNode {
    /*
     * Define the criterion used to sort the primitives along each axis.
     * In this case, the the corresponding centroid coordinate is used for sorting.
     */
    struct Comparator {
        // sorting along X axis
        static bool sortInX(const PrimitiveRef& a, const PrimitiveRef& b) {
            return a.poly_centroid_.x < b.poly_centroid_.x;
        }

        // sorting along Y axis
        static bool sortInY(const PrimitiveRef& a, const PrimitiveRef& b) {
            return a.poly_centroid_.y < b.poly_centroid_.y;
        }

        // sorting along Z axis
        static bool sortInZ(const PrimitiveRef& a, const PrimitiveRef& b) {
            return a.poly_centroid_.z < b.poly_centroid_.z;
        }
    };

    SBVHNode() {}

    /*
     * Sort node references (i.e. primitives) along the informed axis.
     * Sorting is based on the references' centroids.
     */
    void sortReferences(int axis) {
        switch (axis) {
            case 0:
                std::sort(primitive_ref_list_->begin(), primitive_ref_list_->end(), Comparator::sortInX);
                break;
            case 1:
                std::sort(primitive_ref_list_->begin(), primitive_ref_list_->end(), Comparator::sortInY);
                break;
            case 2:
                std::sort(primitive_ref_list_->begin(), primitive_ref_list_->end(), Comparator::sortInZ);
                break;
        }
    }

    std::size_t getNumReferences() const;

    PrimitiveRef& getReferenceAt(unsigned long int id) const;

    unsigned int getActualPrimitiveId(unsigned long int id) const;

    std::unique_ptr<std::vector<PrimitiveRef>> primitive_ref_list_;
    AABB aabb_;
    std::unique_ptr<SBVHNode> left_node_;
    std::unique_ptr<SBVHNode> right_node_;
};

#endif  // SBVH_NODE_H
