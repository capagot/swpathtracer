#include "swpathtracer.h"

int main( void )
{
    // Create only one instance of the RNG engine per thread. No more instances
    // are created during the execution of the program.
    RNG< std::uniform_real_distribution, float, std::mt19937 > rng{ 0.0f, 1.0f };

    Camera *camera = nullptr;
    Sampler *sampler = nullptr;
    Scene scene{};
    Buffer *rendering_buffer = nullptr;
    glm::vec3 background_color;
    std::size_t max_path_depth;

    // Load and execute the Lua script.
    LuaBind lua( "models/test_model2.lua" );

    // Instantiate the renderer components based on the results of the execution
    // of the Lua script.
    lua.loadFromScript( &camera,
                        &sampler,
                        &scene,
                        &rendering_buffer,
                         background_color,
                         max_path_depth,
                         rng);

    // Set up the renderer.
    PathTracer pt( (*camera),
                   scene,
                   background_color,
                   max_path_depth,
                   Integrator:: MAX_DEPTH,
                   (*sampler),
                   (*rendering_buffer),
                   rng );

    // Renders the final image. 
    pt.integrate();

    // Gamma-compress and save the final image to a .ppm file. 
    rendering_buffer->save( "output.ppm" );

    if ( camera )
        delete camera;

    if ( sampler )
        delete sampler;

    if ( rendering_buffer )
        delete rendering_buffer;

    return EXIT_SUCCESS;
}

