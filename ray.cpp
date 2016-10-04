#include "ray.h"

Ray::Ray( void )
{ }

Ray::Ray( const glm::dvec3 &origin,
          const glm::dvec3 &direction ) :
        origin_{ origin },
        direction_{ direction }
{ }

