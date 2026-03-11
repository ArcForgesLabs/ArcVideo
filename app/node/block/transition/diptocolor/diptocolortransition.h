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

#ifndef DIPTOCOLORTRANSITION_H
#define DIPTOCOLORTRANSITION_H

#include "node/block/transition/transition.h"

namespace arcvideo {

class DipToColorTransition : public TransitionBlock {
    Q_OBJECT

public:
    DipToColorTransition();

    NODE_DEFAULT_FUNCTIONS(DipToColorTransition)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    [[nodiscard]] ShaderCode GetShaderCode(const ShaderRequest& request) const override;

    void Retranslate() override;

    static const QString kColorInput;

protected:
    void ShaderJobEvent(const NodeValueRow& value, ShaderJob* job) const override;
};

}  // namespace arcvideo

#endif  // DIPTOCOLORTRANSITION_H
