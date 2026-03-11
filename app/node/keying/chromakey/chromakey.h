/***
  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team
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

#ifndef CHROMAKEYNODE_H
#define CHROMAKEYNODE_H

#include "node/color/ociobase/ociobase.h"

namespace arcvideo {

class ChromaKeyNode : public OCIOBaseNode {
    Q_OBJECT

public:
    ChromaKeyNode();

    NODE_DEFAULT_FUNCTIONS(ChromaKeyNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Retranslate() override;

    void InputValueChangedEvent(const QString& input, int element) override;

    [[nodiscard]] ShaderCode GetShaderCode(const ShaderRequest& request) const override;
    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void ConfigChanged() override;

    static const QString kColorInput;
    static const QString kInvertInput;
    static const QString kMaskOnlyInput;
    static const QString kUpperToleranceInput;
    static const QString kLowerToleranceInput;
    static const QString kGarbageMatteInput;
    static const QString kCoreMatteInput;
    static const QString kShadowsInput;
    static const QString kHighlightsInput;

private:
    void GenerateProcessor();
};

}  // namespace arcvideo

#endif  // CHROMAKEYNODE_H
