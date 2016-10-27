#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class Sphere : public Primitive
{
public:

    Sphere( void );

    Sphere( const glm::dvec3 &center,
            double radius,
            const Material *material );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    AABB getAABB( void ) const;

    glm::dvec3 center_ = { 0.0, 0.0, 0.0 };

    double radius_= 1.0;

private:

    // TODO: this is replicated for each primitive... avoid that!
    static const double kIntersectionTestEpsilon_;
};

#endif /* SPHERE_H_ */

