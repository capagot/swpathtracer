#include "material.h"

Material::Material( void )
{ }

Material::Material( BRDF::BRDFUniquePtr brdf,
                    const glm::dvec3 &emitted ) :
        brdf_( std::move( brdf ) ),
        emitted_{ emitted }
{ }
