#ifndef SCENE_H_
#define SCENE_H_

#include <list>
#include <iostream>
#include <fstream>
#include <memory>
#include <glm/glm.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "triangle.h"
#include "sphere.h"
#include "lambertian.h"
#include "material.h"

class Scene
{
public:

    typedef std::unique_ptr< Primitive > primitive_ptr;

    Scene( void );

    void pushPrimitive( Primitive *primitive );

    void pushMaterial( const Material *material );

    int loadMesh( const std::string &file_name,
                  const Material &material,
                  glm::vec3 &min_aabb,
                  glm::vec3 &max_aabb );


    void printInfo( void ) const;

    std::vector< primitive_ptr > primitives_;

    // TODO: use pointers here too???
    std::list< Material > materials_;

private:

    const aiScene* assimp_scene_ = nullptr;
};

#endif /* SCENE_H_ */

