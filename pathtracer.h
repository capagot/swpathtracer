/*
 * pathtracer.h
 *
 *  Created on: Apr 24, 2016
 *      Author: christian
 */

#ifndef PATHTRACER_H_
#define PATHTRACER_H_

#include "integrator.h"

class PathTracer : private Integrator
{
public:


    PathTracer( Camera &camera,
                const Scene &scene,
                const Spectrum background_color,
                unsigned int max_path_depth,
                TracingStoppingCriterion tracing_stop_criterion_,
                Sampler &sampler,
                Buffer &buffer );

    Spectrum integrate( void );

    void printInfo( void ) const;

private:

    Spectrum integrate_recursive( const Ray &ray, int depth );

};

#endif /* PATHTRACER_H_ */
