#ifndef CAMERAS_CAMERA_H
#define CAMERAS_CAMERA_H

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "image_buffer.h"
#include "onb.h"
#include "ray.h"

class Camera {
   public:
    enum class Type { NONE, ORTHOGRAPHIC, PINHOLE };

    Camera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up,
           std::unique_ptr<ImageBuffer> image_buffer, Type camera_type);
    virtual ~Camera(){};
    inline ImageBuffer& getImageBuffer() const {
        return (*image_buffer_.get());
    };
    virtual Ray getRay(float x, float y) const = 0;
    inline Type getType() const {
        return type_;
    }
    inline const glm::vec3& getPosition() const {
        return position_;
    }
    inline const glm::vec3& getLookAt() const {
        return look_at_;
    }
    inline const glm::vec3& getUp() const {
        return up_;
    }
    inline const glm::vec3& getDirection() const {
        return direction_;
    }

   protected:
    Type type_;
    glm::vec3 position_;
    glm::vec3 look_at_;
    glm::vec3 up_;
    glm::vec3 direction_;
    ONB camera_onb_;
    std::unique_ptr<ImageBuffer> image_buffer_;
};

#endif  // CAMERAS_CAMERA_H
