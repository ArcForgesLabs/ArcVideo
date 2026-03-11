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

#ifndef GAPBLOCK_H
#define GAPBLOCK_H

#include "node/block/block.h"

namespace arcvideo {

/**
 * @brief Node that represents nothing in its respective track for a certain period of time
 */
class GapBlock : public Block {
    Q_OBJECT

public:
    GapBlock();

    NODE_DEFAULT_FUNCTIONS(GapBlock)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QString Description() const override;
};

}  // namespace arcvideo

#endif  // TIMELINEBLOCK_H
