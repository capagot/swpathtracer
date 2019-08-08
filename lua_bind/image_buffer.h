#ifndef LUA_IMAGE_BUFFER_H
#define LUA_IMAGE_BUFFER_H

namespace lb {
struct ImageBuffer {
    ImageBuffer(unsigned int width, unsigned int height, unsigned int viewport_top, unsigned int viewport_left,
                unsigned int viewport_width, unsigned int viewport_height, const std::string& file_name)
        : width_(width),
          height_(height),
          viewport_top_(viewport_top),
          viewport_left_(viewport_left),
          viewport_width_(viewport_width),
          viewport_height_(viewport_height),
          file_name_(file_name) {}

    unsigned int width_;
    unsigned int height_;
    unsigned int viewport_top_;
    unsigned int viewport_left_;
    unsigned int viewport_width_;
    unsigned int viewport_height_;
    std::string file_name_;
};
}  // namespace lb

#endif  // LUA_IMAGE_BUFFER_H
