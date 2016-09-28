/*
 * scene.cpp
 *
 *  Created on: May 14, 2016
 *      Author: christian
 */

#include "scene.h"

Scene::Scene( void )
{ }

void Scene::pushPrimitive( Primitive *primitive )
{
    primitives_.push_back( primitive_ptr( primitive ) );
}

void Scene::pushMaterial( const Material *material )
{
    materials_.push_back( *material );
}

// TODO: fix this to properly unwind function calls in the case of errors.
//       Actually, this has to be made for the entire program.
int Scene::loadFromFile( const std::string &file_name, glm::vec3 &min, glm::vec3 &max )
{
    // TODO: would Assimp handle file reading ??
    std::ifstream fin( file_name.c_str() );

    if( !fin.fail() )
        fin.close();
    else
    {
        std::cerr << "Couldn't open file: " << file_name << std::endl;
        return EXIT_FAILURE;
    }

    Assimp::Importer assimp_importer;

    assimp_scene_ = assimp_importer.ReadFile( file_name,
                                              //aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate            //|
                                              //aiProcess_JoinIdenticalVertices  |
                                              //aiProcess_SortByPType
                                              );

    if( !assimp_scene_ )
    {
        std::cerr << assimp_importer.GetErrorString() << std::endl;
        return EXIT_FAILURE;
    }

    if( assimp_scene_->HasMeshes() )
    {
        materials_.push_back( Material{ Lambertian{ glm::vec3{ 0.75f, 0.75f, 0.75f } }, glm::vec3{ 0.0f, 0.0f, 0.0f } } ); // Dummy material

        for( unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++ )
        {
            const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];

            for( unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3 )
            {
                const aiVector3D *vertex_ptr = &mesh_ptr->mVertices[vertex_id];

                // computes the model's aabb
                if ( vertex_id == 0 )
                {
                    min.x = std::min( std::min( vertex_ptr[0].x, vertex_ptr[1].x ), vertex_ptr[2].x );
                    min.y = std::min( std::min( vertex_ptr[0].y, vertex_ptr[1].y ), vertex_ptr[2].y );
                    min.z = std::min( std::min( vertex_ptr[0].z, vertex_ptr[1].z ), vertex_ptr[2].z );

                    max.x = std::max( std::max( vertex_ptr[0].x, vertex_ptr[1].x ), vertex_ptr[2].x );
                    max.y = std::max( std::max( vertex_ptr[0].y, vertex_ptr[1].y ), vertex_ptr[2].y );
                    max.z = std::max( std::max( vertex_ptr[0].z, vertex_ptr[1].z ), vertex_ptr[2].z );
                }
                else
                {
                    min.x = std::min( std::min( std::min( vertex_ptr[0].x, vertex_ptr[1].x ), vertex_ptr[2].x ), min.x );
                    min.y = std::min( std::min( std::min( vertex_ptr[0].y, vertex_ptr[1].y ), vertex_ptr[2].y ), min.y );
                    min.z = std::min( std::min( std::min( vertex_ptr[0].z, vertex_ptr[1].z ), vertex_ptr[2].z ), min.z );

                    max.x = std::max( std::max( std::max( vertex_ptr[0].x, vertex_ptr[1].x ), vertex_ptr[2].x ), max.x );
                    max.y = std::max( std::max( std::max( vertex_ptr[0].y, vertex_ptr[1].y ), vertex_ptr[2].y ), max.y );
                    max.z = std::max( std::max( std::max( vertex_ptr[0].z, vertex_ptr[1].z ), vertex_ptr[2].z ), max.z );
                }

                //glm::vec3 a{ vertex_ptr[0].x -0.75f, vertex_ptr[0].y - 2.0f, vertex_ptr[0].z - 0.25f };
                //glm::vec3 b{ vertex_ptr[1].x -0.75f, vertex_ptr[1].y - 2.0f, vertex_ptr[1].z - 0.25f };
                //glm::vec3 c{ vertex_ptr[2].x -0.75f, vertex_ptr[2].y - 2.0f, vertex_ptr[2].z - 0.25f };

                glm::vec3 a{ vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z - 0.75f };
                glm::vec3 b{ vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z - 0.75f };
                glm::vec3 c{ vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z - 0.75f };

                //primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, materials_.back() } ) );
            }
        }
    }

    return EXIT_SUCCESS;
}

void Scene::printInfo( void )
{
    std::clog << "Scene Information" << std::endl;
    std::clog << "-----------------" << std::endl;
    std::clog << " number of primitives ..........: " << primitives_.size() << std::endl;
}

