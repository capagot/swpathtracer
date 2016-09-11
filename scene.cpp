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
    float s = 2.0f;
   
    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.75f, 0.75f, 0.75f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );

    // back wall
    glm::vec3 a{-s, -s, -s };
    glm::vec3 b{ s, -s, -s };
    glm::vec3 c{ s,  s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ a, b, c, materials_.back() } ) );

    glm::vec3 f{ s,  s, -s };
    glm::vec3 d{-s,  s, -s };
    glm::vec3 e{-s, -s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ d, e, f, materials_.back() } ) );

    // top
    glm::vec3 g{ s, s, -s };
    glm::vec3 h{-s, s, -s };
    glm::vec3 i{ s, s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ g, i, h, materials_.back() } ) );

    glm::vec3 j{-s, s, -s };
    glm::vec3 k{ s, s,  s };
    glm::vec3 l{-s, s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ j, k, l, materials_.back() } ) );

    // bottom
    glm::vec3 g1{ s, -s, -s };
    glm::vec3 h1{-s, -s, -s };
    glm::vec3 i1{ s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ g1, h1, i1, materials_.back() } ) );

    glm::vec3 j1{-s, -s, -s };
    glm::vec3 k1{ s, -s,  s };
    glm::vec3 l1{-s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ j1, l1, k1, materials_.back() } ) );

    // left wall
    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.75f, 0.25f, 0.25f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );
    glm::vec3 m{-s,  s, -s };
    glm::vec3 n{-s,  s,  s };
    glm::vec3 o{-s, -s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ m, n, o, materials_.back() } ) );

    glm::vec3 p{-s, -s, -s };
    glm::vec3 q{-s,  s,  s };
    glm::vec3 r{-s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ p, q, r, materials_.back() } ) );

    // right wall
    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.25f, 0.25f, 0.75f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } );
    glm::vec3 m1{ s,  s, -s };
    glm::vec3 n1{ s,  s,  s };
    glm::vec3 o1{ s, -s, -s };
    primitives_.push_back( primitive_ptr( new Triangle{ m1, o1, n1, materials_.back() } ) );

    glm::vec3 p1{ s, -s, -s };
    glm::vec3 q1{ s,  s,  s };
    glm::vec3 r1{ s, -s,  s };
    primitives_.push_back( primitive_ptr( new Triangle{ p1, r1, q1, materials_.back() } ) );

    // area light
    float ls = 0.5f;
    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } }, Spectrum{ glm::vec3{ 17.0f, 17.0f, 17.0f } } } );
    glm::vec3 t{ ls, s - 0.001f, -ls };
    glm::vec3 u{-ls, s - 0.001f, -ls };
    glm::vec3 v{ ls, s - 0.001f,  ls };
    primitives_.push_back( primitive_ptr( new Triangle{ t, v, u, materials_.back() } ) );

    glm::vec3 t1{-ls, s - 0.001f, -ls };
    glm::vec3 u1{ ls, s - 0.001f,  ls };
    glm::vec3 v1{-ls, s - 0.001f,  ls };
    primitives_.push_back( primitive_ptr( new Triangle{ t1, u1, v1, materials_.back() } ) );

    /*
    s = 0.75f;

    materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.0f, 0.75f, 0.0f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } ); // Dummy material

    // front    
    glm::vec3 au{-s, -s+.5, -0.50f };
    glm::vec3 bu{ s, -s+.5, -0.50f };
    glm::vec3 cu{ s,  s+.5, -0.50f };
    primitives_.push_back( primitive_ptr( new Triangle{ au, bu, cu, materials_.back() } ) );

    glm::vec3 fu{ s,  s+.5, -0.50 };
    glm::vec3 du{-s,  s+.5, -0.50 };
    glm::vec3 eu{-s, -s+.5, -0.50 };
    primitives_.push_back( primitive_ptr( new Triangle{ du, eu, fu, materials_.back() } ) );
    
    // back
    glm::vec3 a1u{-s, -s+.5, -1.5f };
    glm::vec3 b1u{ s, -s+.5, -1.5f };
    glm::vec3 c1u{ s,  s+.5, -1.5f };
    primitives_.push_back( primitive_ptr( new Triangle{ a1u, c1u, b1u, materials_.back() } ) );

    glm::vec3 f1u{ s,  s+.5, -1.5 };
    glm::vec3 d1u{-s,  s+.5, -1.5 };
    glm::vec3 e1u{-s, -s+.5, -1.5 };
    primitives_.push_back( primitive_ptr( new Triangle{ d1u, f1u, e1u, materials_.back() } ) );
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
        materials_.push_back( Material{ Spectrum{ glm::vec3{ 0.75f, 0.75f, 0.75f } }, Spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } } } ); // Dummy material

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
