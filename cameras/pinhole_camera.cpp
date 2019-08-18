#include "pinhole_camera.h"

PinholeCamera::PinholeCamera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up,
                             float vertical_fov_degrees, std::unique_ptr<ImageBuffer> image_buffer)
    : Camera::Camera(position, look_at, up, std::move(image_buffer), Type::PINHOLE),
      aspect_(static_cast<float>(image_buffer_->getImageWidth()) / image_buffer_->getImageHeight()),
      vertical_fov_degrees_(vertical_fov_degrees) {
    float a = image_buffer_->getImageWidth() * 0.5f;
    float b = image_buffer_->getImageHeight() * 0.5;
    float view_plane_distance = b / tan(vertical_fov_degrees_ * 0.5f * DEG_TO_RAD);
    local_upper_left_corner_ = glm::vec3(-a, b, -view_plane_distance);
}

Ray PinholeCamera::getRay(float x, float y) const {
    glm::vec3 local_ray_direction = glm::normalize(local_upper_left_corner_ + glm::vec3(x, -y, 0.0f));
    glm::vec3 world_ray_direction = camera_onb_.getLocalToWorldMatrix() * local_ray_direction;
    return Ray(position_, world_ray_direction);
}
