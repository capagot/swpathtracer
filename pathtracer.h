/*
 * pathtracer.h
 *
 *  Created on: Apr 24, 2016
 *      Author: christian
 */

#ifndef PATHTRACER_H_
#define PATHTRACER_H_

#include <limits>

#include "camera.h"
#include "scene.h"
#include "renderer.h"

class PathTracer : private Renderer
{
public:

    PathTracer( const Camera &camera,
                const Scene &scene,
                Buffer &buffer,
                const Spectrum background_color = Spectrum{ glm::vec3{ 1.0f, 0.0f, 0.0f } },
                unsigned int spp = 10,
                PixelSamplingMethod pixel_sampling_method = UNIFORM_SAMPLING,
                unsigned int max_path_depth = 5,
                TracingStoppingCriterion stop_criterion = MAX_DEPTH );

    void render( void );

    void printInfo( void ) const;
};

#endif /* PATHTRACER_H_ */
