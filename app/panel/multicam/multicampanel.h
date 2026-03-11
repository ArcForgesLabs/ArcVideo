#ifndef MULTICAMPANEL_H
#define MULTICAMPANEL_H

#include "panel/viewer/viewerbase.h"
#include "widget/multicam/multicamwidget.h"

namespace arcvideo {

class MulticamPanel : public TimeBasedPanel {
    Q_OBJECT

public:
    MulticamPanel();

    [[nodiscard]] MulticamWidget* GetMulticamWidget() const {
        return static_cast<MulticamWidget*>(GetTimeBasedWidget());
    }

protected:
    void Retranslate() override;
};

}  // namespace arcvideo

#endif  // MULTICAMPANEL_H
