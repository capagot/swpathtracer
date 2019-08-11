#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "primitive.h"

#include "smooth_triangle.h"  // TODO: remove from here in the future
#include "triangle.h"         // TODO: remove from here in the future

#include "bsdf.h"
#include "global_settings.h"
#include "intersection_record.h"
#include "layered_bsdf.h"
#include "material.h"
#include "no_accel_structure.h"
#include "sbvh.h"

class Scene {
   public:
    Scene(std::unique_ptr<AccelStructure> acceleration_structure, const glm::vec3& background_color);
    ~Scene();
    void addBSDF(std::unique_ptr<BSDF> bsdf);
    void addLayeredBSDF(const LayeredBSDF& layered_bsdf);
    void addEmission(const glm::vec3& emission);
    void addMaterial(const Material& material);
    void addPrimitive(std::unique_ptr<Primitive> primitive);
    void loadFlatMesh(const std::string& filename, long unsigned int material_id);
    void loadSmoothMesh(const std::string& filename, long unsigned int material_id);
    void buildAccelStructure();
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record, std::size_t& num_intersection_tests,
                   std::size_t& num_intersections) const;
    inline std::size_t getNumBSDFs() const {
        return bsdfs_.size();
    }
    inline const BSDF* getBSDFPtr(std::size_t bsdf_id) const {
        return bsdfs_[bsdf_id].get();
    }
    inline std::size_t getNumLayeredBSDFs() const {
        return layered_bsdfs_.size();
    }
    inline const LayeredBSDF& getLayeredBSDF(std::size_t layered_bsdf_id) const {
        return layered_bsdfs_[layered_bsdf_id];
    }
    inline std::size_t getNumEmissions() const {
        return emissions_.size();
    }
    inline const glm::vec3& getEmission(std::size_t emission_id) const {
        return emissions_[emission_id];
    }
    inline std::size_t getNumMaterials() const {
        return materials_.size();
    }
    inline const Material& getMaterial(std::size_t material_id) const {
        return materials_[material_id];
    }
    inline std::size_t getNumPrimitives() const {
        return primitives_.size();
    }
    inline const glm::vec3& getBackgroundColor() const {
        return background_color_;
    }
    inline AccelStructure::Type getAccelStructureType() const {
        return accel_structure_->getType();
    }
    inline const AccelStructure& getAccelStructure() const {
        return *accel_structure_;
    }

    float x_min_ = std::numeric_limits<float>::infinity();
    float x_max_ = -std::numeric_limits<float>::infinity();
    float y_min_ = std::numeric_limits<float>::infinity();
    float y_max_ = -std::numeric_limits<float>::infinity();
    float z_min_ = std::numeric_limits<float>::infinity();
    float z_max_ = -std::numeric_limits<float>::infinity();

   private:
    void loadMesh(const std::string& filename, long unsigned int material_id, int assimp_post_processes);
    void updateSceneExtents(const glm::vec3& vertex);

    std::vector<std::unique_ptr<BSDF>> bsdfs_;
    std::vector<LayeredBSDF> layered_bsdfs_;
    std::vector<glm::vec3> emissions_;
    std::vector<Material> materials_;
    std::vector<std::unique_ptr<Primitive>> primitives_;
    std::unique_ptr<AccelStructure> accel_structure_;
    glm::vec3 background_color_;
};

#endif  // SCENE_H
