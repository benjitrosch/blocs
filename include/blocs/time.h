#pragma once

#include <chrono>

namespace blocs
{
    /**
     * @brief Time state of a system stage
     * since last scheduled update call.
     */
    namespace time
    {
        /** Delta time (time difference between previous and current frames). */
        extern f64 dt;
        /** Total time elapsed since game started. */
        extern f64 total;
        /** Total number of ticks (milliseconds) since game started. */
        extern u32 ticks;
    }

    /**
     * @brief Internal tool to measure elapsed time of systems.
     */
    class Stopwatch
    {
    private:
        u64  m_startTime;
        u64  m_endTime;
        bool m_stopped;

    public:
        Stopwatch()
            : m_startTime(get_current_time()), m_endTime(get_current_time()),
              m_stopped(false)
        {
            reset();
        }

        ~Stopwatch() { stop(); }

        void reset()
        {
            m_startTime = get_current_time();
            m_stopped   = false;
        }

        void stop()
        {
            m_endTime = get_current_time();
            m_stopped = true;
        }

        inline static u64 get_current_time()
        {
            return std::chrono::time_point_cast<std::chrono::microseconds>(
                       std::chrono::high_resolution_clock::now()
            )
                .time_since_epoch()
                .count();
        }

        u64 get_time_elapsed_micro() { return m_endTime - m_startTime; }

        f64 get_time_elapsed_ms() { return get_time_elapsed_micro() * 0.001; }

        f64 get_current_time_elapsed_ms()
        {
            return (this->get_current_time() - m_startTime) * 0.001;
        }
    };
}
