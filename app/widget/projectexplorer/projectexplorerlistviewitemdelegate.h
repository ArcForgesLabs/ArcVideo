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

#ifndef PROJECTEXPLORERLISTVIEWITEMDELEGATE_H
#define PROJECTEXPLORERLISTVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

#include "common/define.h"

namespace arcvideo {

/**
 * @brief The delegate that's used to draw items when ProjectExplorer is in List view
 */
class ProjectExplorerListViewItemDelegate : public QStyledItemDelegate {
public:
    ProjectExplorerListViewItemDelegate(QObject* parent = nullptr);

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

}  // namespace arcvideo

#endif  // PROJECTEXPLORERLISTVIEWITEMDELEGATE_H
