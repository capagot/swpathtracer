#include "integrator.h"

Integrator::Integrator( Camera &camera,
                        const Scene &scene,
                        const glm::dvec3 background_color,
                        unsigned int max_path_depth,
                        TracingStoppingCriterion tracing_stop_criterion_,
                        Sampler &sampler,
                        Buffer &buffer ) :
        camera_( camera ),
        scene_( scene ),
        background_color_{ background_color },
        max_path_depth_{ max_path_depth },
        tracing_stop_criterion_{ tracing_stop_criterion_ },
        sampler_( sampler ),
        buffer_( buffer )
{ }

