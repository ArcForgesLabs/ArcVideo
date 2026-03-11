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

#ifndef TIMELINEUNDOTRACK_H
#define TIMELINEUNDOTRACK_H

#include "node/output/track/track.h"

namespace arcvideo {

class TrackRippleRemoveBlockCommand : public UndoCommand {
public:
    TrackRippleRemoveBlockCommand(Track* track, Block* block) : track_(track), block_(block) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return track_->project(); }

protected:
    void redo() override {
        before_ = block_->previous();
        track_->RippleRemoveBlock(block_);
    }

    void undo() override { track_->InsertBlockAfter(block_, before_); }

private:
    Track* track_ = nullptr;

    Block* block_ = nullptr;

    Block* before_ = nullptr;
};

class TrackPrependBlockCommand : public UndoCommand {
public:
    TrackPrependBlockCommand(Track* track, Block* block) : track_(track), block_(block) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return track_->project(); }

protected:
    void redo() override { track_->PrependBlock(block_); }

    void undo() override { track_->RippleRemoveBlock(block_); }

private:
    Track* track_ = nullptr;
    Block* block_ = nullptr;
};

class TrackInsertBlockAfterCommand : public UndoCommand {
public:
    TrackInsertBlockAfterCommand(Track* track, Block* block, Block* before)
        : track_(track), block_(block), before_(before) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return block_->project(); }

protected:
    void redo() override { track_->InsertBlockAfter(block_, before_); }

    void undo() override { track_->RippleRemoveBlock(block_); }

private:
    Track* track_ = nullptr;

    Block* block_ = nullptr;

    Block* before_ = nullptr;
};

/**
 * @brief Replaces Block `old` with Block `replace`
 *
 * Both blocks must have equal lengths.
 */
class TrackReplaceBlockCommand : public UndoCommand {
public:
    TrackReplaceBlockCommand(Track* track, Block* old, Block* replace) : track_(track), old_(old), replace_(replace) {}

    [[nodiscard]] Project* GetRelevantProject() const override { return track_->project(); }

protected:
    void redo() override { track_->ReplaceBlock(old_, replace_); }

    void undo() override { track_->ReplaceBlock(replace_, old_); }

private:
    Track* track_ = nullptr;
    Block* old_ = nullptr;
    Block* replace_ = nullptr;
};

}  // namespace arcvideo

#endif  // TIMELINEUNDOTRACK_H
