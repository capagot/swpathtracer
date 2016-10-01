#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "ray.h"
#include "scene.h"
#include "camera.h"
#include "sampler.h"
#include "buffer.h"

class Integrator
{
public:

    typedef enum
    {
        MAX_DEPTH,
        RUSSIAN_ROULETTE
    } TracingStoppingCriterion;

    Integrator( Camera &camera,
                const Scene &scene,
                const glm::vec3 background_color,
                unsigned int max_path_depth,
                TracingStoppingCriterion tracing_stop_criterion_,
                Sampler &sampler,
                Buffer &buffer );

    virtual void integrate( void ) = 0;

    unsigned int getMaxPathDepth( void ) const;

    TracingStoppingCriterion getTracingStopCriterion( void ) const;

protected:
    const Camera &camera_;

    const Scene &scene_;

    glm::vec3 background_color_;

    unsigned int max_path_depth_;

    TracingStoppingCriterion tracing_stop_criterion_;

    Sampler &sampler_;

    Buffer &buffer_;
};

#endif /* INTEGRATOR_H_ */
