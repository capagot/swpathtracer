#ifndef SBVH_H
#define SBVH_H

#include <algorithm>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>

#include "accel_structure.h"
#include "primitive.h"
#include "primitive_ref.h"
#include "sbvh_node.h"
#include "timer.h"

struct StatsSBVH : Stats {
    std::size_t num_nodes_ = 0;
    std::size_t num_inner_nodes_ = 0;
    std::size_t num_leaf_nodes_ = 0;
    std::size_t tree_height_ = 0;
    std::size_t num_object_splits_ = 0;
    std::size_t num_spatial_splits_ = 0;
    std::size_t min_num_primitives_leaf_ = std::numeric_limits<std::size_t>::max();
    std::size_t max_num_primitives_leaf_ = 0;
    std::size_t num_unreferenced_primitives_ = 0;
};

class SBVH : public AccelStructure {
   public:
    struct SplitPlane {
        float offset_ = 0.0f;
        AABB left_aabb_;
        AABB right_aabb_;
        float left_area_;
        float right_area_;
        long int num_left_prim_refs_ = 0;
        long int num_right_prim_refs_ = 0;
        float sah_cost_ = std::numeric_limits<float>::infinity();
    };

    struct Bin {
        AABB aabb_;
        bool has_aabb_ = false;
        std::vector<long int> in_primitive_refs_;
        std::vector<long int> out_primitive_refs_;
    };

    SBVH(float alpha);
    void build();
    const StatsSBVH& getStatistics() const {
        return statistics_;
    }
    void printProgress() {
        std::clog << "\r Building ";

        switch (progress_indicator_) {
            case 0:
            case 2:
            case 4:
                std::clog << "-";
                break;
            case 1:
                std::clog << "/";
                break;
            case 3:
                std::clog << "\\";
                progress_indicator_ = 0;
                break;
        }

        ++progress_indicator_;
        progress_indicator_ %= 4;
    }
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record, long unsigned int& num_intersection_tests,
                   long unsigned int& num_intersections) const;
    void dumpTree(const std::string& file_name) const;
    void collectTreeStatistics();

   private:
    float SAH(std::size_t s1_size, float s1_area, std::size_t s2_size, float s2_area, float root_aabb_area);
    void splitNode(std::unique_ptr<SBVHNode>& node, float root_aabb_area, const std::string& ident,
                   const std::string& split_method, const std::string& child_side);
    bool traverse(const std::unique_ptr<SBVHNode>& node, const Ray& ray, IntersectionRecord& intersection_record,
                  std::size_t& num_intersection_tests, std::size_t& num_intersections) const;

    std::unique_ptr<SBVHNode> root_node_;
    float aabb_intersection_epsilon_;
    float cost_intersec_tri_ = 1.0f;
    float cost_intersec_aabb_ = 1.2f;
    float alpha_;
    int progress_indicator_ = 0;
    std::size_t primitives_inserted_ = 0;
    StatsSBVH statistics_;
};

#endif  // SBVH_H
