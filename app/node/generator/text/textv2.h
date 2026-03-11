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

#ifndef TEXTGENERATORV2_H
#define TEXTGENERATORV2_H

#include "node/generator/shape/shapenodebase.h"

namespace arcvideo {

class TextGeneratorV2 : public ShapeNodeBase {
    Q_OBJECT

public:
    TextGeneratorV2();

    NODE_DEFAULT_FUNCTIONS(TextGeneratorV2)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    void Retranslate() override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void GenerateFrame(FramePtr frame, const GenerateJob& job) const override;

    static const QString kTextInput;
    static const QString kHtmlInput;
    static const QString kVAlignInput;
    static const QString kFontInput;
    static const QString kFontSizeInput;
};

}  // namespace arcvideo

#endif  // TEXTGENERATORV2_H
