#ifndef BSDF_H_
#define BSDF_H_

#include "bxdf.h"

class BSDF
{
public:

    using BSDFUniquePtr = std::unique_ptr< BSDF >;

    BSDF( void )
    {};

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const;

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const;

    std::vector< BxDF::BxDFUniquePtr > bxdf_layers_;

};

#endif /* BSDF_H_ */
