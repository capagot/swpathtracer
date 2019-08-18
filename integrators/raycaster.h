#ifndef INTEGRATORS_RAYCASTER_H
#define INTEGRATORS_RAYCASTER_H

#ifndef DEBUG
#include <omp.h>
#endif

#include "integrator.h"
#include "regular_pixel_sampler.h"
#include "timer.h"

/*
 * The RayCaster class template implements a common interface for all ray casters. It assumes that all ray
 * casters shoot only one ray per pixel, through their centers, and that each ray will store some
 * data to its corresponding position on a target buffer.
 *
 * Since all ray casters shoot rays through the centers of the corresponding pixels, the class template
 * instantiate a RegularPixelSampler, which is set up to use just one sample.
 *
 * Ray casters may differ, however, with respect to the type of data that may be returned by the rays.
 * For instance, depending on the ray caster, the ray may return the distance to, or the normal vector
 * at, the closest hit point. The template parameter refers exactly to the type of data that is to be returned by
 * the rays, and will be used, among other things, to define the type the elements of the target buffer.
 */

template <class T>
class RayCaster : public Integrator {
   public:
    RayCaster(Camera& camera, const Scene& scene, Integrator::Type integrator_type);
    virtual ~RayCaster() = 0;
    T traceRay(const Ray& ray, std::size_t& num_intersection_tests, std::size_t& num_intersections);
    void render();
    virtual void saveImageToFile() = 0;

   protected:
    virtual T getHitValue(const IntersectionRecord& intersection_record, std::size_t num_intersection_tests,
                          std::size_t num_intersections) const = 0;
    virtual T getMissValue(std::size_t num_intersection_tests) const = 0;

    std::vector<std::vector<T>> buffer_;
};

template <class T>
RayCaster<T>::RayCaster(Camera& camera, const Scene& scene, Integrator::Type integrator_type)
    : Integrator(camera, scene, integrator_type, std::make_unique<RegularPixelSampler>(1)),
      buffer_(camera_.getImage().getImageWidth(), std::vector<T>(camera_.getImage().getImageHeight())) {}

template <class T>
RayCaster<T>::~RayCaster() {}

template <class T>
T RayCaster<T>::traceRay(const Ray& ray, std::size_t& num_intersection_tests, std::size_t& num_intersections) {
    IntersectionRecord intersection_record;

    if (scene_.intersect(ray, intersection_record, num_intersection_tests, num_intersections))
        return getHitValue(intersection_record, num_intersection_tests, num_intersections);
    else
        return getMissValue(num_intersection_tests);
}

template <class T>
void RayCaster<T>::render() {
    Timer timer;
    int_tests_count_ = 0;
    int_count_ = 0;
    min_int_tests_count_pp_ = std::numeric_limits<std::size_t>::max();
    max_int_tests_count_pp_ = 0;
    min_int_count_pp_ = std::numeric_limits<std::size_t>::max();
    max_int_count_pp_ = 0;
    timer.start();

#ifndef DEBUG
#pragma omp parallel for schedule(dynamic, 1) reduction(+ : int_tests_count_, int_count_) reduction(min: min_int_tests_count_pp_, min_int_count_pp_) reduction(max: max_int_tests_count_pp_, max_int_count_pp_)
#endif

    for (unsigned int y = 0; y < camera_.getImage().getImageHeight(); ++y)
        for (unsigned int x = 0; x < camera_.getImage().getImageWidth(); ++x) {
            std::size_t curr_int_tests_count;
            std::size_t curr_int_count;

            float x_screen = x + pixel_sampler_->getSample(0).x;
            float y_screen = y + pixel_sampler_->getSample(0).y;

            Ray ray = camera_.getRay(x_screen, y_screen);
            buffer_[x][y] = traceRay(ray, curr_int_tests_count, curr_int_count);

            // update integration statistics...
            int_tests_count_ += curr_int_tests_count;
            int_count_ += curr_int_count;
            min_int_tests_count_pp_ = std::min(min_int_tests_count_pp_, curr_int_tests_count);
            max_int_tests_count_pp_ = std::max(max_int_tests_count_pp_, curr_int_tests_count);
            min_int_count_pp_ = std::min(min_int_count_pp_, curr_int_count);
            max_int_count_pp_ = std::max(max_int_count_pp_, curr_int_count);
        }

    timer.finish();
    total_integration_time_ = timer.getElapsedTime();
    std::cout << "====> Total rendering time: " << total_integration_time_ << " microseconds\n";
}

#endif  // INTEGRATORS_RAYCASTER_H
