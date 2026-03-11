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

#ifndef TIMELINEANDTRACKVIEW_H
#define TIMELINEANDTRACKVIEW_H

#include <QSplitter>
#include <QWidget>

#include "trackview/trackview.h"
#include "view/timelineview.h"

namespace arcvideo {

class TimelineAndTrackView : public QWidget {
public:
    TimelineAndTrackView(Qt::Alignment vertical_alignment = Qt::AlignTop, QWidget* parent = nullptr);

    [[nodiscard]] QSplitter* splitter() const;

    [[nodiscard]] TimelineView* view() const;

    [[nodiscard]] TrackView* track_view() const;

private:
    QSplitter* splitter_ = nullptr;

    TimelineView* view_ = nullptr;

    TrackView* track_view_ = nullptr;

private slots:
    void ViewValueChanged(int v);

    void TracksValueChanged(int v);
};

}  // namespace arcvideo

#endif  // TIMELINEANDTRACKVIEW_H
