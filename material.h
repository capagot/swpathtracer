#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <memory>

#include "lambertian.h"
#include "bsdf.h"

class Material
{
public:

    using MaterialUniquePtr = std::unique_ptr< Material >;

    Material( void );

    Material( BSDF::BSDFUniquePtr bsdf,
              const glm::dvec3 &emitted );

    BSDF::BSDFUniquePtr bsdf_;

    glm::dvec3 emitted_;
};

#endif /* MATERIAL_H_ */

