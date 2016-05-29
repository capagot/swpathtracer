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

// TODO: fix this to properly unwind function calls in the case of errors.
//       Actually, this has to be made for the entire program.
int Scene::loadFromFile( const std::string &file_name )
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
        spectrum_.push_back( Spectrum{ glm::vec3{ 1.0f, 0.0f, 0.0f } } ); // Dummy material

        for( unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++ )
        {
            const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];

            for( unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3 )
            {
                const aiVector3D *vertex_ptr = &mesh_ptr->mVertices[vertex_id];

                glm::vec3 a{ vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z };
                glm::vec3 b{ vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z };
                glm::vec3 c{ vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z };

                primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, spectrum_.back() } ) );
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
