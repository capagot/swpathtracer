#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "camera.h"
#include "ray.h"

class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up, float min_x,
                       float max_x, float min_y, float max_y, std::unique_ptr<ImageBuffer> image_buffer);
    Ray getRay(float x, float y) const override;
    inline float getMinX() const {
        return min_x_;
    }
    inline float getMaxX() const {
        return max_x_;
    }
    inline float getMinY() const {
        return min_y_;
    }
    inline float getMaxY() const {
        return max_y_;
    }
    inline float getAspect() const {
        return aspect_;
    }

   private:
    float min_x_;
    float max_x_;
    float min_y_;
    float max_y_;
    float window_width_;
    float window_height_;
    float aspect_;
    glm::vec3 local_upper_left_corner_;
    float pixel_delta_x_;
    float pixel_delta_y_;
    glm::vec3 worl_ray_direction_;
};

#endif  // ORTHOGRAPHIC_CAMERA_H
