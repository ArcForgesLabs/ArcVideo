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

#ifndef TRACKVIEWITEM_H
#define TRACKVIEWITEM_H

#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#include "node/output/track/track.h"
#include "widget/clickablelabel/clickablelabel.h"
#include "widget/focusablelineedit/focusablelineedit.h"
#include "widget/timelinewidget/view/timelineviewmouseevent.h"

namespace arcvideo {

class TrackViewItem : public QWidget
{
  Q_OBJECT
public:
  TrackViewItem(Track* track,
                QWidget* parent = nullptr);

signals:
  void AboutToDeleteTrack(Track *track);

private:
  QPushButton* CreateMSLButton(const QColor &checked_color) const;

  QStackedWidget* stack_ = nullptr;

  ClickableLabel* label_ = nullptr;
  FocusableLineEdit* line_edit_ = nullptr;

  QPushButton* mute_button_ = nullptr;
  QPushButton* solo_button_ = nullptr;
  QPushButton* lock_button_ = nullptr;

  Track* track_ = nullptr;

private slots:
  void LabelClicked();

  void LineEditConfirmed();

  void LineEditCancelled();

  void UpdateLabel();

  void ShowContextMenu(const QPoint &p);

  void DeleteTrack();

  void DeleteAllEmptyTracks();

  void UpdateMuteButton(bool e);

  void UpdateLockButton(bool e);

};

}

#endif // TRACKVIEWITEM_H
