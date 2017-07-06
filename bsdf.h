#ifndef BSDF_H_
#define BSDF_H_

#include "bxdf.h"
#include "random.h"

class BSDF
{
public:

    using BSDFUniquePtr = std::unique_ptr< BSDF >;

    BSDF( RNG< std::uniform_real_distribution, double, std::mt19937 > &rng );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const std::vector< glm::dvec3 > &w_r ) const;

    void getNewDirection( const glm::dvec3 &w_i, std::vector< glm::dvec3 > &w_r );

    RNG< std::uniform_real_distribution, double, std::mt19937 > rng_;

    std::vector< BxDF::BxDFUniquePtr > bxdf_layers_;

};

#endif /* BSDF_H_ */
