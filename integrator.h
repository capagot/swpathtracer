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

    enum class PathTerminationCriterion
    {
        MAX_DEPTH,
        RUSSIAN_ROULETTE
    };

    Integrator( Camera &camera,
                const Scene &scene,
                const glm::dvec3 background_color,
                PathTerminationCriterion path_termination_criterion_,
                unsigned int path_length,
                Sampler &sampler,
                Buffer &buffer );

    virtual void integrate( void ) = 0;

    unsigned int getMaxPathDepth( void ) const;

    //TracingStoppingCriterion getTracingStopCriterion( void ) const;

protected:

    const Camera &camera_;

    const Scene &scene_;

    glm::dvec3 background_color_;

    PathTerminationCriterion path_termination_criterion_;

    unsigned int path_length_;

    Sampler &sampler_;

    Buffer &buffer_;
};

#endif /* INTEGRATOR_H_ */

