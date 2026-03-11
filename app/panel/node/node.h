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

#ifndef NODEPANEL_H
#define NODEPANEL_H

#include "panel/panel.h"
#include "widget/nodeview/nodewidget.h"

namespace arcvideo {

/**
 * @brief A PanelWidget wrapper around a NodeView
 */
class NodePanel : public PanelWidget {
    Q_OBJECT

public:
    NodePanel();

    [[nodiscard]] NodeWidget* GetNodeWidget() const { return node_widget_; }

    [[nodiscard]] const QVector<Node*>& GetContexts() const { return node_widget_->view()->GetContexts(); }

    [[nodiscard]] bool IsGroupOverlay() const { return node_widget_->view()->IsGroupOverlay(); }

    void SetContexts(const QVector<Node*>& nodes) { node_widget_->SetContexts(nodes); }

    void CloseContextsBelongingToProject(Project* project) {
        node_widget_->view()->CloseContextsBelongingToProject(project);
    }

    void SelectAll() override { node_widget_->view()->SelectAll(); }

    void DeselectAll() override { node_widget_->view()->DeselectAll(); }

    void DeleteSelected() override { node_widget_->view()->DeleteSelected(); }

    void CutSelected() override { node_widget_->view()->CopySelected(true); }

    void CopySelected() override { node_widget_->view()->CopySelected(false); }

    void Paste() override { node_widget_->view()->Paste(); }

    void Duplicate() override { node_widget_->view()->Duplicate(); }

    void SetColorLabel(int index) override { node_widget_->view()->SetColorLabel(index); }

    void ZoomIn() override { node_widget_->view()->ZoomIn(); }

    void ZoomOut() override { node_widget_->view()->ZoomOut(); }

    void RenameSelected() override { node_widget_->view()->LabelSelectedNodes(); }

public slots:
    void Select(const QVector<Node::ContextPair>& p) { node_widget_->view()->Select(p, true); }

signals:
    void NodesSelected(const QVector<Node*>& nodes);

    void NodesDeselected(const QVector<Node*>& nodes);

    void NodeSelectionChanged(const QVector<Node*>& nodes);
    void NodeSelectionChangedWithContexts(const QVector<Node::ContextPair>& nodes);

    void NodeGroupOpened(NodeGroup* group);

    void NodeGroupClosed();

private:
    void Retranslate() override { SetTitle(tr("Node Editor")); }

    NodeWidget* node_widget_;
};

}  // namespace arcvideo

#endif  // NODEPANEL_H
