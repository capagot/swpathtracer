#include "material2.h"

Material::Material( void )
{ }

Material::Material( const Lambertian &bxdf, 
                    const glm::vec3 &emitted ) :
        bxdf_{ bxdf },
        emitted_{ emitted }
{ }

