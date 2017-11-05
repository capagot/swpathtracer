#include "ray.h"

Ray::Ray( void )
{ }

Ray::Ray( const glm::vec3 &origin,
          const glm::vec3 &direction ) :
        origin_{ origin },
        direction_{ direction }
{ }

