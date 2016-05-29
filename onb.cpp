/*
 * onb.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "onb.h"

ONB::ONB( void )
{ }

void ONB::setUpONB( const glm::vec3 &up,
                    const glm::vec3 &direction )
{
    // right-hand ONB
    w_ = -glm::normalize( direction );
    u_ =  glm::normalize( glm::cross( up, w_ ) );
    v_ =  glm::cross( w_, u_ );
}
