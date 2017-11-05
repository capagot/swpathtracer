#ifndef BSDF_H_
#define BSDF_H_

#include "bxdf.h"
#include "random.h"

class BSDF
{
public:

    using BSDFUniquePtr = std::unique_ptr< BSDF >;

    BSDF( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    glm::vec3 fr( const glm::vec3 &w_i,
                   const std::vector< glm::vec3 > &w_r ) const;

    void getNewDirection( const glm::vec3 &w_i, std::vector< glm::vec3 > &w_r );

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

    std::vector< BxDF::BxDFUniquePtr > bxdf_layers_;

};

#endif /* BSDF_H_ */
