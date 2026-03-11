#ifndef JOBTIME_H
#define JOBTIME_H

#include <stdint.h>

#include <QDebug>

namespace arcvideo {

class JobTime {
public:
    JobTime();

    void Acquire();

    [[nodiscard]] uint64_t value() const { return value_; }

    bool operator==(const JobTime& rhs) const { return value_ == rhs.value_; }

    bool operator!=(const JobTime& rhs) const { return value_ != rhs.value_; }

    bool operator<(const JobTime& rhs) const { return value_ < rhs.value_; }

    bool operator>(const JobTime& rhs) const { return value_ > rhs.value_; }

    bool operator<=(const JobTime& rhs) const { return value_ <= rhs.value_; }

    bool operator>=(const JobTime& rhs) const { return value_ >= rhs.value_; }

private:
    uint64_t value_;
};

}  // namespace arcvideo

QDebug operator<<(QDebug debug, const arcvideo::JobTime& r);

Q_DECLARE_METATYPE(arcvideo::JobTime)

#endif  // JOBTIME_H
