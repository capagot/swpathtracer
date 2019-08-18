#ifndef ACCELERATION_SBVH_NODE_H
#define ACCELERATION_SBVH_NODE_H

#include <algorithm>
#include <memory>
#include <vector>

#include "primitive_ref.h"

class SBVHNode {
   public:
    struct Comparator {
        static bool sortInX(const PrimitiveRef& a, const PrimitiveRef& b) {
            return a.poly_centroid_.x < b.poly_centroid_.x;
        }

        static bool sortInY(const PrimitiveRef& a, const PrimitiveRef& b) {
            return a.poly_centroid_.y < b.poly_centroid_.y;
        }

        static bool sortInZ(const PrimitiveRef& a, const PrimitiveRef& b) {
            return a.poly_centroid_.z < b.poly_centroid_.z;
        }
    };

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
    inline std::size_t getNumReferences() const {
        return primitive_ref_list_->size();
    }
    inline PrimitiveRef& getReferenceAt(unsigned long int id) const {
        return (*primitive_ref_list_)[id];
    }
    inline unsigned int getActualPrimitiveId(unsigned long int id) const {
        return (*primitive_ref_list_)[id].id_;
    }

    std::unique_ptr<std::vector<PrimitiveRef>> primitive_ref_list_;
    AABB aabb_;
    std::unique_ptr<SBVHNode> left_node_;
    std::unique_ptr<SBVHNode> right_node_;
};

#endif  // ACCELERATION_SBVH_NODE_H
