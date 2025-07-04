#include <chrono>

class Time {
public:
    Time(void);

    static int timediff_centis(Time start, Time end);

    static int64_t timediff_millis(Time start, Time end);

    static double timediff_secs(Time start, Time end);

private:
    std::chrono::steady_clock::time_point m_time;
};
