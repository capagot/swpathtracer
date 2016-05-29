/*
 * renderer.h
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "camera.h"
#include "scene.h"
#include "buffer.h"

class Renderer
{
public:

    typedef enum
    {
        MAX_DEPTH,
        RUSSIAN_ROULETTE
    } TracingStoppingCriterion;

    typedef enum
    {
        UNIFORM_SAMPLING,
        REGULAR_SAMPLING,
        JITTERED_SAMPLING,
        POISSON_SAMPLING
    } PixelSamplingMethod;

    Renderer( const Camera &camera,
              const Scene &scene,
              Buffer &buffer,
              unsigned int spp,
              PixelSamplingMethod pixel_sampling_method,
              unsigned int max_path_depth,
              TracingStoppingCriterion tracing_stop_criterion,
              const Spectrum background_color );

    virtual void render( void ) = 0;    // pure virtual member function. Will be implemented by each renderer.

    unsigned int getSPP( void ) const;

    PixelSamplingMethod getPixelSamplingMethod( void ) const;

    unsigned int getMaxPathDepth( void ) const;

    TracingStoppingCriterion getTracingStopCriterion( void ) const;

    void printInfo( void ) const;

protected:

    const Camera &camera_;                              // camera

    const Scene &scene_;                                // camera

    Buffer &buffer_;                                    // buffer that will store the rendered image

    unsigned int spp_ ;                                 // samples per pixel

    PixelSamplingMethod pixel_sampling_method_;         // pixel sampling method

    unsigned int max_path_depth_;                       // maximum path depth (length)

    TracingStoppingCriterion tracing_stop_criterion_;   // criterion to stop tracing a ray

    Spectrum background_color_;                         // color to be used if a ray misses the scene
};

#endif /* RENDERER_H_ */
