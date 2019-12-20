#ifndef CORE_SWPATHTRACER_APP_H
#define CORE_SWPATHTRACER_APP_H

#include <fenv.h>
#include <iostream>
#include <unordered_map>

#include "cmd_line_options.h"
#include "cook_torrance.h"
#include "cook_torrance_bsdf_sampler.h"
#include "core.h"
#include "cosine_bsdf_sampler.h"
#include "depth_raycaster.h"
#include "fresnel_schlick.h"
#include "image_buffer.h"
#include "int_test_count_raycaster.h"
#include "jittered_pixel_sampler.h"
#include "lambertian.h"
#include "layered_bsdf.h"
#include "lua_bind.h"
#include "material.h"
#include "normal_raycaster.h"
#include "orthographic_camera.h"
#include "pathtracer.h"
#include "pinhole_camera.h"
#include "prng.h"
#include "refraction_bsdf_sampler.h"
#include "regular_pixel_sampler.h"
#include "scene.h"
#include "smooth_conductor.h"
#include "smooth_dielectric.h"
#include "smooth_specular_bsdf_sampler.h"
#include "smooth_triangle.h"
#include "sphere.h"
#include "triangle.h"
#include "uniform_bsdf_sampler.h"
#include "uniform_pixel_sampler.h"

class SwpathtracerApp {
   public:
    SwpathtracerApp(int argc, char** argv);
    ~SwpathtracerApp();
    void run();

   private:
    void printAppInfo();
    void parseCmdLineOptions();
    void initPRNG();
    void loadCamera(const lb::LuaBind& lua_bind);
    std::unique_ptr<BSDFSampler> getBSDFSampler(const lb::BSDF* lua_bind_bsdf);
    void loadBSDFs(const lb::LuaBind& lua_bind);
    void loadLayeredBSDFs(const lb::LuaBind& lua_bind);
    void loadEmissions(const lb::LuaBind& lua_bind);
    void loadMaterials(const lb::LuaBind& lua_bind);
    void loadPrimitives(const lb::LuaBind& lua_bind);
    void loadScene(const lb::LuaBind& lua_bind);
    void loadIntegrator(const lb::LuaBind& lua_bind);
    void printCameraInfo() const;
    void printInOutInfo() const;
    void printSceneInfo() const;
    void printIntegratorInfo() const;
    void init();
    void render();
    void saveImageToFile();

    int argc_;
    char** argv_;
    std::string lua_script_filename_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<Scene> scene_;
    std::unique_ptr<Integrator> integrator_;
    std::unique_ptr<PRNG<std::uniform_real_distribution, float, std::mt19937>> prng_;
};

#endif  // CORE_SWPATHTRACER_APP_H
