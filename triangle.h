#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class Triangle : public Primitive
{
public:

    Triangle( void );

    Triangle( const glm::vec3 &v0,
              const glm::vec3 &v1,
              const glm::vec3 &v2,
              long unsigned int material_id );

    Triangle( const glm::vec3 &v0,
              const glm::vec3 &v1,
              const glm::vec3 &v2,
              const glm::vec3 &normal,
              long unsigned int material_id );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    //AABB computeSBVHAABB(float p0, float p1, int axis);

    bool computeSBVHAABB(float min_x, float max_x, float min_y, float max_y,  float min_z, float max_z,  int axis, AABB& aabb);

    AABB getAABB( void ) const;

    virtual void printData( void ) const;

    glm::vec3 v0_ = { 0.0f, 0.0f, 0.0f };

    glm::vec3 v1_ = { 1.0f, 0.0f, 0.0f };

    glm::vec3 v2_ = { 0.0f, 1.0f, 0.0f };

    glm::vec3 normal_ = { 0.0f, 0.0f, 1.0f };

private:

    static const float kIntersectionTestEpsilon_;
};

#endif /* TRIANGLE_H_ */
