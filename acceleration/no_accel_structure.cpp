#include "no_accel_structure.h"

NoAccelStructure::NoAccelStructure() : AccelStructure(AccelStructure::Type::NONE) {}

NoAccelStructure::~NoAccelStructure() {}

void NoAccelStructure::build() {}

bool NoAccelStructure::intersect(const Ray& ray, IntersectionRecord& intersection_record,
                                 std::size_t& num_intersection_tests, std::size_t& num_intersections) const {
    IntersectionRecord tmp_intersection_record;
    intersection_record.t_ = std::numeric_limits<float>::infinity();
    bool intersection_result = false;
    std::size_t num_primitives = primitives_->size();
    num_intersection_tests = 0;
    num_intersections = 0;

    for (std::size_t i = 0; i < num_primitives; ++i) {
        ++num_intersection_tests;
        if ((*primitives_)[i]->intersect(ray, tmp_intersection_record)) {
            ++num_intersections;
            if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                intersection_record = tmp_intersection_record;
                intersection_result = true;
            }
        }
    }

    return intersection_result;
}
