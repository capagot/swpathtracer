#ifndef SMOOTH_SPECULAR_REFLECTION_H_
#define SMOOTH_SPECULAR_REFLECTION_H_

#include <glm/glm.hpp>

#include "onb.h"
#include "random.h"
#include "bxdf.h"
#include "fresnel.h"

class SmoothSpecularReflection : public BxDF
{
public:

    SmoothSpecularReflection( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                              Fresnel::FresnelUniquePtr fresnel );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const;

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const;

private:

    Fresnel::FresnelUniquePtr fresnel_;

};

#endif /* SMOOTH_SPECULAR_REFLECTION_H_ */
