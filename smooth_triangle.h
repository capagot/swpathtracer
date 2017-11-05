#ifndef SMOOTH_TRIANGLE_H_
#define SMOOTH_TRIANGLE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class SmoothTriangle : public Primitive
{
public:

    SmoothTriangle( void );

    SmoothTriangle( const glm::vec3 &v0,
                    const glm::vec3 &v1,
                    const glm::vec3 &v2,
                    const glm::vec3 &n0,
                    const glm::vec3 &n1,
                    const glm::vec3 &n2,
                    long unsigned int material_id );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    glm::vec3 getUniformPoint( float e1, float e2 ) const;

    AABB getAABB( void ) const;

    virtual void printData( void ) const;

    glm::vec3 v0_;

    glm::vec3 v1_;

    glm::vec3 v2_;

    glm::vec3 n0_;

    glm::vec3 n1_;

    glm::vec3 n2_;

private:

    static const float kIntersectionTestEpsilon_;
};

#endif /* SMOOTH_TRIANGLE_H_ */

