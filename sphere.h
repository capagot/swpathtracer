#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class Sphere : public Primitive
{
public:

    Sphere( void );

    Sphere( const glm::vec3 &center,
            float radius,
            long unsigned int material_id );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    //AABB computeSBVHAABB(float p0, float p1, int axis);
    bool computeSBVHAABB(float min_x, float max_x, float min_y, float max_y,  float min_z, float max_z,  int axis, AABB& aabb);

    AABB getAABB( void ) const;

    virtual void printData( void ) const;

    glm::vec3 center_ = { 0.0f, 0.0f, 0.0f };

    float radius_= 1.0f;

private:

    // TODO: this is replicated for each primitive... avoid that!
    static const float kIntersectionTestEpsilon_;
};

#endif /* SPHERE_H_ */
