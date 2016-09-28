#include "lua_bind.h"

LuaBind::LuaBind( const std::string &filename )
{
    lua_state_ = luaL_newstate();
    luaL_openlibs( lua_state_ );

    if( luaL_loadfile( lua_state_, filename.c_str() ) )
    {
        std::cerr << "Error loading lua script... exiting...\n";
        exit( EXIT_FAILURE );
    }

    if( lua_pcall( lua_state_, 0, 0, 0 ) )
    {
        std::cerr << "Error executing lua script... exiting...\n";
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
                              glm::vec3 &background_color,
                              std::size_t &max_path_depth,
                              RNG< std::uniform_real_distribution, float, std::mt19937 > &rng )
{
    lua_getglobal( lua_state_, "user variables" );
    getElements( camera,
                 sampler,
                 scene,
                 rendering_buffer,
                 background_color,
                 max_path_depth,
                 rng );
    lua_pop( lua_state_, 1 );
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
                          RNG< std::uniform_real_distribution, float, std::mt19937 > &rng )
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

void LuaBind::getGlobals( glm::vec3 &background_color,
                          std::size_t &max_path_depth )
{
    background_color = parseVec3( "background_color" );
    max_path_depth = parseScalar( "max_path_depth" );
}

void LuaBind::getTriangle( Scene *scene )
{
    glm::vec3 v[3];

    parseVertices( "vertices", v );
    glm::vec3 color = parseVec3( "color" );
    glm::vec3 emission = parseVec3( "emission" );
    scene->materials_.push_back( Material{ Lambertian{ color }, emission } );
    scene->primitives_.push_back( Scene::primitive_ptr( new Triangle{ v[0], v[1], v[2], &(scene->materials_.back()) } ) );
}

void LuaBind::getElements( Camera **camera,
                           Sampler **sampler,
                           Scene *scene,
                           Buffer **rendering_buffer,
                           glm::vec3 &background_color,
                           std::size_t &max_path_depth,
                           RNG< std::uniform_real_distribution, float, std::mt19937 > &rng )
{
    lua_pushnil( lua_state_ );                                   // [0, 1]

    while( lua_next( lua_state_, -2 ) )                          // [-1, 0|2]
    {
        if( lua_istable( lua_state_, -1) )
        {
            lua_pushstring( lua_state_, "object_type" );                // [-1, 0]
            lua_gettable( lua_state_, -2 );                      // [-1, 1]
            if( lua_isstring( lua_state_, -1) )
            {
                std::string object_type = lua_tostring( lua_state_, -1 );

                lua_pop( lua_state_, 1 );                        // [-1, 0]

                if( object_type == "camera" )
                    getCamera( camera );
                if( object_type == "sampler" )
                    getSampler( sampler, rng );
                if( object_type == "buffer" )
                    getBuffer( rendering_buffer );
                if( object_type == "globals" )
                    getGlobals( background_color,
                                max_path_depth );
                if( object_type == "triangle" )
                    getTriangle( scene );
                if( object_type == "mesh" )
                    std::clog << "m\n";
                if( object_type == "union" )
                    getElements( camera,
                                 sampler,
                                 scene,
                                 rendering_buffer,
                                 background_color,
                                 max_path_depth,
                                 rng );

                lua_pushnil( lua_state_ );
            }

            lua_pop( lua_state_, 1 );
        }

        lua_pop( lua_state_, 1 );
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

float LuaBind::parseScalar( const std::string &s )
{
    float v;
    lua_pushstring( lua_state_, s.c_str() );
    lua_gettable( lua_state_, -2 );
    v = lua_tonumber( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
    return v;
}

glm::vec3 LuaBind::parseVec3( const std::string &s )
{
    glm::vec3 v;

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
                             glm::vec3 v[3] )
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
