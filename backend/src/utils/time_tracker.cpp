#include <utils/time_tracker.hpp>

void TimeTracker::start_timeing() { this->initial = time(0); }

void TimeTracker::end_timeing() { this->end = time(0); }

int TimeTracker::get_elapsed_sec() { return this->end - this->initial; }

int TimeTracker::elapsed_time_sec() {
    int sec = this->get_elapsed_sec();
    std::cout << "Elapesed sec: " << sec << std::endl;
    return sec;
}

int TimeTracker::elapsed_time_ms() {
    int ms = this->sec2ms(this->get_elapsed_sec());
    std::cout << "Elapesed ms: " << ms << std::endl;
    return ms;
}

float TimeTracker::elapsed_time_min() {
    float min = this->sec2min(this->get_elapsed_sec());
    std::cout << "Elapesed min: " << min << std::endl;
    return min;
}

int TimeTracker::sec2ms(int sec) { return sec * 1000; }

float TimeTracker::sec2min(int sec) { return sec / 60.0f; }