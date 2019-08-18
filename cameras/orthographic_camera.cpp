#include <iostream>

#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up,
                                       float min_x, float max_x, float min_y, float max_y,
                                       std::unique_ptr<ImageBuffer> image_buffer)
    : Camera::Camera(position, look_at, up, std::move(image_buffer), Type::ORTHOGRAPHIC),
      min_x_(min_x),
      max_x_(max_x),
      min_y_(min_y),
      max_y_(max_y),
      window_width_(max_x_ - min_x_),
      window_height_(max_y_ - min_y_),
      aspect_(static_cast<float>(window_width_) / window_height_),
      local_upper_left_corner_(min_x_, max_y_, 0.0f),
      pixel_delta_x_(window_width_ / image_buffer_->getImageWidth()),
      pixel_delta_y_(window_height_ / image_buffer_->getImageHeight()),
      worl_ray_direction_(camera_onb_.getLocalToWorldMatrix() * glm::vec3(0.0f, 0.0f, -1.0f)) {}

Ray OrthographicCamera::getRay(float x, float y) const {
    glm::vec3 local_ray_origin = local_upper_left_corner_ + glm::vec3(x * pixel_delta_x_, y * -pixel_delta_y_, 0.0f);
    glm::vec3 world_ray_origin = camera_onb_.getLocalToWorldMatrix() * local_ray_origin + position_;
    return Ray(world_ray_origin, worl_ray_direction_);
}
