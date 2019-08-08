#include "integrator.h"

Integrator::Integrator(Camera& camera, const Scene& scene, Type type, std::unique_ptr<PixelSampler> pixel_sampler)
    : type_(type),
      camera_(camera),
      scene_(scene),
      pixel_sampler_(std::move(pixel_sampler)),
      total_integration_time_(0) {}

Integrator::~Integrator() {}
