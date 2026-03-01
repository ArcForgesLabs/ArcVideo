#ifndef JOBTIME_H
#define JOBTIME_H

#include <QDebug>
#include <stdint.h>

namespace arcvideo {

class JobTime
{
public:
  JobTime();

  void Acquire();

  uint64_t value() const
  {
    return value_;
  }

  bool operator==(const JobTime &rhs) const
  {
    return value_ == rhs.value_;
  }

  bool operator!=(const JobTime &rhs) const
  {
    return value_ != rhs.value_;
  }

  bool operator<(const JobTime &rhs) const
  {
    return value_ < rhs.value_;
  }

  bool operator>(const JobTime &rhs) const
  {
    return value_ > rhs.value_;
  }

  bool operator<=(const JobTime &rhs) const
  {
    return value_ <= rhs.value_;
  }

  bool operator>=(const JobTime &rhs) const
  {
    return value_ >= rhs.value_;
  }

private:
  uint64_t value_;

};

}

QDebug operator<<(QDebug debug, const arcvideo::JobTime& r);

Q_DECLARE_METATYPE(arcvideo::JobTime)

#endif // JOBTIME_H
