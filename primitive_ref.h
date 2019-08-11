#ifndef PRIMITIVE_REF_H
#define PRIMITIVE_REF_H

#include "aabb.h"

/*
 * Each reference is composed by the actual primitive id (id_), an aabb (aabb_) and
 * the centroid (poly_centroid_) of the portion of the primitive (an n-sided polygon)
 * bounded by the aabb.
 */
struct PrimitiveRef {
    PrimitiveRef(long int id, const AABB& aabb, const glm::vec3& centroid)
        : id_(id), aabb_(aabb), poly_centroid_(centroid) {}
    long int id_;
    AABB aabb_;
    glm::vec3 poly_centroid_;
};

#endif  // PRIMITIVE_REF_H
