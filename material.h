#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <memory>

#include "lambertian.h"

class Material
{
public:

    typedef std::unique_ptr< Material > MaterialUniquePtr;

    Material( void );

    Material( const Lambertian &bxdf,
              const glm::dvec3 &emitted );

    Lambertian bxdf_;

    glm::dvec3 emitted_;
};

#endif /* MATERIAL_H_ */

