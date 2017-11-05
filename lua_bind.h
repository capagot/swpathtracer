#ifndef LUA_BIND_H_
#define LUA_BIND_H_

#include <iostream>
#include <vector>

#include "lua.hpp"
#include <glm/glm.hpp>

#include "camera.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"
#include "sampler.h"
#include "primitive.h"
#include "scene.h"
#include "buffer.h"
#include "fresnel.h"

class LuaBind
{
public:

    LuaBind( const std::string &filename );

    ~LuaBind( void );

    void loadFromScript( Camera **camera,
                         Sampler **sampler,
                         Scene *scene,
                         Buffer **rendering_buffer,
                         glm::vec3 &background_color,
                         std::string &path_termination,
                         //std::size_t &max_path_depth,
                         std::size_t &path_length,
                         std::string &output_filename,
                         Scene::AccelerationStructure &scene_acceleration_data_structure,
                         RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

private:

    lua_State *lua_state_ = nullptr;

    void getCamera( Camera **camera );

    void getSampler( Sampler **sampler,
                     RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    void getBuffer( Buffer **rendering_buffer );

    void getGlobals( glm::vec3 &background_color,
                     std::string &path_termination,
                     //std::size_t &max_path_depth,
                     std::size_t &path_length,
                     std::string &output_filename,
                     Scene::AccelerationStructure &scene_acceleration_data_structure );

    void getMaterial( Scene *scene,
                      RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    void getTriangle( Scene *scene,
                      RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    void getSphere( Scene *scene,
                    RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    void getMesh( Scene *scene,
                  const Scene::MeshType mesh_type,
                  RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );


    void getElements( Camera **camera,
                      Sampler **sampler,
                      Scene *scene,
                      Buffer **rendering_buffer,
                      glm::vec3 &background_color,
                      std::string &path_termination,
                      //max_path_depth,
                      std::size_t &path_length,
                      std::string &output_filename,
                      Scene::AccelerationStructure &scene_acceleration_data_structure,
                      RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    std::string parseString( const std::string &s_input );

    float parseScalar( const std::string &s );

    glm::vec3 parseVec3( const std::string &s );

    void parseVertices( const std::string &s,
                        glm::vec3 *v );

};

#endif /* LUA_BIND_H_ */
