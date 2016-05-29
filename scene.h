/*
 * scene.h
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <list>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include "triangle.h"
#include "spectrum.h"

class Scene
{
public:

    Scene( void );

    // TODO: extend this function to load from files or other interfaces....
    void load( void );

    void printInfo( void );

    // list of materials (BRDFs)
    // list of light sources (separated??)

    typedef std::unique_ptr< Primitive > primitive_ptr;

    std::vector< primitive_ptr > primitives_;

    std::list< Spectrum > spectrum_;

};

#endif /* SCENE_H_ */
