#ifndef CORE_ONB_H
#define CORE_ONB_H

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

class ONB {
   public:
    ONB(const glm::vec3 &v);
    ONB(const glm::vec3 &u, const glm::vec3 &w);
    const glm::mat3x3 &getLocalToWorldMatrix() const {
        return local_to_world_matrix_;
    }
    const glm::mat3x3 &getWorldToLocalMatrix() const {
        return world_to_local_matrix_;
    }

   private:
    void setBasisMatrix();

    glm::vec3 u_;
    glm::vec3 v_;
    glm::vec3 w_;

    glm::mat3x3 local_to_world_matrix_;
    glm::mat3x3 world_to_local_matrix_;
};

#endif  // CORE_ONB_H
