/*
 * onb.h
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#ifndef ONB_H_
#define ONB_H_

#include <glm/glm.hpp>

class ONB
{
public:

    ONB( void );

    void setUpONB( const glm::vec3 &up,
                   const glm::vec3 &direction );

    glm::vec3 u_ = { 1.0f, 0.0f, 0.0f };

    glm::vec3 v_ = { 0.0f, 1.0f, 0.0f };

    glm::vec3 w_ = { 0.0f, 0.0f, 1.0f };
};

#endif /* ONB_H_ */
