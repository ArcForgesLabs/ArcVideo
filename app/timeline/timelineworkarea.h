/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2022 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#ifndef TIMELINEWORKAREA_H
#define TIMELINEWORKAREA_H

#include <arcvideo/foundation/foundation.h>

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace arcvideo {

using namespace foundation;

class TimelineWorkArea : public QObject {
    Q_OBJECT

public:
    TimelineWorkArea(QObject* parent = nullptr);

    [[nodiscard]] bool enabled() const;
    void set_enabled(bool e);

    [[nodiscard]] const rational& in() const;
    [[nodiscard]] const rational& out() const;
    [[nodiscard]] const rational& length() const;
    [[nodiscard]] const TimeRange& range() const;
    void set_range(const TimeRange& range);

    bool load(QXmlStreamReader* reader);
    void save(QXmlStreamWriter* writer) const;

    static const rational kResetIn;
    static const rational kResetOut;

signals:
    void EnabledChanged(bool e);

    void RangeChanged(const TimeRange& r);

private:
    bool workarea_enabled_;

    TimeRange workarea_range_;
};

}  // namespace arcvideo

#endif  // TIMELINEWORKAREA_H
