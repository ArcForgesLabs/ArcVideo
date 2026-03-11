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

#ifndef PANNODE_H
#define PANNODE_H

#include "node/node.h"

namespace arcvideo {

class PanNode : public Node {
    Q_OBJECT

public:
    PanNode();

    NODE_DEFAULT_FUNCTIONS(PanNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void ProcessSamples(const NodeValueRow& values, const SampleBuffer& input, SampleBuffer& output,
                        int index) const override;

    void Retranslate() override;

    static const QString kSamplesInput;
    static const QString kPanningInput;

private:
    NodeInput* samples_input_ = nullptr;
    NodeInput* panning_input_ = nullptr;
};

}  // namespace arcvideo

#endif  // PANNODE_H
