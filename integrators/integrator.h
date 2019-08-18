#ifndef INTEGRATORS_INTEGRATOR_H
#define INTEGRATORS_INTEGRATOR_H

#include "camera.h"
#include "pixel_sampler.h"
#include "scene.h"

class Integrator {
   public:
    enum class Type { NONE, NORMAL_RAYCASTER, DEPTH_RAYCASTER, INTERSECTION_TEST_COUNT_RAYCASTER, PATHTRACER };

    Integrator(Camera& camera, const Scene& scene, Type type, std::unique_ptr<PixelSampler> pixel_sampler);
    virtual ~Integrator() = 0;
    inline Type getType() const {
        return type_;
    }
    inline const PixelSampler& getPixelSampler() const {
        return *pixel_sampler_;
    }
    inline std::size_t getNumIntersectionTests() const {
        return int_tests_count_;
    }
    inline std::size_t getNumIntersections() const {
        return int_count_;
    }
    inline std::size_t getMinIntersectionTestsPerPixel() const {
        return min_int_tests_count_pp_;
    }
    inline std::size_t getMaxIntersectionTestsPerPixel() const {
        return max_int_tests_count_pp_;
    }
    inline std::size_t getMinIntersectionsPerPixel() const {
        return min_int_count_pp_;
    }
    inline std::size_t getMaxIntersectionsPerPixel() const {
        return max_int_count_pp_;
    }
    virtual void render() = 0;
    virtual void saveImageToFile() = 0;

   protected:
    Type type_;
    Camera& camera_;
    const Scene& scene_;
    std::unique_ptr<PixelSampler> pixel_sampler_;

    // some integration statistics...
    long int total_integration_time_;     //  in microseconds
    std::size_t int_tests_count_;         // total number of intersection tests for the image
    std::size_t int_count_;               // total number of actual intersections for the image
    std::size_t min_int_tests_count_pp_;  // minimum number of intersection tests per pixel
    std::size_t max_int_tests_count_pp_;  // maximum number of intersection tests per pixel
    std::size_t min_int_count_pp_;        // minimum number of actual intersections per pixel
    std::size_t max_int_count_pp_;        // maximum number of actual intersections per pixel
};

#endif  // INTEGRATORS_INTEGRATOR_H
