#ifndef LUA_BIND_LB_CAMERA_H
#define LUA_BIND_LB_CAMERA_H

#include <glm/vec3.hpp>

namespace lb {

struct Camera {
    enum class Type { NONE, ORTHOGRAPHIC, PINHOLE };

    Camera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, Type type)
        : position_(position), look_at_(look_at), up_(up), type_(type) {}
    virtual ~Camera() = 0;

    glm::vec3 position_;
    glm::vec3 look_at_;
    glm::vec3 up_;
    Type type_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_CAMERA_H
