#include "swpathtracer_app.h"

SwpathtracerApp::SwpathtracerApp(int argc, char** argv) : argc_(argc), argv_(argv) {}

SwpathtracerApp::~SwpathtracerApp() {}

void SwpathtracerApp::run() {
    ENABLE_FEEXCEPTION_HANDLING();

    printAppInfo();
    parseCmdLineOptions();
    init();
    render();
    saveImageToFile();

    DISABLE_FEEXCEPTION_HANDLING();
}

void SwpathtracerApp::printAppInfo() {
    std::cout << APP_NAME << " - v" << APP_MAJOR_VERSION << "." << APP_MINOR_VERSION << " - " << APP_BUILD_MODE << "\n";
    std::cout << "Copyright (C) 2016 Christian Azambuja Pagot\n";
    std::cout << "License MIT (https://opensource.org/licenses/MIT).\n";
}

void SwpathtracerApp::parseCmdLineOptions() {
    CmdLineOptions::getOptionValue(argc_, argv_, "-i", lua_script_filename_);
}

void SwpathtracerApp::init() {
    lb::LuaBind lua_bind;
    lua_bind.runScript(lua_script_filename_);

    initPRNG();
    loadCamera(lua_bind);
    loadScene(lua_bind);
    loadIntegrator(lua_bind);
}

void SwpathtracerApp::initPRNG() {
    prng_ = std::make_unique<PRNG<std::uniform_real_distribution, float, std::mt19937>>(0.0f, 1.0f);
}

void SwpathtracerApp::loadCamera(const lb::LuaBind& lua_bind) {
    // TODO: having Viewport as a separate object would decouple it from the ImageBuffer
    lb::ImageBuffer lua_image_buffer = lua_bind.getImageBuffer();
    std::unique_ptr<ImageBuffer> image_buffer =
        std::make_unique<ImageBuffer>(lua_image_buffer.width_, lua_image_buffer.height_, lua_image_buffer.viewport_top_,
                                      lua_image_buffer.viewport_left_, lua_image_buffer.viewport_width_,
                                      lua_image_buffer.viewport_height_, lua_image_buffer.file_name_);
    std::unique_ptr<lb::Camera> lua_camera = lua_bind.getCamera();

    if (lua_camera->type_ == lb::Camera::Type::ORTHOGRAPHIC) {
        lb::OrthographicCamera* lua_ortho_camera = dynamic_cast<lb::OrthographicCamera*>(lua_camera.get());
        camera_ = std::make_unique<OrthographicCamera>(
            lua_ortho_camera->position_, lua_ortho_camera->look_at_, lua_ortho_camera->up_, lua_ortho_camera->min_x_,
            lua_ortho_camera->max_x_, lua_ortho_camera->min_y_, lua_ortho_camera->max_y_, std::move(image_buffer));
    } else if (lua_camera->type_ == lb::Camera::Type::PINHOLE) {
        lb::PinholeCamera* pinhole_camera = dynamic_cast<lb::PinholeCamera*>(lua_camera.get());
        camera_ = std::make_unique<PinholeCamera>(pinhole_camera->position_, pinhole_camera->look_at_,
                                                  pinhole_camera->up_, pinhole_camera->fov_, std::move(image_buffer));
    }

    printCameraInfo();
}

void SwpathtracerApp::loadScene(const lb::LuaBind& lua_bind) {
    lb::GlobalSettings lua_global_settings = lua_bind.getGlobalSettings();
    std::unique_ptr<AccelStructure> accel_structure;

    if (lua_global_settings.accel_structure_ == lb::GlobalSettings::AccelStructureType::NONE) {
        accel_structure = std::make_unique<NoAccelStructure>();
    } else if (lua_global_settings.accel_structure_ == lb::GlobalSettings::AccelStructureType::BVH_SAH) {
        accel_structure = std::make_unique<SBVH>(std::numeric_limits<float>::infinity());
    } else if (lua_global_settings.accel_structure_ == lb::GlobalSettings::AccelStructureType::SBVH_SAH) {
        accel_structure = std::make_unique<SBVH>(lua_global_settings.overlap_threshold_);
    }

    scene_ = std::make_unique<Scene>(std::move(accel_structure), lua_global_settings.background_color_);

    loadBSDFs(lua_bind);
    loadLayeredBSDFs(lua_bind);
    loadEmissions(lua_bind);
    loadMaterials(lua_bind);
    loadPrimitives(lua_bind);
    printSceneInfo();
}

void SwpathtracerApp::loadBSDFs(const lb::LuaBind& lua_bind) {
    unsigned int bsdf_count = lua_bind.getNumBSDFs();

    for (unsigned int i = 0; i < bsdf_count; ++i) {
        std::unique_ptr<BSDF> bsdf;
        std::unique_ptr<lb::BSDF> lua_bsdf = lua_bind.getBSDFAt(i);

        if (lua_bsdf->type_ == lb::BSDF::Type::LAMBERTIAN) {
            lb::Lambertian* lua_lambertian = dynamic_cast<lb::Lambertian*>(lua_bsdf.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler;

            if (lua_bsdf->sampler_type_ == lb::BSDF::SamplerType::UNIFORM)
                bsdf_sampler = std::make_unique<UniformBSDFSampler>((*prng_.get()));
            else
                bsdf_sampler = std::make_unique<CosineBSDFSampler>((*prng_.get()));

            bsdf = std::make_unique<Lambertian>(lua_lambertian->kd_, std::move(bsdf_sampler));
            scene_->addBSDF(std::move(bsdf));

        } else if (lua_bsdf->type_ == lb::BSDF::Type::SMOOTH_CONDUCTOR) {
            lb::SmoothConductor* lua_smooth_conductor = dynamic_cast<lb::SmoothConductor*>(lua_bsdf.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler;

            if (lua_smooth_conductor->sampler_type_ == lb::BSDF::SamplerType::UNIFORM)
                bsdf_sampler = std::make_unique<UniformBSDFSampler>((*prng_.get()));
            else
                bsdf_sampler = std::make_unique<SmoothSpecularBSDFSampler>();

            std::unique_ptr<Fresnel> fresnel =
                std::make_unique<FresnelSchlick>(lua_smooth_conductor->reflectance_at_normal_incidence_);
            bsdf = std::make_unique<SmoothConductor>(std::move(fresnel), std::move(bsdf_sampler));
            scene_->addBSDF(std::move(bsdf));

        } else if (lua_bsdf->type_ == lb::BSDF::Type::SMOOTH_DIELECTRIC) {
            lb::SmoothDielectric* lua_smooth_dielectric = dynamic_cast<lb::SmoothDielectric*>(lua_bsdf.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler_reflection = std::make_unique<SmoothSpecularBSDFSampler>();
            std::unique_ptr<BSDFSampler> bsdf_sampler_refraction =
                std::make_unique<RefractionBSDFSampler>(1.0f, lua_smooth_dielectric->ior_);
            std::unique_ptr<Fresnel> fresnel = std::make_unique<FresnelSchlick>(1.0f, lua_smooth_dielectric->ior_);
            bsdf = std::make_unique<SmoothDielectric>(1.0f, lua_smooth_dielectric->ior_, std::move(fresnel),
                                                      std::move(bsdf_sampler_reflection),
                                                      std::move(bsdf_sampler_refraction), (*prng_.get()));
            scene_->addBSDF(std::move(bsdf));

        } else if (lua_bsdf->type_ == lb::BSDF::Type::COOK_TORRANCE) {
            lb::CookTorrance* lua_cook_torrance = dynamic_cast<lb::CookTorrance*>(lua_bsdf.get());
            std::unique_ptr<BSDFSampler> bsdf_sampler;

            if (lua_cook_torrance->sampler_type_ == lb::BSDF::SamplerType::UNIFORM)
                bsdf_sampler = std::make_unique<UniformBSDFSampler>((*prng_.get()));
            else
                bsdf_sampler = std::make_unique<CookTorranceBSDFSampler>(lua_cook_torrance->roughness_, (*prng_.get()));

            std::unique_ptr<Fresnel> fresnel =
                std::make_unique<FresnelSchlick>(lua_cook_torrance->reflectance_at_normal_incidence_);
            bsdf = std::make_unique<CookTorrance>(lua_cook_torrance->roughness_, std::move(fresnel),
                                                  std::move(bsdf_sampler));
            scene_->addBSDF(std::move(bsdf));
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

    for (unsigned int i = 0; i < emission_count; ++i) scene_->addEmission(lua_bind.getEmissionAt(i));
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
        std::unique_ptr<lb::Object> lua_object = lua_bind.getObjectAt(i);

        if (lua_object->type_ == lb::Object::Type::TRIANGLE) {
            lb::Triangle* lua_triangle = dynamic_cast<lb::Triangle*>(lua_object.get());
            std::unique_ptr<Primitive> primitive;
            primitive = std::make_unique<Triangle>(lua_triangle->v0_, lua_triangle->v1_, lua_triangle->v2_,
                                                   lua_triangle->material_id_);
            scene_->addPrimitive(std::move(primitive));
        } else if (lua_object->type_ == lb::Object::Type::SMOOTH_TRIANGLE) {
            lb::SmoothTriangle* lua_smooth_triangle = dynamic_cast<lb::SmoothTriangle*>(lua_object.get());
            std::unique_ptr<Primitive> primitive;
            primitive = std::make_unique<SmoothTriangle>(
                lua_smooth_triangle->v0_, lua_smooth_triangle->v1_, lua_smooth_triangle->v2_, lua_smooth_triangle->n0_,
                lua_smooth_triangle->n1_, lua_smooth_triangle->n2_, lua_smooth_triangle->material_id_);
            scene_->addPrimitive(std::move(primitive));
        } else if (lua_object->type_ == lb::Object::Type::SPHERE) {
            lb::Sphere* lua_sphere = dynamic_cast<lb::Sphere*>(lua_object.get());
            std::unique_ptr<Primitive> primitive;
            primitive = std::make_unique<Sphere>(lua_sphere->center_, lua_sphere->radius_, lua_sphere->material_id_);
            scene_->addPrimitive(std::move(primitive));
        } else if (lua_object->type_ == lb::Object::Type::MESH) {
            lb::Mesh* lua_mesh = dynamic_cast<lb::Mesh*>(lua_object.get());
            scene_->loadFlatMesh(lua_mesh->filename_, lua_mesh->material_id_);
        } else if (lua_object->type_ == lb::Object::Type::SMOOTH_MESH) {
            lb::SmoothMesh* lua_smooth_mesh = dynamic_cast<lb::SmoothMesh*>(lua_object.get());
            scene_->loadSmoothMesh(lua_smooth_mesh->filename_, lua_smooth_mesh->material_id_);
        }
    }
}

void SwpathtracerApp::loadIntegrator(const lb::LuaBind& lua_bind) {
    std::unique_ptr<lb::Integrator> lua_integrator = lua_bind.getIntegrator();

    if (lua_integrator->type_ == lb::Integrator::Type::NORMAL_RAYCASTER) {
        integrator_ = std::make_unique<NormalRayCaster>((*camera_.get()), (*scene_.get()));
    } else if (lua_integrator->type_ == lb::Integrator::Type::DEPTH_RAYCASTER) {
        integrator_ =
            std::make_unique<DepthRayCaster>((*camera_.get()), (*scene_.get()), lua_integrator->prescribed_min_depth_,
                                             lua_integrator->prescribed_max_depth_);
    } else if (lua_integrator->type_ == lb::Integrator::Type::INT_TEST_COUNT_RAYCASTER) {
        integrator_ = std::make_unique<IntTestCountRayCaster>((*camera_.get()), (*scene_.get()),
                                                              lua_integrator->prescribed_min_int_tests_count_,
                                                              lua_integrator->prescribed_max_int_tests_count_);
    } else if (lua_integrator->type_ == lb::Integrator::Type::PATHTRACER) {
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
        if (lua_integrator->path_termination_criterion_ == lb::Integrator::PathTerminationCriterion::MAX_DEPTH)
            path_termination_criterion = PathTracer::PathTerminationCriterion::MAX_DEPTH;
        else if (lua_integrator->path_termination_criterion_ ==
                 lb::Integrator::PathTerminationCriterion::RUSSIAN_ROULETTE)
            path_termination_criterion = PathTracer::PathTerminationCriterion::RUSSIAN_ROULETTE;

        integrator_ =
            std::make_unique<PathTracer>((*camera_.get()), (*scene_.get()), path_termination_criterion,
                                         lua_integrator->path_length_, std::move(pixel_sampler), (*prng_.get()));
    }

    printIntegratorInfo();
}

void SwpathtracerApp::printCameraInfo() const {
    std::cout << "Camera\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    if (camera_->getType() == Camera::Type::ORTHOGRAPHIC)
        std::cout << "  Type ...........................: orthographic\n";
    else if (camera_->getType() == Camera::Type::PINHOLE)
        std::cout << "  Type ...........................: pinhole\n";

    std::cout << "  Position .......................: [" << camera_->getPosition()[0] << ", "
              << camera_->getPosition()[1] << ", " << camera_->getPosition()[2] << "]\n";
    std::cout << "  Look at ........................: [" << camera_->getLookAt()[0] << ", " << camera_->getLookAt()[1]
              << ", " << camera_->getLookAt()[2] << "]\n";
    std::cout << "  Up .............................: [" << camera_->getUp()[0] << ", " << camera_->getUp()[1] << ", "
              << camera_->getUp()[2] << "]\n";

    if (camera_->getType() == Camera::Type::ORTHOGRAPHIC) {
        OrthographicCamera* ortho_cam = dynamic_cast<OrthographicCamera*>(camera_.get());
        std::cout << "  Aspect .........................: " << ortho_cam->getAspect() << "\n";
        std::cout << "  Window x range .................: [" << ortho_cam->getMinX() << ", " << ortho_cam->getMaxX()
                  << "]\n";
        std::cout << "  Window y range .................: [" << ortho_cam->getMinY() << ", " << ortho_cam->getMaxY()
                  << "]\n";
    } else if (camera_->getType() == Camera::Type::PINHOLE) {
        PinholeCamera* pinhole_cam_ptr = dynamic_cast<PinholeCamera*>(camera_.get());
        std::cout << "  Aspect .........................: " << pinhole_cam_ptr->getAspect() << "\n";
        std::cout << "  Vertical FOV ...................: " << pinhole_cam_ptr->getVFov() << "\n";
    }

    std::cout << "\n";
}

void SwpathtracerApp::printSceneInfo() const {
    std::cout << "Scene\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "  Background color ...............: [" << scene_->getBackgroundColor()[0] << ", "
              << scene_->getBackgroundColor()[1] << ", " << scene_->getBackgroundColor()[2] << "]\n";
    std::cout << "  BSDFs ..........................: " << scene_->getNumBSDFs() << "\n";
    std::cout << "  Layered BSDFs ..................: " << scene_->getNumLayeredBSDFs() << "\n";
    std::cout << "  Emissions ......................: " << scene_->getNumEmissions() << "\n";
    std::cout << "  Materials ......................: " << scene_->getNumMaterials() << "\n";
    std::cout << "  Primitives .....................: " << scene_->getNumPrimitives() << "\n";
    std::cout << "  Scene extents : \n"
              << "    x ............................: [" << scene_->x_min_ << ", " << scene_->x_max_ << "]\n"
              << "    y ............................: [" << scene_->y_min_ << ", " << scene_->y_max_ << "]\n"
              << "    z ............................: [" << scene_->z_min_ << ", " << scene_->z_max_ << "]\n";
    std::cout << "\n";
    std::cout << "Acceleration structure\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "  Type ...........................: ";

    std::string accel_type;
    if (scene_->getAccelStructureType() == AccelStructure::Type::NONE)
        std::cout << "none (array)\n";
    else if (scene_->getAccelStructureType() == AccelStructure::Type::BVH_SAH)
        std::cout << "SAH-based BVH\n";
    else if (scene_->getAccelStructureType() == AccelStructure::Type::SBVH_SAH)
        std::cout << "SAH-based SBVH\n";

    scene_->buildAccelStructure();

    if (scene_->getAccelStructureType() == AccelStructure::Type::NONE) {
        const StatsNoAccel* stats = static_cast<const StatsNoAccel*>(&scene_->getAccelStructure().getStatistics());
        std::cout << "  Build time .....................: " << stats->build_time_ << " usec\n";
        std::cout << "  Primitives ...........................: " << stats->num_references_ << "\n";
    } else if (scene_->getAccelStructureType() == AccelStructure::Type::BVH_SAH) {
        const StatsSBVH* stats = static_cast<const StatsSBVH*>(&scene_->getAccelStructure().getStatistics());
        std::cout << "  Build time .....................: " << stats->build_time_ << " usec\n";
        std::cout << "  References .....................: " << stats->num_references_ << "\n";
        std::cout << "  Tree height ....................: " << stats->tree_height_ << "\n";
        std::cout << "  Nodes ..........................: " << stats->num_nodes_ << "\n";
        std::cout << "    Inner nodes ..................: " << stats->num_inner_nodes_ << "\n";
        std::cout << "    Leaf nodes ...................: " << stats->num_leaf_nodes_ << "\n";
        std::cout << "  Min/max refs per leaf node .....: " << stats->min_num_primitives_leaf_ << " / "
                  << stats->max_num_primitives_leaf_ << "\n";
        std::cout << "  Unreferenced primitives ........: " << stats->num_unreferenced_primitives_ << "\n";
    } else if (scene_->getAccelStructureType() == AccelStructure::Type::SBVH_SAH) {
        const StatsSBVH* stats = static_cast<const StatsSBVH*>(&scene_->getAccelStructure().getStatistics());
        std::cout << "  Build time .....................: " << stats->build_time_ << " usec\n";
        std::cout << "  Object / spatial splits ........: " << stats->num_object_splits_ << " / "
                  << stats->num_spatial_splits_ << "\n";
        std::cout << "  References .....................: " << stats->num_references_ << "\n";
        std::cout << "  Tree height ....................: " << stats->tree_height_ << "\n";
        std::cout << "  Nodes ..........................: " << stats->num_nodes_ << "\n";
        std::cout << "    Inner nodes ..................: " << stats->num_inner_nodes_ << "\n";
        std::cout << "    Leaf nodes ...................: " << stats->num_leaf_nodes_ << "\n";
        std::cout << "  Min/max refs per leaf node .....: " << stats->min_num_primitives_leaf_ << " / "
                  << stats->max_num_primitives_leaf_ << "\n";
        std::cout << "  Unreferenced primitives ........: " << stats->num_unreferenced_primitives_ << "\n";
    }

    std::cout << "\n";
}

void SwpathtracerApp::printIntegratorInfo() const {
    std::cout << "Integrator\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "  Type ...........................: ";

    if (integrator_->getType() == Integrator::Type::NORMAL_RAYCASTER) {
        std::cout << "Normal Ray Caster\n";
    } else if (integrator_->getType() == Integrator::Type::DEPTH_RAYCASTER) {
        std::cout << "Depth Ray Caster\n";
    } else if (integrator_->getType() == Integrator::Type::INTERSECTION_TEST_COUNT_RAYCASTER) {
        std::cout << "Intersection Test Count Ray Caster\n";
    } else if (integrator_->getType() == Integrator::Type::PATHTRACER) {
        std::cout << "Forward Path Tracer\n";
        PathTracer* path_tracer = dynamic_cast<PathTracer*>(integrator_.get());
        std::cout << "  Path termination criterion .....: ";

        if (path_tracer->getPathTerminationCriterion() == PathTracer::PathTerminationCriterion::MAX_DEPTH)
            std::cout << "maximum depth (path length = " << path_tracer->getPathLength() << ")\n";
        else if (path_tracer->getPathTerminationCriterion() == PathTracer::PathTerminationCriterion::RUSSIAN_ROULETTE)
            std::cout << "Russian Roulette (starting after " << path_tracer->getPathLength() << " bounce(s))\n";
    }

    std::cout << "  spp ............................: " << integrator_->getPixelSampler().getSPP() << "\n";

    std::cout << "  Pixel sampler ..................: ";
    if (integrator_->getPixelSampler().getType() == PixelSampler::Type::UNIFORM)
        std::cout << "uniform (box filter)\n";
    else if (integrator_->getPixelSampler().getType() == PixelSampler::Type::REGULAR)
        std::cout << "regular (box filter)\n";
    else if (integrator_->getPixelSampler().getType() == PixelSampler::Type::JITTERED)
        std::cout << "jittered (box filter)\n";

    std::cout << "\n";
}

void SwpathtracerApp::render() {
    integrator_->render();
}

void SwpathtracerApp::saveImageToFile() {
    integrator_->saveImageToFile();
}
