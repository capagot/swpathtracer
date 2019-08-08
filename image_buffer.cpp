#include "image_buffer.h"

ImageBuffer::ImageBuffer(unsigned int width, unsigned int height, unsigned int viewport_top, unsigned int viewport_left,
                         unsigned int viewport_width, unsigned int viewport_height, const std::string& file_name)
    : width_(width),
      height_(height),
      viewport_top_(viewport_top),
      viewport_left_(viewport_left),
      viewport_width_(viewport_width),
      viewport_height_(viewport_height),
      file_name_(file_name) {
    pixel_data_.resize(width_, std::vector<glm::vec3>(height_));

    for (unsigned int y = 0; y < height_; ++y)
        for (unsigned int x = 0; x < width_; ++x) pixel_data_[x][y] = glm::vec3(0.0f);
}

const glm::vec3& ImageBuffer::getPixelValue(unsigned int x, unsigned int y) const {
    return pixel_data_[x][y];
}

void ImageBuffer::setPixelValue(unsigned int x, unsigned int y, const glm::vec3& value) {
    pixel_data_[x][y] = value;
}

void ImageBuffer::convertTosRGB() {
    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            pixel_data_[x][y].r = pow(clamp(pixel_data_[x][y].r), 1.0f / 2.2f);
            pixel_data_[x][y].g = pow(clamp(pixel_data_[x][y].g), 1.0f / 2.2f);
            pixel_data_[x][y].b = pow(clamp(pixel_data_[x][y].b), 1.0f / 2.2f);
        }
    }
}

void ImageBuffer::saveToFile() const {
    std::ofstream image_file;
    image_file.open(file_name_.c_str());

    image_file << "P3" << std::endl << width_ << " " << height_ << std::endl << 255 << std::endl;

    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            image_file << static_cast<int>(clamp(pixel_data_[x][y].r) * 255.0f + 0.5f) << " ";
            image_file << static_cast<int>(clamp(pixel_data_[x][y].g) * 255.0f + 0.5f) << " ";
            image_file << static_cast<int>(clamp(pixel_data_[x][y].b) * 255.0f + 0.5f) << " ";
        }
    }

    image_file.close();
}
