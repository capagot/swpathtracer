#include "swpathtracer.h"

int main( int num_args, char **arg_vector )
{
    /*
    feenableexcept( //FE_INVALID   | 
                    //FE_DIVBYZERO | 
                    FE_OVERFLOW  | 
                    FE_UNDERFLOW );
    //*/
    //
#ifdef DEBUG
    omp_set_dynamic( 0 );     // Explicitly disable dynamic teams
    omp_set_num_threads( 1 );
#endif

    // Create only one instance of the RNG engine per thread. No more instances
    // are created during the execution of the program.
    RNG< std::uniform_real_distribution, float, std::mt19937 > rng{ 0.0f, 1.0f };

    Camera *camera = nullptr;
    Sampler *sampler = nullptr;
    Scene scene{};
    Buffer *rendering_buffer = nullptr;
    glm::vec3 background_color;
    std::string path_termination;
    //std::size_t max_path_depth;
    std::size_t path_length;
    std::string output_filename;
    Scene::AccelerationStructure scene_acceleration_data_structure;

    CmdLineParser cmdlineparser( num_args, arg_vector );

    LuaBind lua( cmdlineparser.getInputScriptFilename() );

    // Instantiate the renderer components based on the results of the execution
    // of the Lua script.
    lua.loadFromScript( &camera,
                        &sampler,
                        &scene,
                        &rendering_buffer,
                         background_color,
                         path_termination,
                         //max_path_depth,
                         path_length,
                         output_filename,
                         scene_acceleration_data_structure,
                         rng);

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
    std::cout << "  path termination criterion .......: " << (( path_termination == "russian-roulette" ) ? "russian roulette" : "fixed length" ) << std::endl; 

    if (  path_termination == "russian-roulette" )
        std::cout << "  path minimum length ..............: ";
    else
        std::cout << "  path maximum length ..............: ";
    std::cout << path_length << std::endl;

    std::cout << "  output file name .................: " << output_filename << std::endl;

    std::cout << std::endl;

    scene.printInfoPreAccelerationStructure();
    scene.buildAccelerationStructure();
    scene.printInfoPostAccelerationStructure();

    std::cout << std::endl;
    rendering_buffer->printInfo();

    // Set up the renderer.
    /*
    PathTracer pt( (*camera),
                   scene,
                   background_color,
                   ( ( path_termination == "russian-roulette" ) ? Integrator::PathTerminationCriterion::RUSSIAN_ROULETTE : Integrator::PathTerminationCriterion::MAX_DEPTH ),
                   path_length,
                   (*sampler),
                   (*rendering_buffer),
                   rng );
    //*/

    
    RayCaster pt( (*camera),
                   scene,
                   background_color,
                   ( ( path_termination == "russian-roulette" ) ? Integrator::PathTerminationCriterion::RUSSIAN_ROULETTE : Integrator::PathTerminationCriterion::MAX_DEPTH ),
                   path_length,
                   (*sampler),
                   (*rendering_buffer),
                   rng );
    //*/

    std::cout << std::endl;
    std::cout << "Rendering statistics: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    pt.printInfoPreRendering();
    pt.integrate();                 // Renders the final image.
    pt.printInfoPostRendering();

    // Gamma-compress and save the final image to a .ppm file.
    rendering_buffer->save( output_filename );

    //*/

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
