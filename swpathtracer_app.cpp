#include "swpathtracer_app.h"

void SwpathtracerApp::run(int argc, char** argv) {
    ENABLE_FEEXCEPTION_HANDLING();

    argc_ = argc;
    argv_ = argv;

    printAppInfo();
    parseCmdLineOptions();
    init();
    render();
    saveImageToFile();

    DISABLE_FEEXCEPTION_HANDLING();
}

void SwpathtracerApp::printAppInfo() {
    std::cout << APP_NAME << " - v" << APP_MAJOR_VERSION << "." << APP_MINOR_VERSION<< " - " << APP_BUILD_MODE << "\n";
    std::cout << "Copyright (C) 2016 Christian Azambuja Pagot\n";
    std::cout << "License MIT (https://opensource.org/licenses/MIT).\n";
}

void SwpathtracerApp::parseCmdLineOptions() {
    CmdLineOptions::getOptionValue(argc_, argv_, "-i", lua_script_filename_);
}

void SwpathtracerApp::loadCamera(const lb::LuaBind& lua_bind) {
    lb::ImageBuffer image_buffer = lua_bind.getImageBuffer();
    std::unique_ptr<ImageBuffer> image_buffer_ptr = std::make_unique<ImageBuffer>(
        image_buffer.width_, image_buffer.height_, image_buffer.viewport_top_, image_buffer.viewport_left_,
        image_buffer.viewport_width_, image_buffer.viewport_height_, image_buffer.file_name_);
    std::unique_ptr<lb::Camera> camera_ptr = lua_bind.getCamera();

    if (camera_ptr->type_ == lb::Camera::Type::ORTHOGRAPHIC) {
        lb::OrthographicCamera* ortho_camera_ptr = dynamic_cast<lb::OrthographicCamera*>(camera_ptr.get());
        camera_ = std::make_unique<OrthographicCamera>(
            ortho_camera_ptr->position_, ortho_camera_ptr->look_at_, ortho_camera_ptr->up_, ortho_camera_ptr->min_x_,
            ortho_camera_ptr->max_x_, ortho_camera_ptr->min_y_, ortho_camera_ptr->max_y_, std::move(image_buffer_ptr));
    } else if (camera_ptr->type_ == lb::Camera::Type::PINHOLE) {
        lb::PinholeCamera* pinhole_camera_ptr = dynamic_cast<lb::PinholeCamera*>(camera_ptr.get());
        camera_ = std::make_unique<PinholeCamera>(pinhole_camera_ptr->position_, pinhole_camera_ptr->look_at_,
                                                  pinhole_camera_ptr->up_, pinhole_camera_ptr->fov_,
                                                  std::move(image_buffer_ptr));
    }

    printCameraInfo();
}

void SwpathtracerApp::loadBSDFs(const lb::LuaBind& lua_bind) {
    unsigned int bsdf_count = lua_bind.getNumBSDFs();

    for (unsigned int i = 0; i < bsdf_count; ++i) {
        std::unique_ptr<BSDF> bsdf_ptr;
        std::unique_ptr<lb::BSDF> lua_bsdf_ptr = lua_bind.getBSDFAt(i);

        if (lua_bsdf_ptr->type_ == lb::BSDF::Type::LAMBERTIAN) {
            lb::Lambertian* lua_lambertian_ptr = dynamic_cast<lb::Lambertian*>(lua_bsdf_ptr.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler;

            if (lua_bsdf_ptr->bsdf_sampler_type_ == lb::BSDF::BSDFSamplerType::UNIFORM)
                bsdf_sampler = std::make_unique<UniformBSDFSampler>((*prng_.get()));
            else
                bsdf_sampler = std::make_unique<CosineBSDFSampler>((*prng_.get()));

            bsdf_ptr = std::make_unique<Lambertian>(lua_lambertian_ptr->kd_, std::move(bsdf_sampler));
            scene_->addBSDF(std::move(bsdf_ptr));

        } else if (lua_bsdf_ptr->type_ == lb::BSDF::Type::SMOOTH_CONDUCTOR) {
            lb::SmoothConductor* lua_smooth_conductor_ptr = dynamic_cast<lb::SmoothConductor*>(lua_bsdf_ptr.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler;
            if (lua_smooth_conductor_ptr->bsdf_sampler_type_ == lb::BSDF::BSDFSamplerType::UNIFORM)
                bsdf_sampler = std::make_unique<UniformBSDFSampler>((*prng_.get()));
            else
                bsdf_sampler = std::make_unique<SmoothSpecularBSDFSampler>();

            std::unique_ptr<Fresnel> fresnel =
                std::make_unique<FresnelSchlick>(lua_smooth_conductor_ptr->reflectance_at_normal_incidence_);
            bsdf_ptr = std::make_unique<SmoothConductor>(std::move(fresnel), std::move(bsdf_sampler));
            scene_->addBSDF(std::move(bsdf_ptr));

        } else if (lua_bsdf_ptr->type_ == lb::BSDF::Type::SMOOTH_DIELECTRIC) {
            lb::SmoothDielectric* lua_smooth_dielectric_ptr = dynamic_cast<lb::SmoothDielectric*>(lua_bsdf_ptr.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler_reflection = std::make_unique<SmoothSpecularBSDFSampler>();
            std::unique_ptr<BSDFSampler> bsdf_sampler_refraction =
                std::make_unique<RefractionBSDFSampler>(1.0f, lua_smooth_dielectric_ptr->ior_);
            std::unique_ptr<Fresnel> fresnel = std::make_unique<FresnelSchlick>(1.0f, lua_smooth_dielectric_ptr->ior_);
            bsdf_ptr = std::make_unique<SmoothDielectric>(1.0f, lua_smooth_dielectric_ptr->ior_, std::move(fresnel),
                                                          std::move(bsdf_sampler_reflection),
                                                          std::move(bsdf_sampler_refraction), (*prng_.get()));
            scene_->addBSDF(std::move(bsdf_ptr));

        } else if (lua_bsdf_ptr->type_ == lb::BSDF::Type::COOK_TORRANCE) {
            lb::CookTorrance* lua_cook_torrance_ptr = dynamic_cast<lb::CookTorrance*>(lua_bsdf_ptr.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler;

            if (lua_cook_torrance_ptr->bsdf_sampler_type_ == lb::BSDF::BSDFSamplerType::UNIFORM)
                bsdf_sampler = std::make_unique<UniformBSDFSampler>((*prng_.get()));
            else
                bsdf_sampler = std::make_unique<CookTorranceBSDFSampler>(lua_cook_torrance_ptr->roughness_, (*prng_.get()));

            std::unique_ptr<Fresnel> fresnel =
                std::make_unique<FresnelSchlick>(lua_cook_torrance_ptr->reflectance_at_normal_incidence_);
            bsdf_ptr = std::make_unique<CookTorrance>(lua_cook_torrance_ptr->roughness_, std::move(fresnel),
                                                      std::move(bsdf_sampler));
            scene_->addBSDF(std::move(bsdf_ptr));
        }
    }
}

void SwpathtracerApp::loadLayeredBSDFs(const lb::LuaBind& lua_bind) {
    unsigned int layered_bsdf_count = lua_bind.getNumLayeredBSDFs();

    for (unsigned int i = 0; i < layered_bsdf_count; ++i) {
        LayeredBSDF layered_bsdf;
        lb::LayeredBSDF lua_layered_bsdf = lua_bind.getLayeredBSDFAt(i);

        for (unsigned int k = 0; k < lua_layered_bsdf.bsdf_id_list_.size(); ++k)
            layered_bsdf.addBSDF(scene_->getBSDFPtr(lua_layered_bsdf.bsdf_id_list_[k]));

        scene_->addLayeredBSDF(layered_bsdf);
    }
}

void SwpathtracerApp::loadEmissions(const lb::LuaBind& lua_bind) {
    unsigned int emission_count = lua_bind.getNumEmissions();

    for (unsigned int i = 0; i < emission_count; ++i)
        scene_->addEmission(lua_bind.getEmissionAt(i));
}

void SwpathtracerApp::loadMaterials(const lb::LuaBind& lua_bind) {
    unsigned int material_count = lua_bind.getNumMaterials();

    for (unsigned int i = 0; i < material_count; ++i) {
        lb::Material lua_material = lua_bind.getMaterialAt(i);
        scene_->addMaterial(Material(scene_->getLayeredBSDF(lua_material.layered_bsdf_id_),
                                     scene_->getEmission(lua_material.emission_id_)));
    }
}

void SwpathtracerApp::loadPrimitives(const lb::LuaBind& lua_bind) {
    unsigned int object_count = lua_bind.getNumObjects();

    for (unsigned int i = 0; i < object_count; ++i) {
        std::unique_ptr<lb::Object> lua_object_ptr = lua_bind.getObjectAt(i);

        if (lua_object_ptr->type_ == lb::Object::Type::TRIANGLE) {
            lb::Triangle* lb_triangle_ptr = dynamic_cast<lb::Triangle*>(lua_object_ptr.get());
            std::unique_ptr<Primitive> primitive_ptr;
            primitive_ptr = std::make_unique<Triangle>(lb_triangle_ptr->v0_, lb_triangle_ptr->v1_, lb_triangle_ptr->v2_,
                                                       lb_triangle_ptr->material_id_);
            scene_->addPrimitive(std::move(primitive_ptr));
        } else if (lua_object_ptr->type_ == lb::Object::Type::SMOOTH_TRIANGLE) {
            lb::SmoothTriangle* lb_smooth_triangle_ptr = dynamic_cast<lb::SmoothTriangle*>(lua_object_ptr.get());
            std::unique_ptr<Primitive> primitive_ptr;
            primitive_ptr = std::make_unique<SmoothTriangle>(lb_smooth_triangle_ptr->v0_, lb_smooth_triangle_ptr->v1_, lb_smooth_triangle_ptr->v2_,
                                                       lb_smooth_triangle_ptr->n0_, lb_smooth_triangle_ptr->n1_, lb_smooth_triangle_ptr->n2_,
                                                       lb_smooth_triangle_ptr->material_id_);
            scene_->addPrimitive(std::move(primitive_ptr));
        } else if (lua_object_ptr->type_ == lb::Object::Type::SPHERE) {
            lb::Sphere* lb_sphere_ptr = dynamic_cast<lb::Sphere*>(lua_object_ptr.get());
            std::unique_ptr<Primitive> primitive_ptr;
            primitive_ptr =
                std::make_unique<Sphere>(lb_sphere_ptr->center_, lb_sphere_ptr->radius_, lb_sphere_ptr->material_id_);
            scene_->addPrimitive(std::move(primitive_ptr));
        } else if (lua_object_ptr->type_ == lb::Object::Type::MESH) {
            lb::Mesh* lb_mesh_ptr = dynamic_cast<lb::Mesh*>(lua_object_ptr.get());
            scene_->loadFlatMesh(lb_mesh_ptr->filename_, lb_mesh_ptr->material_id_);
        } else if (lua_object_ptr->type_ == lb::Object::Type::SMOOTH_MESH) {
            lb::SmoothMesh* lb_smooth_mesh_ptr = dynamic_cast<lb::SmoothMesh*>(lua_object_ptr.get());
            scene_->loadSmoothMesh(lb_smooth_mesh_ptr->filename_, lb_smooth_mesh_ptr->material_id_);
        }
    }
}

void SwpathtracerApp::loadScene(const lb::LuaBind& lua_bind) {
    lb::GlobalSettings lua_global_settings = lua_bind.getGlobalSettings();
    std::unique_ptr<AccelerationStructure> acceleration_structure_ptr;

    if (lua_global_settings.acceleration_structure_ == lb::GlobalSettings::AccelerationStructureType::NONE) {
        acceleration_structure_ptr = std::make_unique<NoAccelerationStructure>();
    } else if (lua_global_settings.acceleration_structure_ == lb::GlobalSettings::AccelerationStructureType::BVH_SAH) {
        acceleration_structure_ptr = std::make_unique<SBVH>(std::numeric_limits<float>::infinity());
    } else if (lua_global_settings.acceleration_structure_ == lb::GlobalSettings::AccelerationStructureType::SBVH_SAH) {
        acceleration_structure_ptr = std::make_unique<SBVH>(lua_global_settings.overlap_threshold_);
    }

    scene_ = std::make_unique<Scene>(std::move(acceleration_structure_ptr), lua_global_settings.background_color_);

    loadBSDFs(lua_bind);
    loadLayeredBSDFs(lua_bind);
    loadEmissions(lua_bind);
    loadMaterials(lua_bind);
    loadPrimitives(lua_bind);
    printSceneInfo();

    std::cout << "+---- Acceleration structure --------------------------------------------------+\n";
    std::cout << "  Type: ";

    std::string accel_type;
    if (scene_->getAccelerationStructureType() == AccelerationStructure::Type::NONE)
        std::cout << "none (array)\n";
    else if (scene_->getAccelerationStructureType() == AccelerationStructure::Type::BVH_SAH)
        std::cout << "SAH-based BVH\n";
    else if (scene_->getAccelerationStructureType() == AccelerationStructure::Type::SBVH_SAH)
        std::cout << "SAH-based SBVH\n";

    scene_->buildAccelerationStructure();

    if (scene_->getAccelerationStructureType() == AccelerationStructure::Type::NONE) {
        const StructureStatsNoAccel* statistics = static_cast<const StructureStatsNoAccel*>(&scene_->getAccelerationStructure().getStatistics());
        std::cout << "  Build time: " << statistics->build_time_ << " usec\n";
        std::cout << "  Primitives: " << statistics->num_references_ << "\n";
    } else if (scene_->getAccelerationStructureType() == AccelerationStructure::Type::BVH_SAH) {
        const StructureStatsSBVH* statistics = static_cast<const StructureStatsSBVH*>(&scene_->getAccelerationStructure().getStatistics());
        std::cout << "  Build time: " << statistics->build_time_ << " usec\n";
        std::cout << "  References: " << statistics->num_references_ << "\n";
        std::cout << "  Tree height: " << statistics->tree_height_ << "\n";
        std::cout << "  Nodes: " << statistics->num_nodes_ << "\n";
        std::cout << "    Inner nodes: " << statistics->num_inner_nodes_ << "\n";
        std::cout << "    Leaf nodes: " << statistics->num_leaf_nodes_ << "\n";
        std::cout << "  Min/max refs per leaf node: "
                  << statistics->min_num_primitives_leaf_ << " / "
                  << statistics->max_num_primitives_leaf_ << "\n";
        std::cout << "  Unreferenced primitives: " << statistics->num_unreferenced_primitives_ << "\n";
    } else if (scene_->getAccelerationStructureType() == AccelerationStructure::Type::SBVH_SAH) {
        const StructureStatsSBVH* statistics = static_cast<const StructureStatsSBVH*>(&scene_->getAccelerationStructure().getStatistics());
        std::cout << "  Build time: " << statistics->build_time_ << " usec\n";
        std::cout << "  Object / spatial splits: " << statistics->num_object_splits_ << " / " << statistics->num_spatial_splits_ << "\n";
        std::cout << "  References: " << statistics->num_references_ << "\n";
        std::cout << "  Tree height: " << statistics->tree_height_ << "\n";
        std::cout << "  Nodes: " << statistics->num_nodes_ << "\n";
        std::cout << "    Inner nodes: " << statistics->num_inner_nodes_ << "\n";
        std::cout << "    Leaf nodes: " << statistics->num_leaf_nodes_ << "\n";
        std::cout << "  Min/max refs per leaf node: "
                  << statistics->min_num_primitives_leaf_ << " / "
                  << statistics->max_num_primitives_leaf_ << "\n";
        std::cout << "  Unreferenced primitives: " << statistics->num_unreferenced_primitives_ << "\n";
    }

    std::cout << "+------------------------------------------------------------------------------+\n";
}

void SwpathtracerApp::loadIntegrator(const lb::LuaBind& lua_bind) {
    std::unique_ptr<lb::Integrator> integrator_ptr = lua_bind.getIntegrator();

    if (integrator_ptr->type_ == lb::Integrator::Type::NORMAL_RAYCASTER) {
        integrator_ = std::make_unique<NormalRayCaster>((*camera_.get()), (*scene_.get()));
    } else if (integrator_ptr->type_ == lb::Integrator::Type::DEPTH_RAYCASTER) {
        integrator_ =
            std::make_unique<DepthRayCaster>((*camera_.get()), (*scene_.get()), integrator_ptr->prescribed_min_depth_,
                                             integrator_ptr->prescribed_max_depth_);
    } else if (integrator_ptr->type_ == lb::Integrator::Type::INTERSECTION_TEST_COUNT_RAYCASTER) {
        integrator_ = std::make_unique<IntersectionTestCountRayCaster>(
            (*camera_.get()), (*scene_.get()), integrator_ptr->prescribed_min_intersection_tests_count_,
            integrator_ptr->prescribed_max_intersection_tests_count_);
    } else if (integrator_ptr->type_ == lb::Integrator::Type::PATHTRACER) {
        std::unique_ptr<lb::PixelSampler> pixel_sampler_ptr = lua_bind.getPixelSampler();

        std::unique_ptr<PixelSampler> pixel_sampler;
        if (pixel_sampler_ptr->type_ == lb::PixelSampler::Type::UNIFORM) {
            pixel_sampler = std::make_unique<UniformPixelSampler>(pixel_sampler_ptr->spp_, (*prng_.get()));
        } else if (pixel_sampler_ptr->type_ == lb::PixelSampler::Type::REGULAR) {
            pixel_sampler = std::make_unique<RegularPixelSampler>(pixel_sampler_ptr->spp_);
        } else if (pixel_sampler_ptr->type_ == lb::PixelSampler::Type::JITTERED) {
           pixel_sampler = std::make_unique<JitteredPixelSampler>(pixel_sampler_ptr->spp_, (*prng_.get()));
        }

        PathTracer::PathTerminationCriterion path_termination_criterion;
        if (integrator_ptr->path_termination_criterion_ == lb::Integrator::PathTerminationCriterion::MAX_DEPTH)
            path_termination_criterion = PathTracer::PathTerminationCriterion::MAX_DEPTH;
        else if (integrator_ptr->path_termination_criterion_ == lb::Integrator::PathTerminationCriterion::RUSSIAN_ROULETTE)
            path_termination_criterion = PathTracer::PathTerminationCriterion::RUSSIAN_ROULETTE;

        integrator_ = std::make_unique<PathTracer>((*camera_.get()), (*scene_.get()),
                                                   path_termination_criterion, integrator_ptr->path_length_,
                                                   std::move(pixel_sampler), (*prng_.get()));
    }

    printIntegratorInfo();
}

void SwpathtracerApp::printCameraInfo() const {
    std::cout << "+---- Camera ------------------------------------------------------------------+\n";

    if (camera_->getType() == Camera::Type::ORTHOGRAPHIC)
        std::cout << "  Type: orthographic\n";
    else if (camera_->getType() == Camera::Type::PINHOLE)
        std::cout << "  Type: pinhole\n";

    std::cout << "  Position: [" << camera_->getPosition()[0] << ", " << camera_->getPosition()[1] << ", "
              << camera_->getPosition()[2] << "]\n";
    std::cout << "  Look at: [" << camera_->getLookAt()[0] << ", " << camera_->getLookAt()[1] << ", "
              << camera_->getLookAt()[2] << "]\n";
    std::cout << "  Up: [" << camera_->getUp()[0] << ", " << camera_->getUp()[1] << ", " << camera_->getUp()[2]
              << "]\n";

    if (camera_->getType() == Camera::Type::ORTHOGRAPHIC) {
        OrthographicCamera* ortho_cam_ptr = dynamic_cast<OrthographicCamera*>(camera_.get());
        std::cout << "  Aspect: " << ortho_cam_ptr->getAspect() << "\n";
        std::cout << "  Window x range: [" << ortho_cam_ptr->getMinX() << ", " << ortho_cam_ptr->getMaxX() << "]\n";
        std::cout << "  Window y range: [" << ortho_cam_ptr->getMinY() << ", " << ortho_cam_ptr->getMaxY() << "]\n";
    } else if (camera_->getType() == Camera::Type::PINHOLE) {
        PinholeCamera* pinhole_cam_ptr = dynamic_cast<PinholeCamera*>(camera_.get());
        std::cout << "  Aspect: " << pinhole_cam_ptr->getAspect() << "\n";
        std::cout << "  Vertical FOV: " << pinhole_cam_ptr->getVFov() << "\n";
    }

    std::cout << "+------------------------------------------------------------------------------+\n";
}

void SwpathtracerApp::printSceneInfo() const {
    std::cout << "+---- Scene -------------------------------------------------------------------+\n";
    std::cout << "  BSDFs: " << scene_->getNumBSDFs() << "\n";
    std::cout << "  Layered BSDFs: " << scene_->getNumLayeredBSDFs() << "\n";
    std::cout << "  Emissions: " << scene_->getNumEmissions() << "\n";
    std::cout << "  Materials: " << scene_->getNumMaterials() << "\n";
    std::cout << "  Primitives: " << scene_->getNumPrimitives() << "\n";
    std::cout << "  Scene extents: \n"
              << "    x: [" << scene_->x_min_ << ", " << scene_->x_max_ << "]\n"
              << "    y: [" << scene_->y_min_ << ", " << scene_->y_max_ << "]\n"
              << "    z: [" << scene_->z_min_ << ", " << scene_->z_max_ << "]\n";
    std::cout << "+------------------------------------------------------------------------------+\n";
}

void SwpathtracerApp::printIntegratorInfo() const {
    std::cout << "+---- Integrator --------------------------------------------------------------+\n";
    std::cout << "  Type: ";

    if (integrator_->getType() == Integrator::Type::NORMAL_RAYCASTER) {
        std::cout << "Normal Ray Caster\n";
    } else if (integrator_->getType() == Integrator::Type::DEPTH_RAYCASTER) {
        std::cout << "Depth Ray Caster\n";
    } else if (integrator_->getType() == Integrator::Type::INTERSECTION_TEST_COUNT_RAYCASTER) {
        std::cout << "Intersection Test Count Ray Caster\n";
    } else if (integrator_->getType() == Integrator::Type::PATHTRACER) {
        std::cout << "Forward Path Tracer\n";
        PathTracer* path_tracer = dynamic_cast<PathTracer*>(integrator_.get());
        std::cout << "  Path termination criterion: ";

        if (path_tracer->getPathTerminationCriterion() == PathTracer::PathTerminationCriterion::MAX_DEPTH)
            std::cout << "maximum depth (path length = " << path_tracer->getPathLength() << ")\n";
        else if (path_tracer->getPathTerminationCriterion() == PathTracer::PathTerminationCriterion::RUSSIAN_ROULETTE)
            std::cout << "Russian Roulette (starting after " << path_tracer->getPathLength() << " bounce(s))\n";
    }

    std::cout << "  spp: " << integrator_->getPixelSampler().getSPP() << "\n";

    std::cout << "  Pixel sampler: ";
    if (integrator_->getPixelSampler().getType() == PixelSampler::Type::UNIFORM)
        std::cout << "uniform (box filter)\n";
    else if (integrator_->getPixelSampler().getType() == PixelSampler::Type::REGULAR)
        std::cout << "regular (box filter)\n";
    else if (integrator_->getPixelSampler().getType() == PixelSampler::Type::JITTERED)
        std::cout << "jittered (box filter)\n";

    std::cout << "+------------------------------------------------------------------------------+\n";
}

// void SwpathtracerApp::printPreRenderingInfo() {


//     std::cout << "+---- Integrator --------------------------------------------------------------+\n";
//     std::cout << "|  Type: ";


//     std::cout << "+------------------------------------------------------------------------------+\n";

//     // print integrator info ///////////////////////////////////////////////////////
//     // std::cout << "Integrator:\n";
//     // if (integrator_->getType() == Integrator::Type::RAYCASTER) {
//     //     RayCaster* ray_caster_ptr = dynamic_cast<RayCaster*>(integrator_.get());
//     //     std::cout << "    Type: ray caster\n";
//     // } else if (integrator_->getType() == Integrator::Type::PATHTRACER) {
//     //     PathTracer* path_tracer_ptr = dynamic_cast<PathTracer*>(integrator_.get());
//     //     std::cout << "    Type: path tracer\n";
//     // }

//     //std::cout << "Num threads:" << omp_get_max_threads() << "\n";
// }

// void SwpathtracerApp::printPostRenderingInfo() {
//     // std::cout << "Rendering statistics:\n";
//     // std::cout << "    Intersection tests:\n";
//     // std::cout << "        min intersection tests pp: " << integrator_->getMinIntersectionTestsPerPixel() << "\n";
//     // std::cout << "        max intersection tests pp: " << integrator_->getMaxIntersectionTestsPerPixel() << "\n";
//     // std::cout << "        num intersection tests: " << integrator_->getNumIntersectionTests() << "\n";
//     // std::cout << "    Intersections:\n";
//     // std::cout << "        min intersections pp: " << integrator_->getMinIntersectionsPerPixel() << "\n";
//     // std::cout << "        max intersections pp: " << integrator_->getMaxIntersectionsPerPixel() << "\n";
//     // std::cout << "        num intersections: " << integrator_->getNumIntersections() << "\n";


// }

void SwpathtracerApp::initPRNG() {
    prng_ = std::make_unique<PRNG<std::uniform_real_distribution, float, std::mt19937>>(0.0f, 1.0f);
}

void SwpathtracerApp::init() {
    lb::LuaBind lua_bind;
    lua_bind.runScript(lua_script_filename_);

    initPRNG();
    loadCamera(lua_bind);
    loadScene(lua_bind);
    loadIntegrator(lua_bind);
}

void SwpathtracerApp::render() {
    integrator_->render();
}

void SwpathtracerApp::saveImageToFile() {
    integrator_->saveImageToFile();
}
