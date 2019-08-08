#ifndef INTERSECTION_RECORD_H
#define INTERSECTION_RECORD_H

#include <glm/vec3.hpp>

struct IntersectionRecord {
    float t_;
    glm::vec3 position_;
    glm::vec3 normal_;
    std::size_t material_id_;
    std::size_t primitive_id_;
};

#endif  // INTERSECTION_RECORD_H
