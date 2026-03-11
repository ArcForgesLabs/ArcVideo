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

#ifndef DECIMALSLIDERBASE_H
#define DECIMALSLIDERBASE_H

#include "numericsliderbase.h"

namespace arcvideo {

class DecimalSliderBase : public NumericSliderBase {
public:
    DecimalSliderBase(QWidget* parent = nullptr);

    [[nodiscard]] int GetDecimalPlaces() const { return decimal_places_; }
    void SetDecimalPlaces(int i);

    [[nodiscard]] bool GetAutoTrimDecimalPlaces() const { return autotrim_decimal_places_; };
    void SetAutoTrimDecimalPlaces(bool e);

    static QString FloatToString(double val, int decimal_places, bool autotrim_decimal_places);

private:
    int decimal_places_;

    bool autotrim_decimal_places_;
};

}  // namespace arcvideo

#endif  // DECIMALSLIDERBASE_H
