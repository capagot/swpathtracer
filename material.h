/*
 * material.h
 *
 *  Created on: Jun 5, 2016
 *      Author: christian
 */

#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "onb.h"
#include "random.h"

class Lambertian
{
public:

    Lambertian( void );

    Lambertian( const glm::vec3 &radiance );

    glm::vec3 getNewDirection( const glm::vec3 &normal,
                               RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    glm::vec3 radiance_ = glm::vec3{ 0.5f, 0.5f, 0.5f };

};

#endif /* LAMBERTIAN_H_ */

