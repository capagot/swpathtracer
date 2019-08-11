#include <iostream>

#include "lua_bind.h"

lb::Camera::~Camera() {}

lb::Object::~Object() {}

lb::BSDF::~BSDF() {}

lb::LuaBind::~LuaBind() {
    if (lua_state_) {
        lua_close(lua_state_);
        lua_state_ = nullptr;
    }
}

void lb::LuaBind::runScript(const std::string& filename) {
    lua_state_ = luaL_newstate();
    luaL_openlibs(lua_state_);

    if (luaL_loadfile(lua_state_, filename.c_str())) {
        std::cerr << "Lua load ERROR: [" << lua_state_ << "] " << lua_tostring(lua_state_, -1) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (lua_pcall(lua_state_, 0, 0, 0)) {
        std::cerr << "Lua execution ERROR: [" << lua_state_ << "] " << lua_tostring(lua_state_, -1) << std::endl;
        exit(EXIT_FAILURE);
    }
}

lb::ImageBuffer lb::LuaBind::getImageBuffer() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__IMAGE_BUFFER__");

    unsigned int width = getNumberField("width");
    unsigned int height = getNumberField("height");
    unsigned int viewport_top = getNumberField("viewport_top");
    unsigned int viewport_left = getNumberField("viewport_left");
    unsigned int viewport_width = getNumberField("viewport_width");
    unsigned int viewport_height = getNumberField("viewport_height");

    std::string file_name = getStringField("file_name");
    lb::ImageBuffer lua_image_buffer(width, height, viewport_top, viewport_left, viewport_width, viewport_height,
                                     file_name);

    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return lua_image_buffer;
}

std::unique_ptr<lb::Camera> lb::LuaBind::getCamera() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");  // Lua stack: -1 = table (elements)
    lua_getfield(lua_state_, -1, "__CAMERA__");          // Lua stack: -1 = table (camera), -2 = table (elements)

    glm::vec3 position =
        getVec3Field("position");  // After f. call, Lua stack: -1 = table (objects), -2 = table (elements)
    glm::vec3 look_at = getVec3Field("look_at");
    glm::vec3 up = getVec3Field("up");
    std::string type = getStringField("type");
    std::unique_ptr<lb::Camera> lua_camera;

    if (type == "orthographic") {
        float min_x = getNumberField("min_x");
        float max_x = getNumberField("max_x");
        float min_y = getNumberField("min_y");
        float max_y = getNumberField("max_y");
        lua_camera = std::make_unique<lb::OrthographicCamera>(position, look_at, up, min_x, max_x, min_y, max_y);
    } else if (type == "pinhole") {
        float fov = getNumberField("fov");
        lua_camera = std::make_unique<lb::PinholeCamera>(position, look_at, up, fov);
    }

    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return lua_camera;
}

std::unique_ptr<lb::PixelSampler> lb::LuaBind::getPixelSampler() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__PIXEL_SAMPLER__");

    std::string type = getStringField("type");
    std::size_t spp = getNumberField("spp");
    std::unique_ptr<lb::PixelSampler> lua_pixel_sampler;

    if (type == "uniform")
        lua_pixel_sampler = std::make_unique<lb::PixelSampler>(lb::PixelSampler::Type::UNIFORM, spp);
    else if (type == "regular")
        lua_pixel_sampler = std::make_unique<lb::PixelSampler>(lb::PixelSampler::Type::REGULAR, spp);
    else if (type == "jittered")
        lua_pixel_sampler = std::make_unique<lb::PixelSampler>(lb::PixelSampler::Type::JITTERED, spp);

    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return lua_pixel_sampler;
}

lb::GlobalSettings lb::LuaBind::getGlobalSettings() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__GLOBAL_SETTINGS__");

    glm::vec3 background_color = getVec3Field("background_color");

    // std::string image_filename = getStringField("image_filename");
    lb::GlobalSettings::AccelStructureType accel_structure;
    std::string accel_structure_str = getStringField("acceleration_structure");

    float overlap_threshold = 0.0f;

    if (accel_structure_str == "bvh-sah") {
        accel_structure = lb::GlobalSettings::AccelStructureType::BVH_SAH;
        overlap_threshold = std::numeric_limits<float>::infinity();
    } else if (accel_structure_str == "sbvh-sah") {
        accel_structure = lb::GlobalSettings::AccelStructureType::SBVH_SAH;
        overlap_threshold = getNumberField("overlap_threshold");
    } else
        accel_structure = lb::GlobalSettings::AccelStructureType::NONE;

    // lb::GlobalSettings lua_global_settings(background_color, image_filename, acceleration_structure);
    lb::GlobalSettings lua_global_settings(background_color, accel_structure, overlap_threshold);

    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return lua_global_settings;
}

std::size_t lb::LuaBind::getNumBSDFs() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__BSDF_COUNT__");

    std::size_t bsdf_count = lua_tonumber(lua_state_, -1);
    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return bsdf_count;
}

std::unique_ptr<lb::BSDF> lb::LuaBind::getBSDFAt(unsigned int id) const {
    ++id;  // c++ arrays are zero-based while Lua's are one-based

    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");  // -1 = table (elements)
    lua_getfield(lua_state_, -1, "__BSDFS__");           // -1 = table (bsdfs), -2 = table (elements)
    lua_rawgeti(lua_state_, -1, id);  // -1 = table (bsdf with id), -2 = table (bsdfs), -3 = table (elements)

    std::string type = getStringField("type");
    std::string bsdf_sampler_type_str = getStringField("bsdf_sampler");
    lb::BSDF::SamplerType bsdf_sampler_type;
    if (bsdf_sampler_type_str == "uniform")
        bsdf_sampler_type = lb::BSDF::SamplerType::UNIFORM;
    else if (bsdf_sampler_type_str == "importance")
        bsdf_sampler_type = lb::BSDF::SamplerType::IMPORTANCE;
    else
        bsdf_sampler_type = lb::BSDF::SamplerType::NONE;

    std::unique_ptr<lb::BSDF> lua_bsdf;

    if (type == "lambertian") {
        glm::vec3 kd = getVec3Field("kd");
        lua_bsdf = std::make_unique<lb::Lambertian>(kd, bsdf_sampler_type);
    } else if (type == "smooth_conductor") {
        glm::vec3 reflectance_at_normal_incidence = getVec3Field("reflectance_at_normal_incidence");
        lua_bsdf = std::make_unique<lb::SmoothConductor>(reflectance_at_normal_incidence, bsdf_sampler_type);
    } else if (type == "smooth_dielectric") {
        float ior = getNumberField("ior");
        lua_bsdf = std::make_unique<lb::SmoothDielectric>(ior, bsdf_sampler_type);
    } else if (type == "cook_torrance") {
        float roughness = getNumberField("roughness");
        glm::vec3 reflectance_at_normal_incidence = getVec3Field("reflectance_at_normal_incidence");
        lua_bsdf =
            std::make_unique<lb::CookTorrance>(roughness, reflectance_at_normal_incidence, bsdf_sampler_type);
    }

    lua_pop(lua_state_, 3);
    assert(lua_gettop(lua_state_) == 0);

    return lua_bsdf;
}

std::size_t lb::LuaBind::getNumLayeredBSDFs() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__LAYERED_BSDF_COUNT__");

    std::size_t layered_bsdf_count = lua_tonumber(lua_state_, -1);
    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return layered_bsdf_count;
}

lb::LayeredBSDF lb::LuaBind::getLayeredBSDFAt(unsigned int id) const {
    ++id;  // c++ arrays are zero-based while Lua's are one-based

    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__LAYERED_BSDFS__");
    lua_rawgeti(lua_state_, -1, id);

    lb::LayeredBSDF lua_layered_bsdf;
    unsigned int num_layers = getNumberField("num_layers");

    for (unsigned int i = 1; i <= num_layers; ++i) {
        lua_rawgeti(lua_state_, -1, i);
        int bsdf_id = lua_tonumber(lua_state_, -1);
        lua_layered_bsdf.bsdf_id_list_.push_back(bsdf_id - 1);  // c++ arrays are 0-based
        lua_pop(lua_state_, 1);
    }

    lua_pop(lua_state_, 3);
    assert(lua_gettop(lua_state_) == 0);

    return lua_layered_bsdf;
}

std::size_t lb::LuaBind::getNumEmissions() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__EMISSION_COUNT__");

    std::size_t emission_count = lua_tonumber(lua_state_, -1);
    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return emission_count;
}

glm::vec3 lb::LuaBind::getEmissionAt(unsigned int id) const {
    ++id;  // c++ arrays are zero-based while Lua's are one-based

    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");  // -1 = table (elements)
    lua_getfield(lua_state_, -1, "__EMISSIONS__");       // -1 = table (emissions), -2 = table (elements)
    lua_rawgeti(lua_state_, -1, id);  // -1 = table (emission with id), -2 = table (emissions), -3 = table (elements)

    glm::vec3 emission;

    for (int i = 1; i <= 3; ++i) {
        lua_rawgeti(lua_state_, -1, i);
        emission[i - 1] = lua_tonumber(lua_state_, -1);
        lua_pop(lua_state_, 1);
    }

    lua_pop(lua_state_, 3);
    assert(lua_gettop(lua_state_) == 0);

    return emission;
}

std::size_t lb::LuaBind::getNumMaterials() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__MATERIAL_COUNT__");

    std::size_t material_count = lua_tonumber(lua_state_, -1);
    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return material_count;
}

lb::Material lb::LuaBind::getMaterialAt(unsigned int id) const {
    ++id;  // c++ arrays are 0-based

    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");  // -1 = table (elements)
    lua_getfield(lua_state_, -1, "__MATERIALS__");       // -1 = table (materials), -2 = table (elements)
    lua_rawgeti(lua_state_, -1, id);  // -1 = table (material with id), -2 = table (materials), -3 = table (elements)

    unsigned int bsdf_id = getNumberField("bsdf") - 1;          // c++ arrays are 0-based
    unsigned int emission_id = getNumberField("emission") - 1;  // c++ arrays are 0-based
    lb::Material lua_material(bsdf_id, emission_id);

    lua_pop(lua_state_, 3);
    assert(lua_gettop(lua_state_) == 0);

    return lua_material;
}

std::size_t lb::LuaBind::getNumObjects() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__OBJECT_COUNT__");

    std::size_t object_count = lua_tonumber(lua_state_, -1);
    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return object_count;
}

std::unique_ptr<lb::Object> lb::LuaBind::getObjectAt(unsigned int id) const {
    ++id;  // c++ arrays are zero-based while Lua's are one-based

    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");  // -1 = table (elements)
    lua_getfield(lua_state_, -1, "__OBJECTS__");         // -1 = table (objects), -2 = table (elements)
    lua_rawgeti(lua_state_, -1, id);  // -1 = table (object with id), -2 = table (objects), -3 = table (elements)

    long unsigned int material_id = getNumberField("material") - 1;  // c++ arrays are 0-based
    std::string type = getStringField("type");
    std::unique_ptr<lb::Object> lua_object;

    if (type == "triangle") {
        glm::vec3 v1 = getVec3Field("v1");
        glm::vec3 v2 = getVec3Field("v2");
        glm::vec3 v3 = getVec3Field("v3");
        lua_object = std::make_unique<lb::Triangle>(v1, v2, v3, material_id);
    } else if (type == "smooth_triangle") {
        glm::vec3 v1 = getVec3Field("v1");
        glm::vec3 v2 = getVec3Field("v2");
        glm::vec3 v3 = getVec3Field("v3");
        glm::vec3 n1 = getVec3Field("n1");
        glm::vec3 n2 = getVec3Field("n2");
        glm::vec3 n3 = getVec3Field("n3");
        lua_object = std::make_unique<lb::SmoothTriangle>(v1, v2, v3, n1, n2, n3, material_id);
    } else if (type == "sphere") {
        glm::vec3 center = getVec3Field("center");
        float radius = getNumberField("radius");
        lua_object = std::make_unique<lb::Sphere>(center, radius, material_id);
    } else if (type == "mesh") {
        std::string filename = getStringField("filename");
        lua_object = std::make_unique<lb::Mesh>(filename, material_id);
    } else if (type == "smooth_mesh") {
        std::string filename = getStringField("filename");
        lua_object = std::make_unique<lb::SmoothMesh>(filename, material_id);
    }

    lua_pop(lua_state_, 3);
    assert(lua_gettop(lua_state_) == 0);

    return lua_object;
}

std::unique_ptr<lb::Integrator> lb::LuaBind::getIntegrator() const {
    lua_getglobal(lua_state_, "__RENDERER_ELEMENTS__");
    lua_getfield(lua_state_, -1, "__INTEGRATOR__");

    std::unique_ptr<lb::Integrator> lua_integrator;

    lua_integrator = std::make_unique<lb::Integrator>();
    std::string type = getStringField("type");

    if (type == "ray-caster") {
        std::string ray_caster_type = getStringField("pixel_value");

        if (ray_caster_type == "normal")
            lua_integrator->type_ = Integrator::Type::NORMAL_RAYCASTER;
        else if (ray_caster_type == "depth") {
            lua_integrator->type_ = Integrator::Type::DEPTH_RAYCASTER;
            lua_integrator->prescribed_min_depth_ = getNumberField("minimum_depth");
            lua_integrator->prescribed_max_depth_ = getNumberField("maximum_depth");
        } else if (ray_caster_type == "intersection-test-count") {
            lua_integrator->type_ = Integrator::Type::INT_TEST_COUNT_RAYCASTER;
            lua_integrator->prescribed_min_int_tests_count_ = getNumberField("minimum_count");
            lua_integrator->prescribed_max_int_tests_count_ = getNumberField("maximum_count");
        }
    } else if (type == "path-tracer") {
        lua_integrator->type_ = Integrator::Type::PATHTRACER;
        std::string path_termination_criterion = getStringField("path_termination");

        if (path_termination_criterion == "max-length")
            lua_integrator->path_termination_criterion_ = Integrator::PathTerminationCriterion::MAX_DEPTH;
        else if (path_termination_criterion == "russian-roulette")
            lua_integrator->path_termination_criterion_ = Integrator::PathTerminationCriterion::RUSSIAN_ROULETTE;

        lua_integrator->path_length_ = getNumberField("path_length");
    }

    lua_pop(lua_state_, 2);
    assert(lua_gettop(lua_state_) == 0);

    return lua_integrator;
}

float lb::LuaBind::getNumberField(const std::string& field_name) const {
    float value = 0.0f;
    lua_getfield(lua_state_, -1, field_name.c_str());

    if (lua_isnumber(lua_state_, -1)) value = lua_tonumber(lua_state_, -1);

    lua_pop(lua_state_, 1);

    return value;
}

std::string lb::LuaBind::getStringField(const std::string& field_name) const {
    std::string value = "";
    lua_getfield(lua_state_, -1, field_name.c_str());

    if (lua_isstring(lua_state_, -1)) value = lua_tostring(lua_state_, -1);

    lua_pop(lua_state_, 1);

    return value;
}

glm::vec3 lb::LuaBind::getVec3Field(const std::string& field_name) const {
    glm::vec3 value = glm::vec3(0.0f);
    lua_getfield(lua_state_, -1, field_name.c_str());

    for (int i = 1; i <= 3; ++i) {
        lua_rawgeti(lua_state_, -1, i);
        value[i - 1] = lua_tonumber(lua_state_, -1);
        lua_pop(lua_state_, 1);
    }

    lua_pop(lua_state_, 1);

    return value;
}
