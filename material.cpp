#include "material.h"

Material::Material( void )
{ }

Material::Material( BxDF::BxDFUniquePtr bxdf,
                    const glm::dvec3 &emitted ) :
        bxdf_( std::move( bxdf ) ),
        emitted_{ emitted }
{ }
