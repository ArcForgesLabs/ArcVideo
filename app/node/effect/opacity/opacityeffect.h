#ifndef OPACITYEFFECT_H
#define OPACITYEFFECT_H

#include "node/group/group.h"

namespace arcvideo {

class OpacityEffect : public Node {
public:
    OpacityEffect();

    NODE_DEFAULT_FUNCTIONS(OpacityEffect)

    [[nodiscard]] QString Name() const override { return tr("Opacity"); }

    [[nodiscard]] QString id() const override { return QStringLiteral("org.arcvideoeditor.ArcVideo.opacity"); }

    [[nodiscard]] QVector<CategoryID> Category() const override { return {kCategoryFilter}; }

    [[nodiscard]] QString Description() const override {
        return tr(
            "Alter a video's opacity.\n\nThis is equivalent to multiplying a video by a number between 0.0 and 1.0.");
    }

    void Retranslate() override;

    [[nodiscard]] ShaderCode GetShaderCode(const ShaderRequest& request) const override;
    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    static const QString kTextureInput;
    static const QString kValueInput;
};

}  // namespace arcvideo

#endif  // OPACITYEFFECT_H
