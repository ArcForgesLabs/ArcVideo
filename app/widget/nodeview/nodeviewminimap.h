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

#ifndef NODEVIEWMINIMAP_H
#define NODEVIEWMINIMAP_H

#include <QGraphicsView>

#include "nodeviewscene.h"

namespace arcvideo {

class NodeViewMiniMap : public QGraphicsView {
    Q_OBJECT

public:
    NodeViewMiniMap(NodeViewScene* scene, QWidget* parent = nullptr);

public slots:
    void SetViewportRect(const QPolygonF& rect);

signals:
    void Resized();

    void MoveToScenePoint(const QPointF& pos);

protected:
    void drawForeground(QPainter* painter, const QRectF& rect) override;

    void resizeEvent(QResizeEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override {}

private slots:
    void SceneChanged(const QRectF& bounding);

    void SetDefaultSize();

private:
    bool MouseInsideResizeTriangle(QMouseEvent* event) const;

    void EmitMoveSignal(QMouseEvent* event);

    int resize_triangle_sz_;

    QPolygonF viewport_rect_;

    bool resizing_;

    QPoint resize_anchor_;
};

}  // namespace arcvideo

#endif  // NODEVIEWMINIMAP_H
