#include <random>
#include <iostream>
#include <omp.h>

#ifndef RANDOM_H_
#define RANDOM_H_

template< template< class > class DISTRIBUTION, class DATA, class ENGINE >
class RNG
{
public:

    RNG( DATA min, DATA max ) :
        min_{ min },
        max_{ max },
        distribution_{ min, max }
    {
        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            engines_.push_back( ENGINE( std::random_device{}() + thread_count ) );
    }

    DATA operator()()
    {
#ifdef DEBUG
        return  min_ + ( DATA )rand( ) / ( ( DATA ) RAND_MAX ) * ( max_ - min_ );        
#else
        int thread_id = omp_get_thread_num();
        DATA value = distribution_( engines_[thread_id] );

        while ( value == max_ )
            value = distribution_( engines_[thread_id] );

        return value;
#endif
    }

    DATA min_;

    DATA max_;

    std::vector< ENGINE > engines_{}; // one instance of the engine is kept for each OpenMP thread to avoid locks.

    DISTRIBUTION< DATA > distribution_;
};

#endif // RANDOM_H_
