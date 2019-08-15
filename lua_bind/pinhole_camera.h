#ifndef LUA_BIND_PINHOLE_CAMERA_H
#define LUA_BIND_PINHOLE_CAMERA_H

#include <glm/vec3.hpp>

#include "camera.h"

namespace lb {

struct PinholeCamera : public Camera {
    PinholeCamera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, float fov)
        : Camera(position, look_at, up, Type::PINHOLE), fov_(fov) {}

    float fov_;
};

}  // namespace lb

#endif  // LUA_BIND_PINHOLE_CAMERA_H
