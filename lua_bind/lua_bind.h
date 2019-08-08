#ifndef LUA_BIND_LUA_BIND_H
#define LUA_BIND_LUA_BIND_H

#include <memory>
#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include "bsdf.h"
#include "camera.h"
#include "cook_torrance.h"
#include "global_settings.h"
#include "image_buffer.h"
#include "integrator.h"
#include "lambertian.h"
#include "layered_bsdf.h"
#include "lua.hpp"
#include "material.h"
#include "mesh.h"
#include "object.h"
#include "orthographic_camera.h"
#include "pinhole_camera.h"
#include "pixel_sampler.h"
#include "smooth_conductor.h"
#include "smooth_dielectric.h"
#include "smooth_mesh.h"
#include "smooth_triangle.h"
#include "sphere.h"
#include "triangle.h"

namespace lb {  // lb = 'l'ua 'b'ind
class LuaBind {
   public:
    LuaBind(){};
    ~LuaBind();
    void runScript(const std::string& filename);
    ImageBuffer getImageBuffer() const;
    std::unique_ptr<Camera> getCamera() const;
    std::unique_ptr<PixelSampler> getPixelSampler() const;
    GlobalSettings getGlobalSettings() const;
    std::size_t getNumBSDFs() const;
    std::unique_ptr<BSDF> getBSDFAt(unsigned int id) const;
    std::size_t getNumLayeredBSDFs() const;
    LayeredBSDF getLayeredBSDFAt(unsigned int id) const;
    std::size_t getNumEmissions() const;
    glm::vec3 getEmissionAt(unsigned int id) const;
    std::size_t getNumMaterials() const;
    Material getMaterialAt(unsigned int id) const;
    std::size_t getNumObjects() const;
    std::unique_ptr<Object> getObjectAt(unsigned int id) const;
    std::unique_ptr<Integrator> getIntegrator() const;

   private:
    float getNumberField(const std::string& field_name) const;
    std::string getStringField(const std::string& field_name) const;
    glm::vec3 getVec3Field(const std::string& field_name) const;

    lua_State* lua_state_ = nullptr;
};
}  // namespace lb

#endif  // LUA_BIND_LUA_BIND_H
