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

#ifndef COLORSPACECHOOSER_H
#define COLORSPACECHOOSER_H

#include <QComboBox>
#include <QGroupBox>

#include "node/color/colormanager/colormanager.h"

namespace arcvideo {

class ColorSpaceChooser : public QGroupBox {
    Q_OBJECT

public:
    ColorSpaceChooser(ColorManager* color_manager, bool enable_input_field = true, bool enable_display_fields = true,
                      QWidget* parent = nullptr);

    [[nodiscard]] QString input() const;
    [[nodiscard]] ColorTransform output() const;

    void set_input(const QString& s);
    void set_output(const ColorTransform& out);

signals:
    void InputColorSpaceChanged(const QString& input);

    void OutputColorSpaceChanged(const ColorTransform& out);

    void ColorSpaceChanged(const QString& input, const ColorTransform& out);

private slots:
    void UpdateViews(const QString& display);

private:
    ColorManager* color_manager_ = nullptr;

    QComboBox* input_combobox_ = nullptr;

    QComboBox* display_combobox_ = nullptr;

    QComboBox* view_combobox_ = nullptr;

    QComboBox* look_combobox_ = nullptr;

private slots:
    void ComboBoxChanged();
};

}  // namespace arcvideo

#endif  // COLORSPACECHOOSER_H
