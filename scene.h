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
#include "bvh.h"

class Scene
{
public:

    Scene( void );

    void pushPrimitive( Primitive *primitive );

    void pushMaterial( const Material *material );

    int loadMesh( const std::string &file_name,
                  const Material &material,
                  glm::dvec3 &min_aabb,
                  glm::dvec3 &max_aabb );

    void buildBVH( void );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const;

    void printInfo( void ) const;

    std::vector< Primitive::PrimitiveUniquePtr > primitives_;

    // TODO: use pointers here too???
    std::list< Material > materials_;

private:

    const aiScene *assimp_scene_ = nullptr;

    const BVH *bvh_ = nullptr;
};

#endif /* SCENE_H_ */
