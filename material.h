#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "lambertian.h"

class Material
{
public:

    Material( void );

    Material( const Lambertian &bxdf, 
              const glm::vec3 &emitted );

    Lambertian bxdf_;

    glm::vec3 emitted_;
};

#endif /* MATERIAL_H_ */

