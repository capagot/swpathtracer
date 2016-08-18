#include <random>
#include <iostream>
#include <omp.h>

template< template< class > class DISTRIBUTION = std::uniform_real_distribution, class DATA = float, class ENGINE = std::mt19937 >
class RNG
{
public:

    RNG( DATA min, DATA max ) :
        distribution_{ min, max }
    {
        std::clog << "RNG ctor called...\n";

        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            engines_.push_back( ENGINE( std::random_device{}() + thread_count ) );
    } 

    DATA operator()()
    {
        int thread_id = omp_get_thread_num();
        return distribution_( engines_[thread_id] );
    }

    std::vector< ENGINE > engines_{}; // one instance of the engine is kept for each OpenMP thread to avoid locks.
    DISTRIBUTION< DATA > distribution_;
};
