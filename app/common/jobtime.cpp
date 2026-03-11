#include "jobtime.h"

#include <QMutex>

namespace arcvideo {

uint64_t job_time_index = 0;
QMutex job_time_mutex;

JobTime::JobTime() {
    Acquire();
}

void JobTime::Acquire() {
    job_time_mutex.lock();

    value_ = job_time_index;
    job_time_index++;

    job_time_mutex.unlock();
}

}  // namespace arcvideo

QDebug operator<<(QDebug debug, const arcvideo::JobTime& r) {
    return debug.space() << r.value();
}
