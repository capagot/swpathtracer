#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <cmath>
#include <fstream>
#include <glm/vec3.hpp>
#include <vector>

class ImageBuffer {
   public:
    ImageBuffer(unsigned int width, unsigned int height, unsigned int viewport_top, unsigned int viewport_left,
                unsigned int viewport_width, unsigned int viewport_height, const std::string& file_name);
    unsigned int getImageWidth() const {
        return width_;
    }
    unsigned int getImageHeight() const {
        return height_;
    }
    unsigned int getViewportTop() const {
        return viewport_top_;
    }
    unsigned int getViewportLeft() const {
        return viewport_left_;
    }
    unsigned int getViewportWidth() const {
        return viewport_width_;
    }
    unsigned int getViewportHeight() const {
        return viewport_height_;
    }
    const glm::vec3& getPixelValue(unsigned int x, unsigned int y) const;
    void setPixelValue(unsigned int x, unsigned int y, const glm::vec3& value);
    void convertTosRGB();
    void saveToFile() const;

   private:
    float clamp(float x) const {
        return std::min(std::max(x, 0.0f), 1.0f);
    }

    unsigned int width_;
    unsigned int height_;
    unsigned int viewport_top_;
    unsigned int viewport_left_;
    unsigned int viewport_width_;
    unsigned int viewport_height_;
    std::string file_name_;
    std::vector<std::vector<glm::vec3>> pixel_data_;
};

#endif  // IMAGE_BUFFER_H
