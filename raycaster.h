#ifndef RAYCASTER_H_
#define RAYCASTER_H_

#include <sstream>
#include <iomanip>
#include <cmath>

#include "integrator.h"
#include "random.h"

class RayCaster : private Integrator
{
public:

    RayCaster( Camera &camera,
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

    float Hue2RGB( float v1, float v2, float h );

    glm::vec3 integrate_recursive( const Ray &ray,
                                   unsigned int depth,
                                   int thread_id );

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

    std::vector< long unsigned int > num_rays_;

    std::vector< long unsigned int > num_intersection_tests_;

    std::vector< long unsigned int > num_intersections_;

};

#endif /* RAYCASTER_H_ */

