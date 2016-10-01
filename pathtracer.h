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
                unsigned int max_path_depth,
                TracingStoppingCriterion tracing_stop_criterion_,
                Sampler &sampler,
                Buffer &buffer,
                RNG< std::uniform_real_distribution, float, std::mt19937 > &rng );

    void integrate( void );

    void printInfoPreRendering( void ) const;

    void printInfoPostRendering( void ) const;

private:

    glm::vec3 integrate_recursive( const Ray &ray,
                                   unsigned int depth,
                                   int thread_id );

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

    std::vector< unsigned long int > num_rays_{};

    std::vector< unsigned long int > num_intersection_tests_{};

    std::vector< unsigned long int > num_intersections_{};

};

#endif /* PATHTRACER_H_ */

