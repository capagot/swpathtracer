/*
 * ray.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "ray.h"

Ray::Ray( void )
{ }

Ray::Ray( const glm::vec3 &origin,
          const glm::vec3 &direction ) :
        origin_{ origin },
        direction_{ direction }
{ }
