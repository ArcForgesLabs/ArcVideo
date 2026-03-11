#include "multicampanel.h"

namespace arcvideo {

#define super TimeBasedPanel

MulticamPanel::MulticamPanel() : super(QStringLiteral("MultiCamPanel")) {
    SetTimeBasedWidget(new MulticamWidget(this));

    Retranslate();
}

void MulticamPanel::Retranslate() {
    super::Retranslate();

    SetTitle(tr("Multi-Cam"));
}

}  // namespace arcvideo
