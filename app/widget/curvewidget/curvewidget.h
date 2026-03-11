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

#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "curveview.h"
#include "widget/nodeparamview/nodeparamviewkeyframecontrol.h"
#include "widget/nodeparamview/nodeparamviewwidgetbridge.h"
#include "widget/nodetreeview/nodetreeview.h"
#include "widget/timebased/timebasedwidget.h"

namespace arcvideo {

class CurveWidget : public TimeBasedWidget, public TimeTargetObject {
    Q_OBJECT

public:
    CurveWidget(QWidget* parent = nullptr);

    const double& GetVerticalScale();
    void SetVerticalScale(const double& vscale);

    void DeleteSelected();

    void SelectAll() { view_->SelectAll(); }

    void DeselectAll() { view_->DeselectAll(); }

    Node* GetSelectedNodeWithID(const QString& id);

    bool CopySelected(bool cut) override;

    bool Paste() override;

public slots:
    void SetNodes(const QVector<Node*>& nodes);

protected:
    void TimebaseChangedEvent(const rational&) override;
    void ScaleChangedEvent(const double&) override;

    void TimeTargetChangedEvent(ViewerOutput* target) override;

    void ConnectedNodeChangeEvent(ViewerOutput* n) override;

    [[nodiscard]] const QVector<KeyframeViewInputConnection*>* GetSnapKeyframes() const override {
        return &view_->GetKeyframeTracks();
    }

    [[nodiscard]] const TimeTargetObject* GetKeyframeTimeTarget() const override { return view_; }

    [[nodiscard]] const std::vector<NodeKeyframe*>* GetSnapIgnoreKeyframes() const override {
        return &view_->GetSelectedKeyframes();
    }

private:
    void SetKeyframeButtonEnabled(bool enable);

    void SetKeyframeButtonChecked(bool checked);

    void SetKeyframeButtonCheckedFromType(NodeKeyframe::Type type);

    void ConnectInput(Node* node, const QString& input, int element);

    void ConnectInputInternal(Node* node, const QString& input, int element);

    QHash<NodeKeyframeTrackReference, QColor> keyframe_colors_;

    NodeTreeView* tree_view_ = nullptr;

    QPushButton* linear_button_ = nullptr;

    QPushButton* bezier_button_ = nullptr;

    QPushButton* hold_button_ = nullptr;

    CurveView* view_ = nullptr;

    NodeParamViewKeyframeControl* key_control_ = nullptr;

    QVector<Node*> nodes_;

    QVector<NodeKeyframeTrackReference> selected_tracks_;

private slots:
    void SelectionChanged();

    void KeyframeTypeButtonTriggered(bool checked);

    void InputSelectionChanged(const NodeKeyframeTrackReference& ref);

    void KeyframeViewDragged(int x, int y);
    void KeyframeViewReleased();
};

}  // namespace arcvideo

#endif  // CURVEWIDGET_H
