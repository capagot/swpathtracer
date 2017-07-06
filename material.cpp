#include "material.h"

Material::Material( void )
{ }

Material::Material( BSDF::BSDFUniquePtr bsdf,
                    const glm::dvec3 &emitted ) :
        bsdf_( std::move( bsdf ) ),
        emitted_{ emitted }
{ }
