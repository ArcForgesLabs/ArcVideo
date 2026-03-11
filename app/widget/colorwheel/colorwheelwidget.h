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

#ifndef COLORWHEELWIDGET_H
#define COLORWHEELWIDGET_H

#include <QOpenGLWidget>

#include "colorswatchwidget.h"

namespace arcvideo {

class ColorWheelWidget : public ColorSwatchWidget {
    Q_OBJECT

public:
    ColorWheelWidget(QWidget* parent = nullptr);

signals:
    void DiameterChanged(int radius);

protected:
    [[nodiscard]] Color GetColorFromScreenPos(const QPoint& p) const override;

    void resizeEvent(QResizeEvent* e) override;

    void paintEvent(QPaintEvent* e) override;

    void SelectedColorChangedEvent(const Color& c, bool external) override;

private:
    [[nodiscard]] int GetDiameter() const;

    [[nodiscard]] qreal GetRadius() const;

    struct Triangle {
        qreal opposite;
        qreal adjacent;
        qreal hypotenuse;
    };

    [[nodiscard]] Triangle GetTriangleFromCoords(const QPoint& center, const QPoint& p) const;
    [[nodiscard]] static Triangle GetTriangleFromCoords(const QPoint& center, qreal y, qreal x);

    [[nodiscard]] Color GetColorFromTriangle(const Triangle& tri) const;
    [[nodiscard]] QPoint GetCoordsFromColor(const Color& c) const;

    QPixmap cached_wheel_;

    float val_;

    bool force_redraw_;
};

}  // namespace arcvideo

#endif  // COLORWHEELWIDGET_H
