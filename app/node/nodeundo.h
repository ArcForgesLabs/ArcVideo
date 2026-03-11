/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2023 Olive Studios LLC

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

#ifndef NODEUNDO_H
#define NODEUNDO_H

#include <utility>

#include "node/node.h"
#include "node/project.h"
#include "undo/undocommand.h"

namespace arcvideo {

class NodeSetPositionCommand : public UndoCommand {
public:
    NodeSetPositionCommand(Node* node, Node* context, const Node::Position& pos) {
        node_ = node;
        context_ = context;
        pos_ = pos;
    }

    [[nodiscard]] Project* GetRelevantProject() const override { return node_->project(); }

protected:
    void redo() override;

    void undo() override;

private:
    Node* node_ = nullptr;
    Node* context_ = nullptr;
    Node::Position pos_;
    Node::Position old_pos_;
    bool added_;
};

class NodeSetPositionAndDependenciesRecursivelyCommand : public UndoCommand {
public:
    NodeSetPositionAndDependenciesRecursivelyCommand(Node* node, Node* context, const Node::Position& pos)
        : node_(node), context_(context), pos_(pos) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return node_->project(); }

protected:
    void prepare() override;

    void redo() override;

    void undo() override;

private:
    void move_recursively(Node* node, const QPointF& diff);

    Node* node_ = nullptr;
    Node* context_ = nullptr;
    Node::Position pos_;
    QVector<UndoCommand*> commands_;
};

class NodeRemovePositionFromContextCommand : public UndoCommand {
public:
    NodeRemovePositionFromContextCommand(Node* node, Node* context) : node_(node), context_(context) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return node_->project(); }

protected:
    void redo() override;

    void undo() override;

private:
    Node* node_;

    Node* context_;

    Node::Position old_pos_;

    bool contained_;
};

class NodeRemovePositionFromAllContextsCommand : public UndoCommand {
public:
    NodeRemovePositionFromAllContextsCommand(Node* node) : node_(node) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return node_->project(); }

protected:
    void redo() override;

    void undo() override;

private:
    Node* node_;

    std::map<Node*, QPointF> contexts_;
};

class NodeArrayInsertCommand : public UndoCommand {
public:
    NodeArrayInsertCommand(Node* node, QString input, int index)
        : node_(node), input_(std::move(input)), index_(index) {}

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override { node_->InputArrayInsert(input_, index_); }

    void undo() override { node_->InputArrayRemove(input_, index_); }

private:
    Node* node_ = nullptr;
    QString input_;
    int index_;
};

class NodeArrayResizeCommand : public UndoCommand {
public:
    NodeArrayResizeCommand(Node* node, QString input, int size) : node_(node), input_(std::move(input)), size_(size) {}

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override {
        old_size_ = node_->InputArraySize(input_);

        if (old_size_ > size_) {
            // Decreasing in size, disconnect any extraneous edges
            for (int i = size_; i < old_size_; i++) {
                try {
                    NodeInput input(node_, input_, i);
                    Node* output = node_->input_connections().at(input);

                    removed_connections_[input] = output;

                    Node::DisconnectEdge(output, input);
                } catch (std::out_of_range&) {
                }
            }
        }

        node_->ArrayResizeInternal(input_, size_);
    }

    void undo() override {
        for (const auto& removed_connection : removed_connections_) {
            Node::ConnectEdge(removed_connection.second, removed_connection.first);
        }
        removed_connections_.clear();

        node_->ArrayResizeInternal(input_, old_size_);
    }

private:
    Node* node_ = nullptr;
    QString input_;
    int size_;
    int old_size_;

    Node::InputConnections removed_connections_;
};

class NodeArrayRemoveCommand : public UndoCommand {
public:
    NodeArrayRemoveCommand(Node* node, QString input, int index)
        : node_(node), input_(std::move(input)), index_(index) {}

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override {
        // Save immediate data
        if (node_->IsInputKeyframable(input_)) {
            is_keyframing_ = node_->IsInputKeyframing(input_, index_);
        }
        standard_value_ = node_->GetSplitStandardValue(input_, index_);
        keyframes_ = node_->GetKeyframeTracks(input_, index_);
        node_->GetImmediate(input_, index_)->delete_all_keyframes(&memory_manager_);

        node_->InputArrayRemove(input_, index_);
    }

    void undo() override {
        node_->InputArrayInsert(input_, index_);

        // Restore keyframes
        for (const NodeKeyframeTrack& track : keyframes_) {
            for (NodeKeyframe* key : track) {
                key->setParent(node_);
            }
        }
        node_->SetSplitStandardValue(input_, standard_value_, index_);

        if (node_->IsInputKeyframable(input_)) {
            node_->SetInputIsKeyframing(input_, is_keyframing_, index_);
        }
    }

private:
    Node* node_ = nullptr;
    QString input_;
    int index_;

    SplitValue standard_value_;
    bool is_keyframing_;
    QVector<NodeKeyframeTrack> keyframes_;
    QObject memory_manager_;
};

/**
 * @brief An undoable command for disconnecting two NodeParams
 *
 * Can be considered a UndoCommand wrapper for NodeParam::DisonnectEdge()/
 */
class NodeEdgeRemoveCommand : public UndoCommand {
public:
    NodeEdgeRemoveCommand(Node* output, NodeInput input);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    Node* output_;
    NodeInput input_;
};

/**
 * @brief An undoable command for connecting two NodeParams together
 *
 * Can be considered a UndoCommand wrapper for NodeParam::ConnectEdge()/
 */
class NodeEdgeAddCommand : public UndoCommand {
public:
    NodeEdgeAddCommand(Node* output, NodeInput input);

    ~NodeEdgeAddCommand() override;

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    Node* output_;
    NodeInput input_;

    NodeEdgeRemoveCommand* remove_command_ = nullptr;
};

class NodeAddCommand : public UndoCommand {
public:
    NodeAddCommand(Project* graph, Node* node);

    void PushToThread(QThread* thread);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    QObject memory_manager_;

    Project* graph_ = nullptr;
    Node* node_ = nullptr;
};

class NodeRemoveAndDisconnectCommand : public UndoCommand {
public:
    NodeRemoveAndDisconnectCommand(Node* node) : node_(node), graph_(nullptr), command_(nullptr) {}

    ~NodeRemoveAndDisconnectCommand() override { delete command_; }

    [[nodiscard]] Project* GetRelevantProject() const override { return graph_; }

protected:
    void prepare() override;

    void redo() override {
        command_->redo_now();

        graph_ = node_->parent();
        node_->setParent(&memory_manager_);
    }

    void undo() override {
        node_->setParent(graph_);
        graph_ = nullptr;

        command_->undo_now();
    }

private:
    QObject memory_manager_;

    Node* node_ = nullptr;
    Project* graph_ = nullptr;

    MultiUndoCommand* command_ = nullptr;
};

class NodeRemoveWithExclusiveDependenciesAndDisconnect : public UndoCommand {
public:
    NodeRemoveWithExclusiveDependenciesAndDisconnect(Node* node) : node_(node), command_(nullptr) {}

    ~NodeRemoveWithExclusiveDependenciesAndDisconnect() override { delete command_; }

    [[nodiscard]] Project* GetRelevantProject() const override {
        if (command_) {
            return static_cast<const NodeRemoveAndDisconnectCommand*>(command_->child(0))->GetRelevantProject();
        } else {
            return node_->project();
        }
    }

protected:
    void prepare() override {
        command_ = new MultiUndoCommand();

        command_->add_child(new NodeRemoveAndDisconnectCommand(node_));

        // Remove exclusive dependencies
        QVector<Node*> deps = node_->GetExclusiveDependencies();
        for (Node* d : deps) {
            command_->add_child(new NodeRemoveAndDisconnectCommand(d));
        }
    }

    void redo() override { command_->redo_now(); }

    void undo() override { command_->undo_now(); }

private:
    Node* node_ = nullptr;
    MultiUndoCommand* command_ = nullptr;
};

class NodeLinkCommand : public UndoCommand {
public:
    NodeLinkCommand(Node* a, Node* b, bool link) : a_(a), b_(b), link_(link) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return a_->project(); }

protected:
    void redo() override {
        if (link_) {
            done_ = Node::Link(a_, b_);
        } else {
            done_ = Node::Unlink(a_, b_);
        }
    }

    void undo() override {
        if (done_) {
            if (link_) {
                Node::Unlink(a_, b_);
            } else {
                Node::Link(a_, b_);
            }
        }
    }

private:
    Node* a_ = nullptr;
    Node* b_ = nullptr;
    bool link_;
    bool done_;
};

class NodeUnlinkAllCommand : public UndoCommand {
public:
    NodeUnlinkAllCommand(Node* node) : node_(node) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return node_->project(); }

protected:
    void redo() override {
        unlinked_ = node_->links();

        for (Node* link : unlinked_) {
            Node::Unlink(node_, link);
        }
    }

    void undo() override {
        for (Node* link : unlinked_) {
            Node::Link(node_, link);
        }

        unlinked_.clear();
    }

private:
    Node* node_ = nullptr;

    QVector<Node*> unlinked_;
};

class NodeLinkManyCommand : public MultiUndoCommand {
public:
    NodeLinkManyCommand(const QVector<Node*> nodes, bool link) : nodes_(nodes) {
        for (Node* a : nodes_) {
            for (Node* b : nodes_) {
                if (a != b) {
                    add_child(new NodeLinkCommand(a, b, link));
                }
            }
        }
    }

    [[nodiscard]] Project* GetRelevantProject() const override { return nodes_.first()->project(); }

private:
    QVector<Node*> nodes_;
};

class NodeRenameCommand : public UndoCommand {
public:
    NodeRenameCommand() = default;
    NodeRenameCommand(Node* node, const QString& new_name) { AddNode(node, new_name); }

    void AddNode(Node* node, const QString& new_name);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;

    void undo() override;

private:
    QVector<Node*> nodes_;

    QStringList new_labels_;
    QStringList old_labels_;
};

class NodeOverrideColorCommand : public UndoCommand {
public:
    NodeOverrideColorCommand(Node* node, int index);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;

    void undo() override;

private:
    Node* node_;

    int old_index_;

    int new_index_;
};

class NodeViewDeleteCommand : public UndoCommand {
public:
    NodeViewDeleteCommand();

    void AddNode(Node* node, Node* context);

    void AddEdge(Node* output, const NodeInput& input);

    bool ContainsNode(Node* node, Node* context);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;

    void undo() override;

private:
    QVector<Node::ContextPair> nodes_;

    QVector<Node::OutputConnection> edges_;

    struct RemovedNode {
        Node* node;
        Node* context;
        QPointF pos;
        Project* removed_from_graph;
    };

    QVector<RemovedNode> removed_nodes_;

    QObject memory_manager_;
};

class NodeParamSetKeyframingCommand : public UndoCommand {
public:
    NodeParamSetKeyframingCommand(NodeInput input, bool setting);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    NodeInput input_;
    bool new_setting_;
    bool old_setting_;
};

class NodeParamInsertKeyframeCommand : public UndoCommand {
public:
    NodeParamInsertKeyframeCommand(Node* node, NodeKeyframe* keyframe);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    Node* input_ = nullptr;

    NodeKeyframe* keyframe_ = nullptr;

    QObject memory_manager_;
};

class NodeParamRemoveKeyframeCommand : public UndoCommand {
public:
    NodeParamRemoveKeyframeCommand(NodeKeyframe* keyframe);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    Node* input_ = nullptr;

    NodeKeyframe* keyframe_ = nullptr;

    QObject memory_manager_;
};

class NodeParamSetKeyframeTimeCommand : public UndoCommand {
public:
    NodeParamSetKeyframeTimeCommand(NodeKeyframe* key, const rational& time);
    NodeParamSetKeyframeTimeCommand(NodeKeyframe* key, const rational& new_time, const rational& old_time);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    NodeKeyframe* key_ = nullptr;

    rational old_time_;
    rational new_time_;
};

class NodeParamSetKeyframeValueCommand : public UndoCommand {
public:
    NodeParamSetKeyframeValueCommand(NodeKeyframe* key, QVariant value);
    NodeParamSetKeyframeValueCommand(NodeKeyframe* key, QVariant new_value, QVariant old_value);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    NodeKeyframe* key_ = nullptr;

    QVariant old_value_;
    QVariant new_value_;
};

class NodeParamSetStandardValueCommand : public UndoCommand {
public:
    NodeParamSetStandardValueCommand(NodeKeyframeTrackReference input, QVariant value);
    NodeParamSetStandardValueCommand(NodeKeyframeTrackReference input, QVariant new_value, QVariant old_value);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;
    void undo() override;

private:
    NodeKeyframeTrackReference ref_;

    QVariant old_value_;
    QVariant new_value_;
};

class NodeParamSetSplitStandardValueCommand : public UndoCommand {
public:
    NodeParamSetSplitStandardValueCommand(NodeInput input, SplitValue new_value, SplitValue old_value)
        : ref_(std::move(input)), old_value_(std::move(old_value)), new_value_(std::move(new_value)) {}

    NodeParamSetSplitStandardValueCommand(const NodeInput& input, const SplitValue& value)
        : NodeParamSetSplitStandardValueCommand(input, value, input.node()->GetSplitStandardValue(input.input())) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return ref_.node()->project(); }

protected:
    void redo() override { ref_.node()->SetSplitStandardValue(ref_.input(), new_value_, ref_.element()); }

    void undo() override { ref_.node()->SetSplitStandardValue(ref_.input(), old_value_, ref_.element()); }

private:
    NodeInput ref_;

    SplitValue old_value_;
    SplitValue new_value_;
};

class NodeParamArrayAppendCommand : public UndoCommand {
public:
    NodeParamArrayAppendCommand(Node* node, QString input);

    [[nodiscard]] Project* GetRelevantProject() const override;

protected:
    void redo() override;

    void undo() override;

private:
    Node* node_ = nullptr;

    QString input_;
};

class NodeSetValueHintCommand : public UndoCommand {
public:
    NodeSetValueHintCommand(NodeInput input, Node::ValueHint hint)
        : input_(std::move(input)), new_hint_(std::move(hint)) {}

    NodeSetValueHintCommand(Node* node, const QString& input, int element, const Node::ValueHint& hint)
        : NodeSetValueHintCommand(NodeInput(node, input, element), hint) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return input_.node()->project(); }

protected:
    void redo() override;

    void undo() override;

private:
    NodeInput input_;

    Node::ValueHint new_hint_;
    Node::ValueHint old_hint_;
};

class NodeImmediateRemoveAllKeyframesCommand : public UndoCommand {
public:
    NodeImmediateRemoveAllKeyframesCommand(NodeInputImmediate* immediate) : immediate_(immediate) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return nullptr; }

protected:
    void prepare() override;

    void redo() override;

    void undo() override;

private:
    NodeInputImmediate* immediate_;

    QObject memory_manager_;

    QVector<NodeKeyframe*> keys_;
};

}  // namespace arcvideo

#endif  // NODEUNDO_H
