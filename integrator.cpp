#include "integrator.h"

Integrator::Integrator( Camera &camera,
                        const Scene &scene,
                        const glm::dvec3 background_color,
                        PathTerminationCriterion path_termination_criterion,
                        unsigned int path_length,
                        Sampler &sampler,
                        Buffer &buffer ) :
        camera_( camera ),
        scene_( scene ),
        background_color_{ background_color },
        path_termination_criterion_{ path_termination_criterion },
        path_length_{ path_length },
        sampler_( sampler ),
        buffer_( buffer )
{ }

