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

#ifndef NODEGROUP_H
#define NODEGROUP_H

#include <utility>

#include "node/node.h"

namespace arcvideo {

class NodeGroup : public Node {
    Q_OBJECT

public:
    NodeGroup();

    NODE_DEFAULT_FUNCTIONS(NodeGroup)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Retranslate() override;

    bool LoadCustom(QXmlStreamReader* reader, SerializedData* data) override;
    void SaveCustom(QXmlStreamWriter* writer) const override;
    void PostLoadEvent(SerializedData* data) override;

    QString AddInputPassthrough(const NodeInput& input, const QString& force_id = QString());

    void RemoveInputPassthrough(const NodeInput& input);

    [[nodiscard]] Node* GetOutputPassthrough() const { return output_passthrough_; }

    void SetOutputPassthrough(Node* node);

    using InputPassthrough = QPair<QString, NodeInput>;
    using InputPassthroughs = QVector<InputPassthrough>;
    [[nodiscard]] const InputPassthroughs& GetInputPassthroughs() const { return input_passthroughs_; }

    [[nodiscard]] bool ContainsInputPassthrough(const NodeInput& input) const;

    [[nodiscard]] QString GetInputName(const QString& id) const override;

    static NodeInput ResolveInput(NodeInput input);
    static bool GetInner(NodeInput* input);

    [[nodiscard]] QString GetIDOfPassthrough(const NodeInput& input) const {
        for (auto it = input_passthroughs_.cbegin(); it != input_passthroughs_.cend(); it++) {
            if (it->second == input) {
                return it->first;
            }
        }
        return {};
    }

    [[nodiscard]] NodeInput GetInputFromID(const QString& id) const {
        for (auto it = input_passthroughs_.cbegin(); it != input_passthroughs_.cend(); it++) {
            if (it->first == id) {
                return it->second;
            }
        }
        return {};
    }

signals:
    void InputPassthroughAdded(arcvideo::NodeGroup* group, const arcvideo::NodeInput& input);

    void InputPassthroughRemoved(arcvideo::NodeGroup* group, const arcvideo::NodeInput& input);

    void OutputPassthroughChanged(arcvideo::NodeGroup* group, arcvideo::Node* output);

private:
    InputPassthroughs input_passthroughs_;

    Node* output_passthrough_;
};

class NodeGroupAddInputPassthrough : public UndoCommand {
public:
    NodeGroupAddInputPassthrough(NodeGroup* group, NodeInput input, const QString& force_id = QString())
        : group_(group), input_(std::move(input)), actually_added_(false) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return group_->project(); }

protected:
    void redo() override;

    void undo() override;

private:
    NodeGroup* group_;

    NodeInput input_;

    QString force_id_;

    bool actually_added_;
};

class NodeGroupSetOutputPassthrough : public UndoCommand {
public:
    NodeGroupSetOutputPassthrough(NodeGroup* group, Node* output) : group_(group), new_output_(output) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return group_->project(); }

protected:
    void redo() override;

    void undo() override;

private:
    NodeGroup* group_;

    Node* new_output_;
    Node* old_output_;
};

}  // namespace arcvideo

#endif  // NODEGROUP_H
