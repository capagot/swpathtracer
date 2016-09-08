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

void ONB::setUpFromV( const glm::vec3 &v )
{
    v_ = glm::normalize( v );
    
    if( fabs( v_.x ) > fabs( v_.y ) )
        w_ = glm::normalize( glm::vec3{ v_.z, 0.0f, -v_.x } );
    else
        w_ =  glm::normalize( glm::vec3{ 0.0f, -v_.z, v_.y } );

    u_ = glm::cross( v_, w_ );

    m_[0] = w_;
    m_[1] = v_;
    m_[2] = u_;

}

