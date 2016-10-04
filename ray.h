#ifndef RAY_H_
#define RAY_H_

#include <glm/glm.hpp>

class Ray
{
public:

    Ray( void );

    Ray( const glm::dvec3 &origin,
         const glm::dvec3 &direction );

    glm::dvec3 origin_{ 0.0, 0.0, 0.0 };

    glm::dvec3 direction_{ 0.0, 0.0, -1.0 };
};

#endif /* RAY_H_ */

