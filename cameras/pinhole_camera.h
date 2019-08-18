#ifndef CAMERAS_PINHOLE_CAMERA_H
#define CAMERAS_PINHOLE_CAMERA_H

#include "camera.h"
#include "ray.h"
#include "core.h"

class PinholeCamera : public Camera {
   public:
    PinholeCamera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up, float vertical_fov_degrees,
                  std::unique_ptr<ImageBuffer> image_buffer);
    Ray getRay(float x, float y) const override;
    inline float getAspect() const {
        return aspect_;
    }
    inline float getVFov() const {
        return vertical_fov_degrees_;
    }

   private:
    float aspect_;
    float vertical_fov_degrees_;
    glm::vec3 local_upper_left_corner_;
};

#endif  // CAMERAS_PINHOLE_CAMERA_H
