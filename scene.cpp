#include "scene.h"

Scene::Scene( void )
{ }

Scene::~Scene( void )
{
    if ( bvh_ )
    {
        delete bvh_;
        bvh_ = nullptr;
    }
}

//void Scene::pushPrimitive( Primitive *primitive )
//{
//    primitives_.push_back( Primitive::PrimitiveUniquePtr( primitive ) );
//}

//void Scene::pushMaterial( const Material *material )
//{
//    materials_.push_back( *material );
//}

int Scene::loadMesh( const std::string &file_name,
                     const Material &material,
                     glm::dvec3 &min,
                     glm::dvec3 &max )
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
        materials_.push_back( material );

        for( unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++ )
        {
            const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];

            for( unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3 )
            {
                const aiVector3D *vertex_ptr = &mesh_ptr->mVertices[vertex_id];

                // Compute the mesh's aabb min and max vertices.
                if ( vertex_id == 0 )
                {
                    min.x = std::min( std::min( static_cast< double >( vertex_ptr[0].x ), static_cast< double >( vertex_ptr[1].x ) ), static_cast< double >( vertex_ptr[2].x ) );
                    min.y = std::min( std::min( static_cast< double >( vertex_ptr[0].y ), static_cast< double >( vertex_ptr[1].y ) ), static_cast< double >( vertex_ptr[2].y ) );
                    min.z = std::min( std::min( static_cast< double >( vertex_ptr[0].z ), static_cast< double >( vertex_ptr[1].z ) ), static_cast< double >( vertex_ptr[2].z ) );

                    max.x = std::max( std::max( static_cast< double >( vertex_ptr[0].x ), static_cast< double >( vertex_ptr[1].x ) ), static_cast< double >( vertex_ptr[2].x ) );
                    max.y = std::max( std::max( static_cast< double >( vertex_ptr[0].y ), static_cast< double >( vertex_ptr[1].y ) ), static_cast< double >( vertex_ptr[2].y ) );
                    max.z = std::max( std::max( static_cast< double >( vertex_ptr[0].z ), static_cast< double >( vertex_ptr[1].z ) ), static_cast< double >( vertex_ptr[2].z ) );
                }
                else
                {
                    min.x = std::min( std::min( std::min( static_cast< double >( vertex_ptr[0].x ), static_cast< double >( vertex_ptr[1].x ) ), static_cast< double >( vertex_ptr[2].x ) ), min.x );
                    min.y = std::min( std::min( std::min( static_cast< double >( vertex_ptr[0].y ), static_cast< double >( vertex_ptr[1].y ) ), static_cast< double >( vertex_ptr[2].y ) ), min.y );
                    min.z = std::min( std::min( std::min( static_cast< double >( vertex_ptr[0].z ), static_cast< double >( vertex_ptr[1].z ) ), static_cast< double >( vertex_ptr[2].z ) ), min.z );

                    max.x = std::max( std::max( std::max( static_cast< double >( vertex_ptr[0].x ), static_cast< double >( vertex_ptr[1].x ) ), static_cast< double >( vertex_ptr[2].x ) ), max.x );
                    max.y = std::max( std::max( std::max( static_cast< double >( vertex_ptr[0].y ), static_cast< double >( vertex_ptr[1].y ) ), static_cast< double >( vertex_ptr[2].y ) ), max.y );
                    max.z = std::max( std::max( std::max( static_cast< double >( vertex_ptr[0].z ), static_cast< double >( vertex_ptr[1].z ) ), static_cast< double >( vertex_ptr[2].z ) ), max.z );
                }

                glm::dvec3 v0{ vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z };
                glm::dvec3 v1{ vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z };
                glm::dvec3 v2{ vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z };

                //primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ v0, v1, v2, &( materials_.back() ) } ) );
                primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ v0, v1, v2, materials_.size() - 1 } ) );
            }
        }
    }

    return EXIT_SUCCESS;
}

void Scene::buildAccelerationStructure( void )
{
    if ( acceleration_structure_ == Scene::AccelerationStructure::BVH_SAH )
    {
        buildBVH();
        std::clog << std::endl;
    }
}

bool Scene::intersect( const Ray &ray,
                       IntersectionRecord &intersection_record,
                       long unsigned int &num_intersection_tests_,
                       long unsigned int &num_intersections_ ) const
{
    bool intersection_result = false;
    IntersectionRecord tmp_intersection_record;
    std::size_t num_primitives = primitives_.size();

    switch( acceleration_structure_ )
    {
    case AccelerationStructure::NONE:
        for ( std::size_t primitive_id = 0; primitive_id < num_primitives; primitive_id++ )
        {
            num_intersection_tests_++;

            if ( primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
            {
                num_intersections_++;

                if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
                {
                    intersection_record = tmp_intersection_record;
                    intersection_result = true;
                }
            }
        }
        break;
    case AccelerationStructure::BVH_SAH:
        intersection_result = bvh_->intersect( ray,
                                               intersection_record,
                                               num_intersection_tests_,
                                               num_intersections_ );
        break;
    }

    return intersection_result;
}

void Scene::printInfoPreAccelerationStructure( void ) const
{
    std::cout << "Scene: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "  # of primitives ..................: " << primitives_.size() << std::endl;
    std::cout << "  # of materials ...................: " << materials_.size() << std::endl;
    std::cout << "  acceleration structure ...........: ";

    if ( acceleration_structure_ == Scene::AccelerationStructure::NONE )
        std::cout << "none";
    else
        if ( acceleration_structure_ == Scene::AccelerationStructure::BVH_SAH )
            std::cout << "BVH-SAH";

    std::cout << std::endl << std::flush;
}

// TODO: print statistics genearted during the creation of the acceleration structure
void Scene::printInfoPostAccelerationStructure( void ) const
{}

void Scene::buildBVH( void )
{
    bvh_ = new BVH( primitives_ );
    //bvh_->dump();
}
