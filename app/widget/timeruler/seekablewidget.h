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

#ifndef SEEKABLEWIDGET_H
#define SEEKABLEWIDGET_H

#include <QHBoxLayout>
#include <QScrollBar>

#include "widget/menu/menu.h"
#include "widget/timebased/timebasedviewselectionmanager.h"

namespace arcvideo {

class SeekableWidget : public TimeBasedView {
    Q_OBJECT

public:
    SeekableWidget(QWidget* parent = nullptr);

    [[nodiscard]] int GetScroll() const { return horizontalScrollBar()->value(); }

    [[nodiscard]] TimelineMarkerList* GetMarkers() const { return markers_; }
    [[nodiscard]] TimelineWorkArea* GetWorkArea() const { return workarea_; }

    void SetMarkers(TimelineMarkerList* markers);
    void SetWorkArea(TimelineWorkArea* workarea);

    [[nodiscard]] bool IsDraggingPlayhead() const override { return dragging_; }

    [[nodiscard]] bool IsMarkerEditingEnabled() const { return marker_editing_enabled_; }
    void SetMarkerEditingEnabled(bool e) { marker_editing_enabled_ = e; }

    void DeleteSelected();

    bool CopySelected(bool cut);

    bool PasteMarkers();

    void DeselectAllMarkers();

    void SeekToScenePoint(qreal scene);

    [[nodiscard]] bool HasItemsSelected() const { return !selection_manager_.GetSelectedObjects().empty(); }

    [[nodiscard]] const std::vector<TimelineMarker*>& GetSelectedMarkers() const {
        return selection_manager_.GetSelectedObjects();
    }

    void SelectionManagerSelectEvent(void* obj) override;
    void SelectionManagerDeselectEvent(void* obj) override;

    void CatchUpScrollEvent() override;

public slots:
    void SetScroll(int i) { horizontalScrollBar()->setValue(i); }

    void TimebaseChangedEvent(const rational&) override;

signals:
    void DragMoved(int x, int y);

    void DragReleased();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    void focusOutEvent(QFocusEvent* event) override;

    void DrawMarkers(QPainter* p, int marker_bottom = 0);
    void DrawWorkArea(QPainter* p);

    void DrawPlayhead(QPainter* p, int x, int y);

    [[nodiscard]] inline const int& text_height() const { return text_height_; }

    [[nodiscard]] inline const int& playhead_width() const { return playhead_width_; }

    [[nodiscard]] int GetLeftLimit() const;
    [[nodiscard]] int GetRightLimit() const;

protected slots:
    virtual bool ShowContextMenu(const QPoint& p);

private:
    enum ResizeMode { kResizeNone, kResizeIn, kResizeOut };

    bool FindResizeHandle(QMouseEvent* event);

    void ClearResizeHandle();

    void DragResizeHandle(const QPointF& scene_pos);

    void CommitResizeHandle();

    TimelineMarkerList* markers_ = nullptr;
    TimelineWorkArea* workarea_ = nullptr;

    int text_height_;

    int playhead_width_;

    bool dragging_;

    bool ignore_next_focus_out_;

    TimeBasedViewSelectionManager<TimelineMarker> selection_manager_;

    QObject* resize_item_;
    ResizeMode resize_mode_;
    TimeRange resize_item_range_;
    QPointF resize_start_;
    uint32_t resize_snap_mask_;

    int marker_top_;
    int marker_bottom_;

    bool marker_editing_enabled_;

    QPolygon last_playhead_shape_;

private slots:
    void SetMarkerColor(int c);

    void ShowMarkerProperties();
};

}  // namespace arcvideo

#endif  // SEEKABLEWIDGET_H
