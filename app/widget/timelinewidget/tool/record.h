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

#ifndef RECORDTIMELINETOOL_H
#define RECORDTIMELINETOOL_H

#include "beam.h"

namespace arcvideo {

class RecordTool : public BeamTool {
public:
    RecordTool(TimelineWidget* parent);

    void MousePress(TimelineViewMouseEvent* event) override;
    void MouseMove(TimelineViewMouseEvent* event) override;
    void MouseRelease(TimelineViewMouseEvent* event) override;

protected:
    void MouseMoveInternal(const rational& cursor_frame, bool outwards);

    TimelineViewGhostItem* ghost_ = nullptr;

    rational drag_start_point_;
};

}  // namespace arcvideo

#endif  // RECORDTOOL_H
