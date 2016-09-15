/*
 * onb.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "onb.h"

ONB::ONB( void )
{ }

void ONB::setFromUW( const glm::vec3 &u,
                     const glm::vec3 &w )
{
    u_ = u;
    w_ = w;
    v_ =  glm::cross( w_, u_ );

    setBasisMatrix();
}

void ONB::setFromV( const glm::vec3 &v )
{
    v_ = v;

    if( fabs( v_.x ) > fabs( v_.y ) )
        w_ = glm::normalize( glm::vec3{ v_.z, 0.0f, -v_.x } );
    else
        w_ =  glm::normalize( glm::vec3{ 0.0f, -v_.z, v_.y } );

    u_ = glm::cross( v_, w_ );

    setBasisMatrix();
}

void ONB::setBasisMatrix( void )
{
    m_[0] = u_; // sets the column 0 of m_.
    m_[1] = v_; // sets the column 1 of m_.
    m_[2] = w_; // sets the column 2 of m_.
}
