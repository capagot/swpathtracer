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
                const glm::vec3 background_color,
                PathTerminationCriterion PathTermination_criterion_,
                unsigned int path_length,
                Sampler &sampler,
                Buffer &buffer,
                RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    void integrate( void );

    void printInfoPreRendering( void ) const;

    void printInfoPostRendering( void ) const;

private:

    glm::vec3 integrate_recursive( const Ray &ray,
                                   unsigned int depth,
                                   float see_emitted_light,
                                   int thread_id );

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

    std::vector< long unsigned int > num_rays_;

    std::vector< long unsigned int > num_intersection_tests_;

    std::vector< long unsigned int > num_intersections_;

};

#endif /* PATHTRACER_H_ */

