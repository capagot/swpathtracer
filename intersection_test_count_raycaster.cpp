#include "intersection_test_count_raycaster.h"

IntersectionTestCountRayCaster::IntersectionTestCountRayCaster(Camera& camera, const Scene& scene,
                                                               long int prescribed_min_int_tests_count,
                                                               long int prescribed_max_int_tests_count)
    : RayCaster(camera, scene, Integrator::Type::INTERSECTION_TEST_COUNT_RAYCASTER),
      prescribed_min_int_tests_count_(prescribed_min_int_tests_count),
      prescribed_max_int_tests_count_(prescribed_max_int_tests_count) {}

float IntersectionTestCountRayCaster::Hue2RGB(float v1, float v2, float h) const {
    if (h < 0.0f) h += 1.0f;
    if (h > 1.0f) h -= 1.0f;
    if ((6.0f * h) < 1.0f) return v1 + (v2 - v1) * 6.0f * h;
    if ((2.0f * h) < 1.0f) return v2;
    if ((3.0f * h) < 2.0f) return v1 + (v2 - v1) * ((2.0f / 3.0f) - h) * 6.0f;

    return v1;
}

void IntersectionTestCountRayCaster::saveImageToFile() {
    std::size_t min_int_tests_count_pp =
        (prescribed_min_int_tests_count_ != -1) ? prescribed_min_int_tests_count_ : min_int_tests_count_pp_;
    std::size_t max_int_tests_count_pp =
        (prescribed_max_int_tests_count_ != -1) ? prescribed_max_int_tests_count_ : max_int_tests_count_pp_;

    unsigned int hit_test_range_pp = max_int_tests_count_pp - min_int_tests_count_pp;

    // intersection test count normalization (everything will be in [0,1])
    for (unsigned int x = 0; x < camera_.getImage().getImageWidth(); ++x)
        for (unsigned int y = 0; y < camera_.getImage().getImageHeight(); ++y) {
            float value;
            if (hit_test_range_pp > 0)
                value = (buffer_[x][y] - min_int_tests_count_pp) / static_cast<float>(hit_test_range_pp);
            else if (max_int_tests_count_pp > 0)
                value = 1.0f;
            else
                value = 0.0f;

            camera_.getImage().setPixelValue(x, y, glm::vec3(value));
        }

    // heat map color determination
    for (unsigned int x = 0; x < camera_.getImage().getImageWidth(); ++x) {
        for (unsigned int y = 0; y < camera_.getImage().getImageHeight(); ++y) {
            float t = camera_.getImage().getPixelValue(x, y)[0];

            glm::vec3 rgb_color(0.0f);
            glm::vec3 hsl_color(0.65f - (t * 0.65f), 1.0f, t * 0.5f);

            float v1;
            float v2;

            // coversion from HSL to RGB
            if (hsl_color[1] == 0.0f)  // s channel
                rgb_color = glm::vec3(static_cast<int>(hsl_color[2] * 255.0f));
            else {
                if (hsl_color[2] < 0.5f)  // l channel
                    v2 = hsl_color[2] * (1.0f + hsl_color[1]);
                else
                    v2 = (hsl_color[2] + hsl_color[1]) - (hsl_color[1] * hsl_color[2]);

                v1 = 2.0f * hsl_color[2] - v2;

                rgb_color = glm::vec3(Hue2RGB(v1, v2, hsl_color[0] + (1.0f / 3.0f)), Hue2RGB(v1, v2, hsl_color[0]),
                                      Hue2RGB(v1, v2, hsl_color[0] - (1.0f / 3.0f)));
            }

            camera_.getImage().setPixelValue(x, y, rgb_color);
        }
    }

    camera_.getImage().saveToFile();
}

long int IntersectionTestCountRayCaster::getHitValue(const IntersectionRecord& intersection_record,
                                                     std::size_t num_intersection_tests,
                                                     std::size_t num_intersections) const {
    (void)intersection_record;
    (void)num_intersections;

    return num_intersection_tests;
}

long int IntersectionTestCountRayCaster::getMissValue(std::size_t num_intersection_tests) const {
    return num_intersection_tests;
}
