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

#ifndef TIMEREMAPNODE_H
#define TIMEREMAPNODE_H

#include "node/node.h"

namespace arcvideo {

class TimeRemapNode : public Node {
    Q_OBJECT

public:
    TimeRemapNode();

    NODE_DEFAULT_FUNCTIONS(TimeRemapNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    [[nodiscard]] TimeRange InputTimeAdjustment(const QString& input, int element, const TimeRange& input_time,
                                                bool clamp) const override;
    [[nodiscard]] TimeRange OutputTimeAdjustment(const QString& input, int element,
                                                 const TimeRange& input_time) const override;

    void Retranslate() override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    static const QString kTimeInput;
    static const QString kInputInput;

private:
    [[nodiscard]] rational GetRemappedTime(const rational& input) const;
};

}  // namespace arcvideo

#endif  // TIMEREMAPNODE_H
