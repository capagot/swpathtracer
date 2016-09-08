/*
 * pathtracer.h
 *
 *  Created on: Apr 24, 2016
 *      Author: christian
 */

#ifndef PATHTRACER_H_
#define PATHTRACER_H_

#include <sstream>
#include <iomanip>
#include <cmath>

#include "integrator.h"
#include "random.h"

class PathTracer : private Integrator
{
public:


    PathTracer( Camera &camera,
                const Scene &scene,
                const Spectrum background_color,
                unsigned int max_path_depth,
                TracingStoppingCriterion tracing_stop_criterion_,
                Sampler &sampler,
                Buffer &buffer,
                RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    Spectrum integrate( void );

    void printInfo( void ) const;

private:

    Spectrum integrate_recursive( const Ray &ray, int depth );

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;
   
    // TODO: remove this
    float clamp( float x ) const
    {
        return ( x < 0.0f ) ? 0.0f : ( x > 1.0f ) ? 1.0f : x;
    };

};

#endif /* PATHTRACER_H_ */

