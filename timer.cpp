/*
 * timer.cpp
 *
 *  Created on: May 29, 2016
 *      Author: christian
 */

#include "timer.h"

void Timer::start( void )
{
    if( clock_gettime( CLOCK_REALTIME, &time_start_ ) == -1 )
    {
        std::cerr << "Start timer error." << std::endl;
        exit( EXIT_FAILURE );
    }
}

void Timer::stop( void )
{
    if( clock_gettime( CLOCK_REALTIME, &time_end_ ) == -1 )
    {
        std::cerr << "Stop timer error." << std::endl;
        exit( EXIT_FAILURE );
    }

    computeElapsedTime();
}

void Timer::computeElapsedTime( void )
{
    if( ( time_end_.tv_nsec - time_start_.tv_nsec ) < 0 )
    {
        elapsed_time_.tv_sec = time_end_.tv_sec - time_start_.tv_sec - 1;
        elapsed_time_.tv_nsec = 1000000000 + time_end_.tv_nsec - time_start_.tv_nsec;
    }
    else
    {
        elapsed_time_.tv_sec = time_end_.tv_sec - time_start_.tv_sec;
        elapsed_time_.tv_nsec = time_end_.tv_nsec - time_start_.tv_nsec;
    }
}

__time_t Timer::getElapsedSeconds( void ) const
{
    return elapsed_time_.tv_sec;
}

__syscall_slong_t Timer::getElapsedNanoSeconds( void ) const
{
    return elapsed_time_.tv_nsec;
}
