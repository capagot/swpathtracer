#ifndef CORE_RAY_H
#define CORE_RAY_H

#include <glm/vec3.hpp>

class Ray {
   public:
    Ray(const glm::vec3& origin, const glm::vec3& direction);
    const glm::vec3& getOrigin() const {
        return origin_;
    }
    const glm::vec3& getDirection() const {
        return direction_;
    }

   private:
    glm::vec3 origin_;
    glm::vec3 direction_;
};

#endif  // CORE_RAY_H
