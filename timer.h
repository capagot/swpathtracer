#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iomanip>
#include <iostream>

class Timer {
   public:
    Timer(){};
    inline void start() {
        start_time_ = std::chrono::steady_clock::now();
    }

    inline void finish() {
        finish_time_ = std::chrono::steady_clock::now();
    }

    inline long int getElapsedTime() {
        return (std::chrono::duration_cast<std::chrono::microseconds>(finish_time_ - start_time_)).count();
    }

   private:
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point finish_time_;
};

#endif  // PROFILER_H
