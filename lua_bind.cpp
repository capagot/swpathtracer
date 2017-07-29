#include "lua_bind.h"

LuaBind::LuaBind( const std::string &filename )
{
    lua_state_ = luaL_newstate();
    luaL_openlibs( lua_state_ );

    if( luaL_loadfile( lua_state_, filename.c_str() ) )
    {
        std::cerr << "Lua load ERROR: [" <<  lua_state_ << "] " << lua_tostring( lua_state_, -1 ) << std::endl;
        exit( EXIT_FAILURE );
    }

    if( lua_pcall( lua_state_, 0, 0, 0 ) )
    {
        std::cerr << "Lua execution ERROR: [" <<  lua_state_ << "] " << lua_tostring( lua_state_, -1 ) << std::endl;
        exit( EXIT_FAILURE );
    }
}

LuaBind::~LuaBind( void )
{
    lua_close( lua_state_ );
    lua_state_ = nullptr;
}

void LuaBind::loadFromScript( Camera **camera,
                              Sampler **sampler,
                              Scene *scene,
                              Buffer **rendering_buffer,
                              glm::dvec3 &background_color,
                              std::string &path_termination,
                              //std::size_t &max_path_depth,
                              std::size_t &path_length,
                              std::string &output_filename,
                              Scene::AccelerationStructure &scene_acceleration_data_structure,
                              RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    lua_getglobal( lua_state_, "user variables" );
    getElements( camera,
                 sampler,
                 scene,
                 rendering_buffer,
                 background_color,
                 path_termination,
                 //max_path_depth,
                 path_length,
                 output_filename,
                 scene_acceleration_data_structure,
                 rng );
    lua_pop( lua_state_, 1 );

    // TODO: refactor this!!!!
    scene->acceleration_structure_ = scene_acceleration_data_structure;
}

void LuaBind::getCamera( Camera **camera )
{
    if ( parseString( "type" ) == "perspective" )
    {
        PerspectiveCamera *perspective_camera = new PerspectiveCamera();
        perspective_camera->fov_degrees_ = parseScalar( "fov" );
        perspective_camera->aspect_ = parseScalar( "aspect" );
        (*camera) = perspective_camera;
    }
    else
        if ( parseString( "type" ) == "orthographic" )
        {
            OrthographicCamera *orthographic_camera = new OrthographicCamera();
            orthographic_camera->min_x_ = parseScalar( "minx" );
            orthographic_camera->max_x_ = parseScalar( "maxx" );
            orthographic_camera->min_y_ = parseScalar( "miny" );
            orthographic_camera->max_y_ = parseScalar( "maxy" );
            (*camera) = orthographic_camera;
        }

    (*camera)->setPosition( parseVec3( "position" ) );
    (*camera)->setUp( parseVec3( "up" ) );
    (*camera)->setLookAt( parseVec3( "look_at" ) );
}

void LuaBind::getSampler( Sampler **sampler,
                          RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    std::size_t spp = parseScalar( "spp" );

    if ( parseString( "type" ) == "uniform" )
        (*sampler) = new UniformSampler( rng, spp );
    else
        if ( parseString( "type" ) == "regular" )
            (*sampler) = new RegularSampler( spp );
        else
            if ( parseString( "type" ) == "jittered" )
                (*sampler) = new JitteredSampler( rng, spp );
}

void LuaBind::getBuffer( Buffer **rendering_buffer )
{
    std::size_t h_res = parseScalar( "hres" );
    std::size_t v_res = parseScalar( "vres" );
    (*rendering_buffer) = new Buffer( h_res, v_res, 3 );
}

void LuaBind::getGlobals( glm::dvec3 &background_color,
                          std::string &path_termination,
                          //std::size_t &max_path_depth,
                          std::size_t &path_length,
                          std::string &output_filename,
                          Scene::AccelerationStructure &scene_acceleration_data_structure )
{
    background_color = parseVec3( "background_color" );
    path_termination = parseString( "path_termination" );
    path_length = parseScalar( "path_length" );
    output_filename = parseString( "output_filename" );
    std::string acelleration_structure = parseString( "acceleration_data_structure" );

    scene_acceleration_data_structure = Scene::AccelerationStructure::NONE;
    if ( acelleration_structure == "bvh-sah" )
        scene_acceleration_data_structure = Scene::AccelerationStructure::BVH_SAH;
}

void LuaBind::getMaterial( Scene *scene,
                           RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler_object;
    Fresnel::FresnelUniquePtr fresnel_object;
    BxDF::BxDFUniquePtr bxdf_object;
    BSDF::BSDFUniquePtr bsdf_object = BSDF::BSDFUniquePtr{ new BSDF{ rng } };
    Material::MaterialUniquePtr material_object;

    lua_pushstring( lua_state_, "material" );
    lua_gettable( lua_state_, -2 );

    if( lua_istable( lua_state_, -1) )
    {
        int current_layer = 1;

        lua_pushnumber( lua_state_, current_layer );
        lua_gettable( lua_state_, -2 );

        while ( lua_istable( lua_state_, -1 ) ) // loops over all BSDF layers
        {
            lua_pushstring( lua_state_, "object_type" );
            lua_gettable( lua_state_, -2 );
            if( lua_isstring( lua_state_, -1) )
            {
                std::string object_type = lua_tostring( lua_state_, -1 );

                lua_pop( lua_state_, 1 );

                if ( object_type == "lambertian_brdf" )
                {
                    glm::dvec3 kd = parseVec3( "kd" );
                    std::string surface_sampler = parseString( "surface_sampler" );
                    if ( surface_sampler == "uniform" )
                        surface_sampler_object = SurfaceSampler::SurfaceSamplerUniquePtr{ new SurfaceSamplerUniform( rng ) };
                    if ( surface_sampler == "importance" )
                        surface_sampler_object = SurfaceSampler::SurfaceSamplerUniquePtr{ new SurfaceSamplerCosine( rng ) };
                    fresnel_object = Fresnel::FresnelUniquePtr{ new FresnelNoOp{} };
                    bxdf_object = BxDF::BxDFUniquePtr{ new Lambertian{ kd,
                                                                       std::move( surface_sampler_object ),
                                                                       std::move( fresnel_object ) } };
                }
                else
                    if ( object_type == "smooth_dielectric" )
                    {
                        double eta = parseScalar( "eta" );
                        surface_sampler_object = SurfaceSampler::SurfaceSamplerUniquePtr{ new SurfaceSamplerSmoothRefraction{ rng, 1.0, eta } };
                        fresnel_object = Fresnel::FresnelUniquePtr{ new FresnelSchlick{ 1.0, eta } };
                        bxdf_object = BxDF::BxDFUniquePtr{ new SmoothDielectric{ std::move( surface_sampler_object ),
                                                                                 std::move( fresnel_object ) } };
                    }
                    else
                        if ( object_type == "smooth_specular_reflection" )
                        {
                            surface_sampler_object = SurfaceSampler::SurfaceSamplerUniquePtr{ new SurfaceSamplerSmoothConductor{} };
                            glm::dvec3 reflectance_at_normal_incidence = parseVec3( "reflectance_at_normal_incidence" );
                            fresnel_object = Fresnel::FresnelUniquePtr{ new FresnelSchlick{ reflectance_at_normal_incidence } };
                            bxdf_object = BxDF::BxDFUniquePtr{ new SmoothConductor{ std::move( surface_sampler_object ),
                                                                                    std::move( fresnel_object ) } };
                        }
                        else
                            if ( object_type == "cook_torrance_brdf" )
                            {
                                double m = parseScalar( "m" );
                                glm::dvec3 reflectance_at_normal_incidence = parseVec3( "reflectance_at_normal_incidence" );
                                fresnel_object = Fresnel::FresnelUniquePtr{ new FresnelSchlick{ reflectance_at_normal_incidence } };
                                std::string surface_sampler = parseString( "surface_sampler" );
                                if ( surface_sampler == "uniform" )
                                    surface_sampler_object = SurfaceSampler::SurfaceSamplerUniquePtr{ new SurfaceSamplerUniform( rng ) };
                                if ( surface_sampler == "importance" )
                                    surface_sampler_object = SurfaceSampler::SurfaceSamplerUniquePtr{ new SurfaceSamplerCookTorrance( rng, m ) };
                                bxdf_object = BxDF::BxDFUniquePtr{ new CookTorrance{ m,
                                                                                     std::move( surface_sampler_object ),
                                                                                     std::move( fresnel_object ) } };
                            }

                bsdf_object->bxdf_layers_.push_back( std::move( bxdf_object ) );
            }

            lua_pop( lua_state_, 1 );

            lua_pushnumber( lua_state_, ++current_layer );
            lua_gettable( lua_state_, -2 );
        }

        lua_pop( lua_state_, 1 );

        glm::dvec3 emission = parseVec3( "emission" );

        Material::MaterialUniquePtr material_object = Material::MaterialUniquePtr{ new Material{ std::move( bsdf_object ),
            emission } };
        scene->materials_.push_back( std::move( material_object ) );
    }

    lua_pop( lua_state_, 1 );
}

void LuaBind::getTriangle( Scene *scene,
                           RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    glm::dvec3 v[3];

    parseVertices( "vertices", v );
    getMaterial( scene, rng );
    scene->primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ v[0], v[1], v[2], scene->materials_.size() - 1 } ) );
}

void LuaBind::getSphere( Scene *scene,
                         RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    glm::dvec3 center = parseVec3( "center" );
    double radius = parseScalar( "radius" );
    getMaterial( scene, rng );
    scene->primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ center, radius, scene->materials_.size() - 1 } ) );
}

void LuaBind::getMesh( Scene *scene,
                       const Scene::MeshType mesh_type,
                       RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    std::string filename = parseString( "filename" );
    glm::dvec3 min_aabb;
    glm::dvec3 max_aabb;
    getMaterial( scene, rng );
    scene->loadMesh( filename,
                     mesh_type,
                     min_aabb,
                     max_aabb );
}

void LuaBind::getElements( Camera **camera,
                           Sampler **sampler,
                           Scene *scene,
                           Buffer **rendering_buffer,
                           glm::dvec3 &background_color,
                           std::string &path_termination,
                           //std::size_t &max_path_depth,
                           std::size_t &path_length,
                           std::string &output_filename,
                           Scene::AccelerationStructure &scene_acceleration_data_structure,
                           RNG< std::uniform_real_distribution, double, std::mt19937 > &rng )
{
    lua_pushnil( lua_state_ );                                   // [pop 0, push 1]

    while( lua_next( lua_state_, -2 ) )                          // [pop 1, push 0|2]
    {
        if( lua_istable( lua_state_, -1) )
        {
            lua_pushstring( lua_state_, "object_type" );         // [pop 1, push 0]
            lua_gettable( lua_state_, -2 );                      // [pop 1, push 1]
            if( lua_isstring( lua_state_, -1) )
            {
                std::string object_type = lua_tostring( lua_state_, -1 );

                lua_pop( lua_state_, 1 );                        // [pop 1, push 0]

                if( object_type == "camera" )
                    getCamera( camera );
                if( object_type == "sampler" )
                    getSampler( sampler, rng );
                if( object_type == "buffer" )
                    getBuffer( rendering_buffer );
                if( object_type == "globals" )
                    getGlobals( background_color,
                                path_termination,
                                //max_path_depth,
                                path_length,
                                output_filename,
                                scene_acceleration_data_structure );
                if( object_type == "triangle" )
                    getTriangle( scene, rng);
                if( object_type == "sphere" )
                    getSphere( scene, rng );
                if( object_type == "mesh" )
                    getMesh( scene, Scene::MeshType::FLAT, rng );
                if( object_type == "smooth_mesh" )
                    getMesh( scene, Scene::MeshType::SMOOTH, rng );
                if( object_type == "union" )
                    getElements( camera,
                                 sampler,
                                 scene,
                                 rendering_buffer,
                                 background_color,
                                 path_termination,
                                 //max_path_depth,
                                 path_length,
                                 output_filename,
                                 scene_acceleration_data_structure,
                                 rng );

                lua_pushnil( lua_state_ );                      // [pop 0, push 1]
            }

            lua_pop( lua_state_, 1 );                           // [pop 1, push 0]
        }

        lua_pop( lua_state_, 1 );                               // [pop 1, push 0]
    }
}

std::string LuaBind::parseString( const std::string &s_input )
{
    std::string s_output;
    lua_pushstring( lua_state_, s_input.c_str() );
    lua_gettable( lua_state_, -2 );
    s_output = lua_tostring( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
    return s_output;
}

double LuaBind::parseScalar( const std::string &s )
{
    double v;
    lua_pushstring( lua_state_, s.c_str() );
    lua_gettable( lua_state_, -2 );
    v = lua_tonumber( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
    return v;
}

glm::dvec3 LuaBind::parseVec3( const std::string &s )
{
    glm::dvec3 v;

    lua_pushstring( lua_state_, s.c_str() );
    lua_gettable( lua_state_, -2 );
    lua_pushnil( lua_state_ );

    for ( int i = 0; i < 3; i++ )
    {
        lua_next( lua_state_, -2 );
        v[i] = lua_tonumber( lua_state_, -1 );
        lua_pop( lua_state_, 1 );
    }

    lua_pop( lua_state_, 2 );
    return v;
}

void LuaBind::parseVertices( const std::string &s,
                             glm::dvec3 v[3] )
{
    lua_pushstring( lua_state_, s.c_str() );
    lua_gettable( lua_state_, -2 );
    lua_pushnil( lua_state_ );

    for ( int i = 0; i < 3; i++ )
    {
        lua_next( lua_state_, -2 );
        lua_pushnil( lua_state_ );

        for ( int j = 0; j < 3; j++ )
        {
            lua_next( lua_state_, -2 );
            v[i][j] = lua_tonumber( lua_state_, -1 );
            lua_pop( lua_state_, 1 );
        }

        lua_pop( lua_state_, 2 );
    }

    lua_pop( lua_state_, 2 );
}
