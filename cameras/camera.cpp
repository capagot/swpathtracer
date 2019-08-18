#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up,
               std::unique_ptr<ImageBuffer> image_buffer, Type type)
    : type_(type),
      position_(position),
      look_at_(look_at),
      up_(up),
      direction_(glm::normalize(look_at - position)),
      camera_onb_(glm::normalize(glm::cross(up_, -direction_)), -direction_),
      image_buffer_(std::move(image_buffer)) {}
