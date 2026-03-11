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

#ifndef PROJECT_PANEL_H
#define PROJECT_PANEL_H

#include "footagemanagementpanel.h"
#include "node/project.h"
#include "panel/panel.h"
#include "widget/projectexplorer/projectexplorer.h"

namespace arcvideo {

/**
 * @brief A PanelWidget wrapper around a ProjectExplorer and a ProjectToolbar
 */
class ProjectPanel : public PanelWidget, public FootageManagementPanel {
    Q_OBJECT

public:
    ProjectPanel(const QString& unique_name);

    [[nodiscard]] Project* project() const;
    void set_project(Project* p);

    [[nodiscard]] Folder* get_root() const;

    void set_root(Folder* item);

    [[nodiscard]] QVector<Node*> SelectedItems() const;

    [[nodiscard]] Folder* GetSelectedFolder() const;

    [[nodiscard]] QVector<ViewerOutput*> GetSelectedFootage() const override;

    [[nodiscard]] ProjectViewModel* model() const;

    bool SelectItem(Node* n, bool deselect_all_first = true) { return explorer_->SelectItem(n, deselect_all_first); }

    void SelectAll() override;
    void DeselectAll() override;

    void DeleteSelected() override;

    void RenameSelected() override;

public slots:
    void Edit(Node* item);

signals:
    void ProjectNameChanged();

    void SelectionChanged(const QVector<Node*>& selected);

private:
    void Retranslate() override;

    ProjectExplorer* explorer_ = nullptr;

private slots:
    static void ItemDoubleClickSlot(Node* item);

    void ShowNewMenu();

    void UpdateSubtitle();

    static void SaveConnectedProject();
};

}  // namespace arcvideo

#endif  // PROJECT_PANEL_H
