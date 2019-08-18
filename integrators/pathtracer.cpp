#include "pathtracer.h"
#include "timer.h"

PathTracer::PathTracer(Camera& camera, const Scene& scene, PathTerminationCriterion path_termination_criterion,
                       int path_length, std::unique_ptr<PixelSampler> pixel_sampler,
                       PRNG<std::uniform_real_distribution, float, std::mt19937>& prng)
    : Integrator(camera, scene, Integrator::Type::PATHTRACER, std::move(pixel_sampler)),
      path_length_(path_length),
      path_termination_criterion_(path_termination_criterion),
      prng_(prng) {}

glm::vec3 PathTracer::traceRay(const Ray& world_ray, int depth, std::size_t& num_intersection_tests,
                               std::size_t& num_intersections) {
    IntersectionRecord intersection_record;
    intersection_record.t_ = std::numeric_limits<float>::infinity();
    glm::vec3 radiance(0.0f);

    if ((path_termination_criterion_ == PathTerminationCriterion::MAX_DEPTH) && (depth >= path_length_))
        return glm::vec3(0.0f);  // returns zero radiance

    if (!scene_.intersect(world_ray, intersection_record, num_intersection_tests, num_intersections)) {
        if (!depth)  // If it is a primary ray (depth == 0), returns the background color
            return scene_.getBackgroundColor();
        else  // If it is not a primary ray, returns zero radiance
            return glm::vec3(0.0f);
    }

    ONB local_frame(intersection_record.normal_);

    glm::vec3 local_wo = glm::normalize(local_frame.getWorldToLocalMatrix() * -world_ray.getDirection());
    glm::vec3 local_wi;
    float pdf;
    glm::vec3 bsdf;
    glm::vec3 reflectance;

    scene_.getMaterial(intersection_record.material_id_).evalBSDF(local_wo, bsdf, reflectance, pdf, local_wi);
    glm::vec3 world_wi = local_frame.getLocalToWorldMatrix() * local_wi;
    Ray world_new_ray = Ray(intersection_record.position_ + world_wi * 1e-3f, world_wi);
    glm::vec3 emission = scene_.getMaterial(intersection_record.material_id_).getEmission();

    float k = std::max(std::max(reflectance[0], reflectance[1]), reflectance[2]);

    if ((path_termination_criterion_ == PathTerminationCriterion::RUSSIAN_ROULETTE) && (depth >= path_length_)) {
        if ((prng_() < k) &&
            (depth < 1000))  // if RR gets crazy and does not stop, force stopping it after 1000 bounces.
            bsdf = bsdf / k;
        else
            return emission;
    }

    // Rendering Equation estimator
    radiance =
        emission +
        (bsdf * traceRay(world_new_ray, depth + 1, num_intersection_tests, num_intersections) * local_wi.y) / pdf;

    return radiance;
}

void PathTracer::render() {
    Timer timer;
    int_tests_count_ = 0;
    int_count_ = 0;
    min_int_tests_count_pp_ = std::numeric_limits<std::size_t>::max();
    max_int_tests_count_pp_ = 0;
    min_int_count_pp_ = std::numeric_limits<std::size_t>::max();
    max_int_count_pp_ = 0;

    std::cout << "+---- Rendering ---------------------------------------------------------------+\n";

#ifndef DEBUG    
    std::cout << "  Threads ........................: " << omp_get_max_threads() << "\n";
#else
    std::cout << "  Threads ........................: 1\n";
#endif 

    timer.start();

    unsigned int x_ini = camera_.getImage().getViewportLeft();
    unsigned int x_end = x_ini + camera_.getImage().getViewportWidth();
    unsigned int y_ini = camera_.getImage().getViewportTop();
    unsigned int y_end = y_ini + camera_.getImage().getViewportHeight();

#ifndef DEBUG
#pragma omp parallel for schedule(dynamic, 1) reduction(+ : int_tests_count_, int_count_) reduction(min: min_int_tests_count_pp_, min_int_count_pp_) reduction(max: max_int_tests_count_pp_, max_int_count_pp_)
#endif

    for (unsigned int y = y_ini; y < y_end; ++y) {
        printProgress(y);

        for (unsigned int x = x_ini; x < x_end; ++x) {
            std::size_t curr_int_tests_count;
            std::size_t curr_int_count;

            for (unsigned int sample = 0; sample < pixel_sampler_->getSPP(); ++sample) {
                float x_screen = x + pixel_sampler_->getSample(sample).x;
                float y_screen = y + pixel_sampler_->getSample(sample).y;

                Ray ray = camera_.getRay(x_screen, y_screen);
                glm::vec3 radiance = traceRay(ray, 0, curr_int_tests_count, curr_int_count);

                camera_.getImage().setPixelValue(x, y, camera_.getImage().getPixelValue(x, y) + radiance);
            }

            camera_.getImage().setPixelValue(
                x, y, camera_.getImage().getPixelValue(x, y) / static_cast<float>(pixel_sampler_->getSPP()));

            // update integration statistics...
            int_tests_count_ += curr_int_tests_count;
            int_count_ += curr_int_count;
            min_int_tests_count_pp_ = std::min(min_int_tests_count_pp_, curr_int_tests_count);
            max_int_tests_count_pp_ = std::max(max_int_tests_count_pp_, curr_int_tests_count);
            min_int_count_pp_ = std::min(min_int_count_pp_, curr_int_count);
            max_int_count_pp_ = std::max(max_int_count_pp_, curr_int_count);
        }
    }

    timer.finish();
    total_integration_time_ = timer.getElapsedTime();

    std::cout << "\n";
    std::cout << "  Statistics :\n";
    std::cout << "    Total rendering time .........: " << total_integration_time_ << "\n";
    std::cout << "    Intersection tests ...........: " << int_tests_count_ << "\n";
    std::cout << "      Min intersection tests pp ..: " << min_int_tests_count_pp_ << "\n";
    std::cout << "      Max intersection tests pp ..: " << max_int_tests_count_pp_ << "\n";
    std::cout << "    Intersections ................: " << int_count_ << std::fixed << std::setprecision(2) << " ("
              << (100.0f * int_count_) / int_tests_count_ << "% of all intersection tests)\n";
    std::cout << "      Min intersections pp .......: " << min_int_count_pp_ << "\n";
    std::cout << "      Max intersections pp .......: " << max_int_count_pp_ << "\n";
    std::cout << "+------------------------------------------------------------------------------+\n";
}

void PathTracer::saveImageToFile() {
    camera_.getImage().convertTosRGB();
    camera_.getImage().saveToFile();
}

void PathTracer::printProgress(unsigned int y) {
    std::stringstream progress_stream;
    progress_stream << "\r  Progress .......................: " << std::fixed << std::setw(6) << std::setprecision(2)
                    << 100.0f * y / (camera_.getImage().getViewportTop() + camera_.getImage().getViewportHeight() - 1)
                    << "%";
    std::clog << progress_stream.str();
}
