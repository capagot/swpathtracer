/*
 * integrator.h
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

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

    virtual glm::vec3 integrate( void ) = 0;    // pure virtual member function. Will be implemented by each integrator.

    //virtual glm::vec3 integrate_recursive( const Ray &ray, int depth ) = 0;

    unsigned int getMaxPathDepth( void ) const;

    TracingStoppingCriterion getTracingStopCriterion( void ) const;

    void printInfo( void ) const;

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

