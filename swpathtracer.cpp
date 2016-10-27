#include "swpathtracer.h"

int main( int num_args, char **arg_vector )
{
    // DEBUG
    //omp_set_dynamic( 0 );     // Explicitly disable dynamic teams
    //omp_set_num_threads( 1 );

    // Create only one instance of the RNG engine per thread. No more instances
    // are created during the execution of the program.
    RNG< std::uniform_real_distribution, double, std::mt19937 > rng{ 0.0, 1.0 };

    Camera *camera = nullptr;
    Sampler *sampler = nullptr;
    Scene scene{};
    Buffer *rendering_buffer = nullptr;
    glm::dvec3 background_color;
    std::size_t max_path_depth;
    std::string output_filename;

    CmdLineParser cmdlineparser( num_args, arg_vector );

    LuaBind lua( cmdlineparser.getInputScriptFilename() );

    // Instantiate the renderer components based on the results of the execution
    // of the Lua script.
    lua.loadFromScript( &camera,
                        &sampler,
                        &scene,
                        &rendering_buffer,
                         background_color,
                         max_path_depth,
                         output_filename,
                         rng);


    scene.buildBVH();

    std::cout << "Input: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "  file name ........................: " << cmdlineparser.getInputScriptFilename() << std::endl;

    std::cout << std::endl;
    camera->printInfo();

    std::cout << std::endl;
    sampler->printInfo();

    std::cout << std::endl;
    std::cout << "Globals: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "  background color .................: [" << background_color[0] << ", "
                                                           << background_color[1] << ", "
                                                           << background_color[2] << "]" << std::endl;
    std::cout << "  path tracing term. criterion .....: maximum depth" << std::endl;
    std::cout << "  max. path depth ..................: " << max_path_depth << std::endl;
    std::cout << "  output file name .................: " << output_filename << std::endl;

    std::cout << std::endl;
    scene.printInfo();

    //exit(0);

    std::cout << std::endl;
    rendering_buffer->printInfo();

    // Set up the renderer.
    PathTracer pt( (*camera),
                   scene,
                   background_color,
                   max_path_depth,
                   Integrator:: MAX_DEPTH,
                   (*sampler),
                   (*rendering_buffer),
                   rng );

    std::cout << std::endl;
    std::cout << "Rendering statistics: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    pt.printInfoPreRendering();

    // Renders the final image.
    pt.integrate();

    pt.printInfoPostRendering();

    // Gamma-compress and save the final image to a .ppm file.
    rendering_buffer->save( output_filename );

    // This is ugly!
    if ( camera )
    {
        delete camera;
        camera = nullptr;
    }

    // This is ugly!
    if ( sampler )
    {
        delete sampler;
        sampler = nullptr;
    }

    // This is ugly!
    if ( rendering_buffer )
    {
        delete rendering_buffer;
        rendering_buffer = nullptr;
    }

    return EXIT_SUCCESS;
}
