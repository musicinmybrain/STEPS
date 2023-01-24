/*
 * Minimal utility to get process memory usage.
 *
 */
#include "peak_rss.hpp"

#if defined(__linux__) // Linux

#include <sys/time.h>
#include <sys/resource.h>

#elif defined(__APPLE__) // OSX

#include <unistd.h>
#include <sys/resource.h>
#include <mach/mach.h>

#endif


namespace steps {
namespace util {

/*
 * Extract the peak working set size (rss=resident set size).
 * ref. https://en.wikichip.org/wiki/resident_set_size
 *
 * return value is in bytes
 */

std::size_t peak_rss()
{

#if defined(__linux__) // Linux

    struct rusage rusage;
    if (!getrusage(RUSAGE_SELF, &rusage))
        return static_cast<std::size_t>(rusage.ru_maxrss * 1024);

#elif defined(__APPLE__) // Darwin

    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS)
        return static_cast<std::size_t>(info.resident_size_max);

#endif // Error or unknown OS

    return {};
}


} // namespace util
} // namespace steps