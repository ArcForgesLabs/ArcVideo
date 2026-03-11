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

#ifndef NODEVIEWITEMCONNECTOR_H
#define NODEVIEWITEMCONNECTOR_H

#include <QGraphicsPolygonItem>

#include "nodeviewcommon.h"

namespace arcvideo {

class NodeViewItemConnector : public QGraphicsPolygonItem {
public:
    NodeViewItemConnector(bool is_output, QGraphicsItem* parent = nullptr);

    void SetFlowDirection(NodeViewCommon::FlowDirection dir);

    [[nodiscard]] bool IsOutput() const { return output_; }

    [[nodiscard]] QPainterPath shape() const override;
    [[nodiscard]] QRectF boundingRect() const override;

private:
    bool output_;
};

}  // namespace arcvideo

#endif  // NODEVIEWITEMCONNECTOR_H
