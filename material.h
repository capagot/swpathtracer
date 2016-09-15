/*
 * material.h
 *
 *  Created on: Jun 5, 2016
 *      Author: christian
 */

#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include  "onb.h"
#include  "ray.h"
#include "random.h"

class Lambertian
{
public:

    Lambertian( void );

    Lambertian( const glm::vec3 &brdf, const glm::vec3 &emitted );

    glm::vec3 getNewDirection( const glm::vec3 &normal,
                               RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    glm::vec3 brdf_;

    // TODO: there is no emission in a lambertian surface!!! :P
    glm::vec3 emitted_;
};

#endif /* LAMBERTIAN_H_ */
