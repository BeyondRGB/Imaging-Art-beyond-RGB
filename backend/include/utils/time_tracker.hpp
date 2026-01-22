#ifndef TIME_TRACKER_H
#define TIME_TRACKER_H

#include <ctime>
#include <iostream>

/**
 * @brief Helper class for timing various tasks
 *
 * To use
 *      - Create an instance of the TimeTracker
 *      - start_timing()
 *      - preform tasks to be timed
 *      - end_timing()
 *      - request elaped time
 *
 *
 */
class TimeTracker {
  public:
    /**
     * @brief Begin timing
     */
    void start_timeing();

    /**
     * @brief end timing
     */
    void end_timeing();

    /**
     * @brief Get total elapsed time in seconds
     *
     * @return int
     */
    int elapsed_time_sec();

    /**
     * @brief Get total elapsed time in ms
     *
     * @return int
     */
    int elapsed_time_ms();

    /**
     * @brief get total elapsed time in minutes
     *
     * @return float
     */
    float elapsed_time_min();

  private:
    time_t initial;
    time_t end;

    int get_elapsed_sec();
    float sec2min(int sec);
    int sec2ms(int sec);
};

#endif // TIME_TRACKER_H