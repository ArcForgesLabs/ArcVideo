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

#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include <QString>
#include <list>
#include <vector>

#include "common/define.h"

namespace arcvideo {

class Project;

class UndoCommand {
public:
    UndoCommand();

    virtual ~UndoCommand() = default;

    DISABLE_COPY_MOVE(UndoCommand)

    [[nodiscard]] bool has_prepared() const { return prepared_; }
    void set_prepared(bool e) { prepared_ = true; }

    void redo_now();
    void undo_now();

    void redo_and_set_modified();
    void undo_and_set_modified();

    [[nodiscard]] virtual Project* GetRelevantProject() const = 0;

protected:
    virtual void prepare() {}
    virtual void redo() = 0;
    virtual void undo() = 0;

private:
    bool modified_;

    Project* project_ = nullptr;

    bool prepared_;

    bool done_;
};

class MultiUndoCommand : public UndoCommand {
public:
    MultiUndoCommand() = default;

    [[nodiscard]] Project* GetRelevantProject() const override { return nullptr; }

    void add_child(UndoCommand* command) { children_.push_back(command); }

    [[nodiscard]] int child_count() const { return static_cast<int>(children_.size()); }

    [[nodiscard]] UndoCommand* child(int i) const { return children_[i]; }

protected:
    void redo() override;
    void undo() override;

private:
    std::vector<UndoCommand*> children_;
};

}  // namespace arcvideo

#endif  // UNDOCOMMAND_H
