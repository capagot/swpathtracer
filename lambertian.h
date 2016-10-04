#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "onb.h"
#include "random.h"

class Lambertian
{
public:

    Lambertian( void );

    Lambertian( const glm::dvec3 &radiance );

    glm::dvec3 getNewDirection( const glm::dvec3 &normal,
                                RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const;

    glm::dvec3 radiance_ = glm::dvec3{ 0.5, 0.5, 0.5 };

};

#endif /* LAMBERTIAN_H_ */

