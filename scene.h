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
#include "smooth_triangle.h"
#include "sphere.h"
#include "lambertian.h"
#include "cook_torrance.h"
#include "smooth_conductor.h"
#include "smooth_dielectric.h"
#include "material.h"
#include "bvh.h"

class Scene
{
public:

    enum AccelerationStructure
    {
        NONE,
        BVH_SAH
    };

    enum MeshType
    {
        FLAT,
        SMOOTH
    };

    Scene( void );

    ~Scene( void );

    //void pushPrimitive( Primitive *primitive );

    //void pushMaterial( long unsigned int material_id );

    int loadMesh( const std::string &file_name,
                  const Scene::MeshType mesh_type,
                  glm::dvec3 &min_aabb,
                  glm::dvec3 &max_aabb );

    void buildAccelerationStructure( void );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const;

    void printInfoPreAccelerationStructure( void ) const;

    void printInfoPostAccelerationStructure( void ) const;

    std::vector< Primitive::PrimitiveUniquePtr > primitives_;

    std::vector< Material::MaterialUniquePtr > materials_;

    // TODO: move this to private section
    AccelerationStructure acceleration_structure_ = AccelerationStructure::NONE;

private:

    void buildBVH( void );

    const aiScene *assimp_scene_ = nullptr;

    const BVH *bvh_ = nullptr;

};

#endif /* SCENE_H_ */
