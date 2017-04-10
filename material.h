#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <memory>

#include "lambertian.h"

class Material
{
public:

    typedef std::unique_ptr< Material > MaterialUniquePtr;

    Material( void );

    Material( BxDF::BxDFUniquePtr bxdf,
              const glm::dvec3 &emitted );

    BxDF::BxDFUniquePtr bxdf_;

    glm::dvec3 emitted_;
};

#endif /* MATERIAL_H_ */
