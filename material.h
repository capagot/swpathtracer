/*
 * material.h
 *
 *  Created on: Jun 5, 2016
 *      Author: christian
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include  "spectrum.h"
#include  "ray.h"

class Material
{
public:

    Material( void );

    Material( const Spectrum &brdf, const Spectrum &emitted );

    Ray getRayDirection( const glm::vec3 &normal, float r1, float r2 ) const;

    Spectrum brdf_;

    Spectrum emitted_;
};

#endif /* MATERIAL_H_ */
