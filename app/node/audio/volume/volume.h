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

#ifndef VOLUMENODE_H
#define VOLUMENODE_H

#include "node/math/math/mathbase.h"

namespace arcvideo {

class VolumeNode : public MathNodeBase {
    Q_OBJECT

public:
    VolumeNode();

    NODE_DEFAULT_FUNCTIONS(VolumeNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void ProcessSamples(const NodeValueRow& values, const SampleBuffer& input, SampleBuffer& output,
                        int index) const override;

    void Retranslate() override;

    static const QString kSamplesInput;
    static const QString kVolumeInput;
};

}  // namespace arcvideo

#endif  // VOLUMENODE_H
