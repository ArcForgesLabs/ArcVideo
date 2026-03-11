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

#ifndef TIMEBASEDPANEL_H
#define TIMEBASEDPANEL_H

#include "panel/panel.h"
#include "widget/timebased/timebasedwidget.h"

namespace arcvideo {

class TimeBasedPanel : public PanelWidget {
    Q_OBJECT

public:
    TimeBasedPanel(const QString& object_name);

    ~TimeBasedPanel() override;

    void ConnectViewerNode(ViewerOutput* node);

    void DisconnectViewerNode() { ConnectViewerNode(nullptr); }

    // Get the timebase of this panels widget
    const rational& timebase();

    [[nodiscard]] ViewerOutput* GetConnectedViewer() const { return widget_->GetConnectedNode(); }

    [[nodiscard]] TimeRuler* ruler() const { return widget_->ruler(); }

    void ZoomIn() override;

    void ZoomOut() override;

    void GoToStart() override;

    void PrevFrame() override;

    void NextFrame() override;

    void GoToEnd() override;

    void GoToPrevCut() override;

    void GoToNextCut() override;

    void PlayPause() override;

    void PlayInToOut() override;

    void ShuttleLeft() override;

    void ShuttleStop() override;

    void ShuttleRight() override;

    void SetIn() override;

    void SetOut() override;

    void ResetIn() override;

    void ResetOut() override;

    void ClearInOut() override;

    void SetMarker() override;

    void ToggleShowAll() override;

    void GoToIn() override;

    void GoToOut() override;

    void DeleteSelected() override;

    void CutSelected() override;

    void CopySelected() override;

    void Paste() override;

    [[nodiscard]] TimeBasedWidget* GetTimeBasedWidget() const { return widget_; }

public slots:
    void SetTimebase(const rational& timebase);

signals:
    void PlayPauseRequested();

    void PlayInToOutRequested();

    void ShuttleLeftRequested();

    void ShuttleStopRequested();

    void ShuttleRightRequested();

protected:
    void SetTimeBasedWidget(TimeBasedWidget* widget);

    void Retranslate() override;

    void SetShowAndRaiseOnConnect() { show_and_raise_on_connect_ = true; }

private:
    TimeBasedWidget* widget_ = nullptr;

    bool show_and_raise_on_connect_;

private slots:
    void ConnectedNodeChanged(ViewerOutput* old, ViewerOutput* now);
};

}  // namespace arcvideo

#endif  // TIMEBASEDPANEL_H
