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
#include <fstream>
#include <memory>
#include <glm/glm.hpp>

// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp"  //OO version Header!
#include "assimp/scene.h" //#include "assimp/Scene.h"
#include "assimp/postprocess.h" //#include "assimp/PostProcess.h"

#include "triangle.h"
#include "spectrum.h"

class Scene
{
public:

    Scene( void );

    void load( void ); // TODO: remove this in the future.

    int loadFromFile( const std::string &file_name );

    void printInfo( void );

    // list of materials (BRDFs)
    // list of light sources (separated??)

    typedef std::unique_ptr< Primitive > primitive_ptr;

    std::vector< primitive_ptr > primitives_;

    std::list< Spectrum > spectrum_;

private:

    const aiScene* assimp_scene_ = nullptr;
};

#endif /* SCENE_H_ */
