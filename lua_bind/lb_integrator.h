#ifndef LUA_BIND_LB_INTEGRATOR_H
#define LUA_BIND_LB_INTEGRATOR_H

namespace lb {

struct Integrator {
    enum class Type { NONE, NORMAL_RAYCASTER, DEPTH_RAYCASTER, INT_TEST_COUNT_RAYCASTER, PATHTRACER };
    enum class PathTerminationCriterion { MAX_DEPTH, RUSSIAN_ROULETTE };

    Type type_;
    float prescribed_min_depth_;
    float prescribed_max_depth_;
    long int prescribed_min_int_tests_count_;
    long int prescribed_max_int_tests_count_;
    PathTerminationCriterion path_termination_criterion_;
    int path_length_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_INTEGRATOR_H
