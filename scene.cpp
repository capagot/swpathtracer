/*
 * scene.cpp
 *
 *  Created on: May 14, 2016
 *      Author: christian
 */

#include "scene.h"

Scene::Scene( void )
{ }

void Scene::load ( void )
{
    glm::vec3 a{-0.5f, -0.5f, -1.0f, };
    glm::vec3 b{ 0.5f, -0.5f, -1.0f, };
    glm::vec3 c{ 0.5f,  0.5f, -1.0f, };
    spectrum_.push_back( Spectrum{ glm::vec3{ 1.0f, 0.0f, 0.0f } } );
    primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, spectrum_.back() } ) );

    glm::vec3 f{ 0.5f,  0.5f, -1.0f, };
    glm::vec3 d{-0.5f,  0.5f, -1.0f, };
    glm::vec3 e{-0.5f, -0.5f, -1.0f, };
    spectrum_.push_back( Spectrum{ glm::vec3{ 0.0f, 1.0f, 0.0f } } );
    primitives_.push_back( primitive_ptr( new Triangle{ d, e, f, spectrum_.back() } ) );
}

void Scene::printInfo( void )
{
    std::clog << "Scene Information" << std::endl;
    std::clog << "-----------------" << std::endl;

    std::size_t number_primitives = 0;

    //for( std::size_t i = 0; i < primitives_.; i++ )
    //    number_primitives += primitives_.size();

    std::clog << " number of primitives ..........: " << number_primitives << std::endl;
}
