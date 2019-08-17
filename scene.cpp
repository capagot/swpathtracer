#include "scene.h"

Scene::Scene(std::unique_ptr<AccelStructure> accel_structure, const glm::vec3& background_color)
    : accel_structure_(std::move(accel_structure)), background_color_(background_color) {
    accel_structure_->attachPrimitiveList(&primitives_);
}

Scene::~Scene() {}

void Scene::addBSDF(std::unique_ptr<BSDF> bsdf) {
    bsdfs_.push_back(std::move(bsdf));
}

void Scene::addLayeredBSDF(const LayeredBSDF& layered_bsdf) {
    layered_bsdfs_.push_back(layered_bsdf);
}

void Scene::addEmission(const glm::vec3& emission) {
    emissions_.push_back(emission);
}

void Scene::addMaterial(const Material& material) {
    materials_.push_back(material);
}

void Scene::addPrimitive(std::unique_ptr<Primitive> primitive) {
    updateSceneExtents(primitive->getAABB().getMin());
    updateSceneExtents(primitive->getAABB().getMax());
    primitives_.push_back(std::move(primitive));
}

void Scene::loadMesh(const std::string& filename, long unsigned int material_id, int assimp_post_processes) {
    Assimp::Importer assimp_importer;
    const aiScene* assimp_scene_ = assimp_importer.ReadFile(filename, assimp_post_processes);

    if (!assimp_scene_)
        throw std::runtime_error(assimp_importer.GetErrorString());

    if (assimp_scene_->HasMeshes()) {
        for (unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++) {
            const aiMesh* mesh_ptr = assimp_scene_->mMeshes[mesh_id];

            for (unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3) {
                const aiVector3D* vertex_ptr = &mesh_ptr->mVertices[vertex_id];

                glm::vec3 v0(vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z);
                glm::vec3 v1(vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z);
                glm::vec3 v2(vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z);

                if (assimp_post_processes & aiProcess_GenSmoothNormals) {
                    const aiVector3D* normal_ptr = &mesh_ptr->mNormals[vertex_id];
                    glm::vec3 n0(normal_ptr[0].x, normal_ptr[0].y, normal_ptr[0].z);
                    glm::vec3 n1(normal_ptr[1].x, normal_ptr[1].y, normal_ptr[1].z);
                    glm::vec3 n2(normal_ptr[2].x, normal_ptr[2].y, normal_ptr[2].z);
                    addPrimitive(std::make_unique<SmoothTriangle>(v0, v1, v2, n0, n1, n2, material_id));
                } else
                    addPrimitive(std::make_unique<Triangle>(v0, v1, v2, material_id));
            }
        }
    }
}

void Scene::loadFlatMesh(const std::string& filename, long unsigned int material_id) {
    int assimp_post_processes = aiProcess_FindDegenerates | aiProcess_Triangulate;
    loadMesh(filename, material_id, assimp_post_processes);
}

void Scene::loadSmoothMesh(const std::string& filename, long unsigned int material_id) {
    int assimp_post_processes = aiProcess_FindDegenerates | aiProcess_Triangulate | aiProcess_GenSmoothNormals;
    loadMesh(filename, material_id, assimp_post_processes);
}

void Scene::updateSceneExtents(const glm::vec3& vertex) {
    x_min_ = std::min(x_min_, vertex.x);
    x_max_ = std::max(x_max_, vertex.x);
    y_min_ = std::min(y_min_, vertex.y);
    y_max_ = std::max(y_max_, vertex.y);
    z_min_ = std::min(z_min_, vertex.z);
    z_max_ = std::max(z_max_, vertex.z);
}

void Scene::buildAccelStructure() {
    accel_structure_->build();
}

bool Scene::intersect(const Ray& ray, IntersectionRecord& intersection_record, std::size_t& num_intersection_tests,
                      std::size_t& num_intersections) const {
    return accel_structure_->intersect(ray, intersection_record, num_intersection_tests, num_intersections);
}
