#ifndef TIME_TRACKER_H
#define TIME_TRACKER_H

#include <iostream>
#include <ctime>

class TimeTracker{
public:
    void start_timeing();
    void end_timeing();

    int elapsed_time_sec();
    int elapsed_time_ms();
    float elapsed_time_min();

private:
    time_t initial;
    time_t end;

    int get_elapsed_sec();
    float sec2min(int sec);
    int sec2ms(int sec);
};

#endif //TIME_TRACKER_H