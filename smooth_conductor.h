#ifndef SMOOTH_CONDUCTOR_H_
#define SMOOTH_CONDUCTOR_H_

#include <glm/glm.hpp>

#include "onb.h"
#include "random.h"
#include "bxdf.h"
#include "fresnel.h"

class SmoothConductor : public BxDF
{
public:

    SmoothConductor( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                     Fresnel::FresnelUniquePtr fresnel );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const;

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const;

//private:

//    Fresnel::FresnelUniquePtr fresnel_;

};

#endif /* SMOOTH_CONDUCTOR_H_ */

