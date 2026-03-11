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

#ifndef MASKDISTORTNODE_H
#define MASKDISTORTNODE_H

#include "node/generator/polygon/polygon.h"

namespace arcvideo {

class MaskDistortNode : public PolygonGenerator {
    Q_OBJECT

public:
    MaskDistortNode();

    NODE_DEFAULT_FUNCTIONS(MaskDistortNode)

    [[nodiscard]] QString Name() const override { return tr("Mask"); }

    [[nodiscard]] QString id() const override { return QStringLiteral("org.arcvideoeditor.ArcVideo.mask"); }

    [[nodiscard]] QVector<CategoryID> Category() const override { return {kCategoryDistort}; }

    [[nodiscard]] QString Description() const override { return tr("Apply a polygonal mask."); }

    [[nodiscard]] ShaderCode GetShaderCode(const ShaderRequest& request) const override;

    void Retranslate() override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    static const QString kInvertInput;
    static const QString kFeatherInput;
};

}  // namespace arcvideo

#endif  // MASKDISTORTNODE_H
