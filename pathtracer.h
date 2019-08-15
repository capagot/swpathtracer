#ifndef PATHTRACER_H
#define PATHTRACER_H

#ifndef DEBUG
#include <omp.h>
#endif

#include "integrator.h"
#include "prng.h"

class PathTracer : public Integrator {
   public:
    enum class PathTerminationCriterion { MAX_DEPTH, RUSSIAN_ROULETTE };

    PathTracer(Camera& camera, const Scene& scene, PathTerminationCriterion path_termination_criterion, int path_length,
               std::unique_ptr<PixelSampler> pixel_sampler,
               PRNG<std::uniform_real_distribution, float, std::mt19937>& prng);
    glm::vec3 traceRay(const Ray& world_ray, int depth, std::size_t& num_intersection_tests,
                       std::size_t& num_intersections);
    void render() override;
    void saveImageToFile() override;
    inline PathTerminationCriterion getPathTerminationCriterion() const {
        return path_termination_criterion_;
    }
    inline int getPathLength() const {
        return path_length_;
    }

   private:
    void printProgress(unsigned int y);

    // When PathTerminationCriterion == MAX_DEPTH, path_length_ defined the maximum path length (or recursion level).
    // When PathTerminationCriterion == RUSSIAN_ROULETTE, path_length_ sets tha minimum path length (or recursion level)
    // before Russian-Roulette starts taking place.
    int path_length_;
    PathTerminationCriterion path_termination_criterion_;
    PRNG<std::uniform_real_distribution, float, std::mt19937>& prng_;
};

#endif  // PATHTRACER_H
