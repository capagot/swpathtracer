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
    /*
    materials_.push_back( Material{ Spectrum{ glm::vec3{ 1.0f, 1.0f, 1.0f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );
    glm::vec3 a{ -0.5, -0.5, -1.0 };
    glm::vec3 b{  0.5, -0.5, -1.0 };
    glm::vec3 c{  0.0,  0.5, -1.0 };
    primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, materials_.back() } ) );
    //*/

    float s = 2.0f;

    materials_.push_back( Material{ Spectrum{ glm::vec3{ 1.0f, 1.0f, 1.0f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );
    glm::vec3 a{-s, -s, -s };
    glm::vec3 b{ s, -s, -s };
    glm::vec3 c{ s,  s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, materials_.back() } ) );

    glm::vec3 f{ s,  s, -s };
    glm::vec3 d{-s,  s, -s };
    glm::vec3 e{-s, -s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ d, e, f, materials_.back() } ) );

    glm::vec3 g{ s, s, -s };
    glm::vec3 h{-s, s, -s };
    glm::vec3 i{ s, s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ g, h, i, materials_.back() } ) );

    glm::vec3 j{-s, s, -s };
    glm::vec3 k{ s, s,  s };
    glm::vec3 l{-s, s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ j, k, l, materials_.back() } ) );

    glm::vec3 g1{ s, -s, -s };
    glm::vec3 h1{-s, -s, -s };
    glm::vec3 i1{ s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ g1, h1, i1, materials_.back() } ) );

    glm::vec3 j1{-s, -s, -s };
    glm::vec3 k1{ s, -s,  s };
    glm::vec3 l1{-s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ j1, k1, l1, materials_.back() } ) );

    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.0f, 1.0f, 0.0f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );
    glm::vec3 m{-s,  s, -s };
    glm::vec3 n{-s,  s,  s };
    glm::vec3 o{-s, -s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ m, n, o, materials_.back() } ) );

    glm::vec3 p{-s, -s, -s };
    glm::vec3 q{-s,  s,  s };
    glm::vec3 r{-s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ p, q, r, materials_.back() } ) );

    materials_.push_back( Material{ Spectrum{ glm::vec3{ 1.0f, 0.0f, 0.0f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );
    glm::vec3 m1{ s,  s, -s };
    glm::vec3 n1{ s,  s,  s };
    glm::vec3 o1{ s, -s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ m1, n1, o1, materials_.back() } ) );

    glm::vec3 p1{ s, -s, -s };
    glm::vec3 q1{ s,  s,  s };
    glm::vec3 r1{ s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ p1, q1, r1, materials_.back() } ) );

    float ls = 0.5f;
    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } }, Spectrum{ glm::vec3{ 1.0f, 1.0f, 1.0f } } } );
    glm::vec3 t{ ls, s - 0.001f, -ls };
    glm::vec3 u{-ls, s - 0.001f, -ls };
    glm::vec3 v{ ls, s - 0.001f,  ls };
    primitives_.push_back( primitive_ptr( new Triangle{ t, u, v, materials_.back() } ) );

    glm::vec3 t1{-ls, s - 0.001f, -ls };
    glm::vec3 u1{ ls, s - 0.001f,  ls };
    glm::vec3 v1{-ls, s - 0.001f,  ls };
    primitives_.push_back( primitive_ptr( new Triangle{ t1, u1, v1, materials_.back() } ) );
    //*/
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
        materials_.push_back( Material{ Spectrum{ glm::vec3{ 1.0f, 0.0f, 0.0f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } ); // Dummy material

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

                glm::vec3 a{ vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z };
                glm::vec3 b{ vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z };
                glm::vec3 c{ vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z };

                primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, materials_.back() } ) );
            }
        }
    }

    load();

    return EXIT_SUCCESS;
}

void Scene::printInfo( void )
{
    std::clog << "Scene Information" << std::endl;
    std::clog << "-----------------" << std::endl;
    std::clog << " number of primitives ..........: " << primitives_.size() << std::endl;
}
