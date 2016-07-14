/*
 * material.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: christian
 */

#include  "material.h"

Material::Material( void ) :
        brdf_{ glm::vec3{ 0.8f, 0.8f, 0.8f } },
        emitted_{ glm::vec3{ 0.0f, 0.0f, 0.0f } }
{ }

Material::Material( const Spectrum &brdf, const Spectrum &emitted ) :
        brdf_{ brdf },
        emitted_{ emitted }
{ }

Ray Material::getRayDirection( const glm::vec3 &normal, float r1, float r2 ) const
{
    double r2s = sqrt(r2);
    //double r1 = r1_ * 2.0 * 3.1415;
    //return  normal((onb.getU() * cos(r1) * r2s) +
    //        (onb.getV() * sin(r1) * r2s) +
    //        (onb.getW() * sqrt(1.0-r2_)));
}
