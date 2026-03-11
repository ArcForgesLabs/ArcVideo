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

#ifndef NODEPARAMVIEW_H
#define NODEPARAMVIEW_H

#include <QVBoxLayout>
#include <QWidget>

#include "node/group/group.h"
#include "node/node.h"
#include "node/project/serializer/serializer.h"
#include "nodeparamviewcontext.h"
#include "nodeparamviewdockarea.h"
#include "nodeparamviewitem.h"
#include "widget/keyframeview/keyframeview.h"
#include "widget/timebased/timebasedwidget.h"

namespace arcvideo {

class NodeParamView : public TimeBasedWidget {
    Q_OBJECT

public:
    NodeParamView(bool create_keyframe_view, QWidget* parent = nullptr);
    NodeParamView(QWidget* parent = nullptr) : NodeParamView(true, parent) {}

    ~NodeParamView() override;

    void CloseContextsBelongingToProject(Project* p);

    void DeleteSelected();

    void SelectAll() { keyframe_view_->SelectAll(); }

    void DeselectAll() { keyframe_view_->DeselectAll(); }

    void SetSelectedNodes(const QVector<NodeParamViewItem*>& nodes, bool handle_focused_node = true,
                          bool emit_signal = true);
    void SetSelectedNodes(const QVector<Node::ContextPair>& nodes, bool emit_signal = true);

    Node* GetNodeWithID(const QString& id);
    Node* GetNodeWithIDAndIgnoreList(const QString& id, const QVector<Node*>& ignore);

    [[nodiscard]] const QVector<Node*>& GetContexts() const { return contexts_; }

    bool CopySelected(bool cut) override;

    bool Paste() override;
    static bool Paste(QWidget* parent,
                      std::function<QHash<Node*, Node*>(const ProjectSerializer::Result&)> get_existing_map_function);

public slots:
    void SetContexts(const QVector<Node*>& contexts);

    void UpdateElementY();

signals:
    void FocusedNodeChanged(Node* n);

    void SelectedNodesChanged(const QVector<Node::ContextPair>& nodes);

    void RequestViewerToStartEditingText();

protected:
    void resizeEvent(QResizeEvent* event) override;

    void ScaleChangedEvent(const double&) override;
    void TimebaseChangedEvent(const rational&) override;

    void ConnectedNodeChangeEvent(ViewerOutput* n) override;

    [[nodiscard]] const QVector<KeyframeViewInputConnection*>* GetSnapKeyframes() const override {
        return keyframe_view_ ? &keyframe_view_->GetKeyframeTracks() : nullptr;
    }

    [[nodiscard]] const std::vector<NodeKeyframe*>* GetSnapIgnoreKeyframes() const override {
        return keyframe_view_ ? &keyframe_view_->GetSelectedKeyframes() : nullptr;
    }

    [[nodiscard]] const TimeTargetObject* GetKeyframeTimeTarget() const override { return keyframe_view_; }

private:
    void QueueKeyframePositionUpdate();

    void AddContext(Node* context);

    void RemoveContext(Node* context);

    void AddNode(Node* n, Node* ctx, NodeParamViewContext* context);

    static void SortItemsInContext(NodeParamViewContext* context);

    NodeParamViewContext* GetContextItemFromContext(Node* context);

    [[nodiscard]] bool IsGroupMode() const {
        return contexts_.size() == 1 && dynamic_cast<NodeGroup*>(contexts_.first());
    }

    void ToggleSelect(NodeParamViewItem* item);

    QHash<Node*, Node*> GenerateExistingPasteMap(const ProjectSerializer::Result& r);

    KeyframeView* keyframe_view_ = nullptr;

    QVector<NodeParamViewContext*> context_items_;

    QScrollBar* vertical_scrollbar_ = nullptr;

    int last_scroll_val_;

    QScrollArea* param_scroll_area_ = nullptr;

    QWidget* param_widget_container_ = nullptr;

    NodeParamViewDockArea* param_widget_area_ = nullptr;

    QVector<Node*> pinned_nodes_;

    QVector<Node*> active_nodes_;

    NodeParamViewItem* focused_node_ = nullptr;
    QVector<NodeParamViewItem*> selected_nodes_;

    QVector<Node*> contexts_;
    QVector<Node*> current_contexts_;

    bool show_all_nodes_;

private slots:
    void UpdateGlobalScrollBar();

    void PinNode(bool pin);

    // void FocusChanged(QWidget *old, QWidget *now);

    void NodeAddedToContext(Node* n);

    void NodeRemovedFromContext(Node* n);

    void InputCheckBoxChanged(const NodeInput& input, bool e);

    void GroupInputPassthroughAdded(arcvideo::NodeGroup* group, const arcvideo::NodeInput& input);

    void GroupInputPassthroughRemoved(arcvideo::NodeGroup* group, const arcvideo::NodeInput& input);

    void UpdateContexts();

    void ItemAboutToBeRemoved(NodeParamViewItem* item);

    void ItemClicked();

    void SelectNodeFromConnectedLink(Node* node);

    void RequestEditTextInViewer();

    void InputArraySizeChanged(const QString& input, int old_size, int new_size);
};

}  // namespace arcvideo

#endif  // NODEPARAMVIEW_H
