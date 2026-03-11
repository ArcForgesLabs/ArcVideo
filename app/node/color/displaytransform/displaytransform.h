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

#ifndef DISPLAYTRANSFORMNODE_H
#define DISPLAYTRANSFORMNODE_H

#include "node/color/ociobase/ociobase.h"
#include "render/colorprocessor.h"

namespace arcvideo {

class DisplayTransformNode : public OCIOBaseNode {
    Q_OBJECT

public:
    DisplayTransformNode();

    NODE_DEFAULT_FUNCTIONS(DisplayTransformNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Retranslate() override;
    void InputValueChangedEvent(const QString& input, int element) override;

    [[nodiscard]] QString GetDisplay() const;
    [[nodiscard]] QString GetView() const;
    [[nodiscard]] ColorProcessor::Direction GetDirection() const;

    static const QString kDisplayInput;
    static const QString kViewInput;
    static const QString kDirectionInput;

protected slots:
    void ConfigChanged() override;

private:
    void GenerateProcessor();

    void UpdateDisplays();

    void UpdateViews();
};

}  // namespace arcvideo

#endif  // DISPLAYTRANSFORMNODE_H
