#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <time.h>

class Timer
{
public:

    void start( void );

    void stop( void );

    __time_t getElapsedSeconds( void ) const;

    __syscall_slong_t getElapsedNanoSeconds( void ) const;

private:

    void computeElapsedTime( void );

    struct timespec time_start_;

    struct timespec time_end_;

    struct timespec elapsed_time_;
};

#endif /* TIMER_H_ */

