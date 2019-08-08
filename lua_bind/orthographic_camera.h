#ifndef LUA_BIND_ORTHOGRAPHIC_CAMERA_H
#define LUA_BIND_ORTHOGRAPHIC_CAMERA_H

#include <glm/vec3.hpp>

#include "camera.h"

namespace lb {
struct OrthographicCamera : public Camera {
    OrthographicCamera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, float min_x, float max_x, float min_y,
                       float max_y)
        : Camera(position, look_at, up, Type::ORTHOGRAPHIC),
          min_x_(min_x),
          max_x_(max_x),
          min_y_(min_y),
          max_y_(max_y) {}

    float min_x_;
    float max_x_;
    float min_y_;
    float max_y_;
};
}  // namespace lb

#endif  // LUA_BIND_ORTHOGRAPHIC_CAMERA_H
