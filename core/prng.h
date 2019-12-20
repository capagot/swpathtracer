#ifndef CORE_PRNG_H
#define CORE_PRNG_H

#ifndef DEBUG
#include <omp.h>
#endif

#include <iostream>
#include <random>

// PRNG where distribution and the returned value type are templated.
template <template <class> class DISTRIBUTION, class VALUE_TYPE, class ENGINE>
class PRNG {
   public:
    PRNG(VALUE_TYPE min, VALUE_TYPE max) : min_(min), max_(max), distribution_(min, max) {
#ifndef DEBUG
        int num_threads = std::max(1, omp_get_max_threads());
#else
        constexpr int num_threads = 1;
#endif

        for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
            auto seed = std::random_device()() + thread_id;
            engines_.push_back(ENGINE(seed));
        }
    }

    VALUE_TYPE operator()() {
#ifndef DEBUG
        int thread_id = omp_get_thread_num();
#else
        constexpr int thread_id = 0;
#endif

        VALUE_TYPE value = distribution_(engines_[thread_id]);

        // Guarantee that 'value' will be in [0, 1)
        while (value == max_) value = distribution_(engines_[thread_id]);

        ++aux_count_; 
        
        return value;
    }

    std::size_t aux_count_ = 0;

    VALUE_TYPE min_;
    VALUE_TYPE max_;
    std::vector<ENGINE> engines_;  // one instance of the engine is kept for each OpenMP thread to avoid locks.
    DISTRIBUTION<VALUE_TYPE> distribution_;
};

#endif  // CORE_PRNG_H
