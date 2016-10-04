#include "material.h"

Material::Material( void )
{ }

Material::Material( const Lambertian &bxdf, 
                    const glm::dvec3 &emitted ) :
        bxdf_{ bxdf },
        emitted_{ emitted }
{ }

