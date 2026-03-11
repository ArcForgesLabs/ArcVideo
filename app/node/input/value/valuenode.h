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

#ifndef VALUENODE_H
#define VALUENODE_H

#include "node/node.h"

namespace arcvideo {

class ValueNode : public Node {
    Q_OBJECT

public:
    ValueNode();

    NODE_DEFAULT_FUNCTIONS(ValueNode)

    [[nodiscard]] QString Name() const override { return tr("Value"); }

    [[nodiscard]] QString id() const override { return QStringLiteral("org.arcvideoeditor.ArcVideo.value"); }

    [[nodiscard]] QVector<CategoryID> Category() const override { return {kCategoryGenerator}; }

    [[nodiscard]] QString Description() const override {
        return tr("Create a single value that can be connected to various other inputs.");
    }

    static const QString kTypeInput;
    static const QString kValueInput;

    void Retranslate() override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

protected:
    void InputValueChangedEvent(const QString& input, int element) override;

private:
    static const QVector<NodeValue::Type> kSupportedTypes;
};

}  // namespace arcvideo

#endif  // VALUENODE_H
