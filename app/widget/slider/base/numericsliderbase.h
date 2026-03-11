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

#ifndef NUMERICSLIDERBASE_H
#define NUMERICSLIDERBASE_H

#include "sliderbase.h"

namespace arcvideo {

class NumericSliderBase : public SliderBase {
    Q_OBJECT

public:
    NumericSliderBase(QWidget* parent = nullptr);

    void SetLadderElementCount(int b) { ladder_element_count_ = b; }

    void SetDragMultiplier(const double& d);

    void SetOffset(const QVariant& v);

    [[nodiscard]] bool IsDragging() const;

protected:
    [[nodiscard]] const QVariant& GetOffset() const { return offset_; }

    [[nodiscard]] virtual QVariant AdjustDragDistanceInternal(const QVariant& start, const double& drag) const;

    void SetMinimumInternal(const QVariant& v);

    void SetMaximumInternal(const QVariant& v);

    [[nodiscard]] virtual bool ValueGreaterThan(const QVariant& lhs, const QVariant& rhs) const;

    [[nodiscard]] virtual bool ValueLessThan(const QVariant& lhs, const QVariant& rhs) const;

    [[nodiscard]] bool CanSetValue() const override;

private:
    [[nodiscard]] bool UsingLadders() const;

    [[nodiscard]] QVariant AdjustValue(const QVariant& value) const override;

    SliderLadder* drag_ladder_ = nullptr;

    int ladder_element_count_;

    bool dragged_;

    bool has_min_;
    QVariant min_value_;

    bool has_max_;
    QVariant max_value_;

    double dragged_diff_;

    QVariant drag_start_value_;

    QVariant offset_;

    double drag_multiplier_;

    bool setting_drag_value_;

    /**
     * @brief An effects slider somewhere is being dragged
     */
    static bool effects_slider_is_being_dragged_;

private slots:
    void LabelPressed();

    void RepositionLadder();

    void LadderDragged(int value, double multiplier);

    void LadderReleased();
};

}  // namespace arcvideo

#endif  // NUMERICSLIDERBASE_H
