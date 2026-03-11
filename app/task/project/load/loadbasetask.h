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

#ifndef PROJECTLOADBASETASK_H
#define PROJECTLOADBASETASK_H

#include "node/project.h"
#include "task/task.h"

namespace arcvideo {

class ProjectLoadBaseTask : public Task {
    Q_OBJECT

public:
    ProjectLoadBaseTask(const QString& filename);

    [[nodiscard]] Project* GetLoadedProject() const { return project_; }

    [[nodiscard]] const QString& GetFilename() const { return filename_; }

    [[nodiscard]] const MainWindowLayoutInfo& GetLoadedLayout() const { return layout_; }

protected:
    Project* project_ = nullptr;

    MainWindowLayoutInfo layout_;

private:
    QString filename_;
};

}  // namespace arcvideo

#endif  // LOADBASETASK_H
