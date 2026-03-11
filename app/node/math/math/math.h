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

#ifndef MATHNODE_H
#define MATHNODE_H

#include "mathbase.h"

namespace arcvideo {

class MathNode : public MathNodeBase {
    Q_OBJECT

public:
    MathNode();

    NODE_DEFAULT_FUNCTIONS(MathNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Retranslate() override;

    [[nodiscard]] ShaderCode GetShaderCode(const ShaderRequest& request) const override;

    [[nodiscard]] Operation GetOperation() const { return static_cast<Operation>(GetStandardValue(kMethodIn).toInt()); }

    void SetOperation(Operation o) { SetStandardValue(kMethodIn, o); }

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void ProcessSamples(const NodeValueRow& values, const SampleBuffer& input, SampleBuffer& output,
                        int index) const override;

    static const QString kMethodIn;
    static const QString kParamAIn;
    static const QString kParamBIn;
    static const QString kParamCIn;
};

}  // namespace arcvideo

#endif  // MATHNODE_H
