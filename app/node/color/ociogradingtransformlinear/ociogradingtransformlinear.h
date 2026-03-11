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

#ifndef OCIOGRADINGTRANSFORMLINEARNODE_H
#define OCIOGRADINGTRANSFORMLINEARNODE_H

#include "node/color/ociobase/ociobase.h"
#include "render/colorprocessor.h"

namespace arcvideo {

class OCIOGradingTransformLinearNode : public OCIOBaseNode {
    Q_OBJECT

public:
    OCIOGradingTransformLinearNode();

    NODE_DEFAULT_FUNCTIONS(OCIOGradingTransformLinearNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Retranslate() override;
    void InputValueChangedEvent(const QString& input, int element) override;
    void GenerateProcessor();

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    static const QString kContrastInput;
    static const QString kOffsetInput;
    static const QString kExposureInput;
    static const QString kSaturationInput;
    static const QString kPivotInput;
    static const QString kClampBlackEnableInput;
    static const QString kClampBlackInput;
    static const QString kClampWhiteEnableInput;
    static const QString kClampWhiteInput;

protected slots:
    void ConfigChanged() override;

private:
    void SetVec4InputColors(const QString& input);
};

}  // namespace arcvideo

#endif
