#ifndef EXPORTADVANCEDVIDEODIALOG_H
#define EXPORTADVANCEDVIDEODIALOG_H

#include <QComboBox>
#include <QDialog>

#include "codec/encoder.h"
#include "widget/slider/integerslider.h"

namespace arcvideo {

class ExportAdvancedVideoDialog : public QDialog {
    Q_OBJECT

public:
    ExportAdvancedVideoDialog(const QList<QString>& pix_fmts, QWidget* parent = nullptr);

    [[nodiscard]] int threads() const { return static_cast<int>(thread_slider_->GetValue()); }

    void set_threads(int t) { thread_slider_->SetValue(t); }

    [[nodiscard]] QString pix_fmt() const { return pixel_format_combobox_->currentText(); }

    void set_pix_fmt(const QString& s) { pixel_format_combobox_->setCurrentText(s); }

    [[nodiscard]] VideoParams::ColorRange yuv_range() const {
        return static_cast<VideoParams::ColorRange>(yuv_color_range_combobox_->currentIndex());
    }

    void set_yuv_range(VideoParams::ColorRange i) { yuv_color_range_combobox_->setCurrentIndex(i); }

private:
    IntegerSlider* thread_slider_ = nullptr;

    QComboBox* pixel_format_combobox_ = nullptr;

    QComboBox* yuv_color_range_combobox_ = nullptr;
};

}  // namespace arcvideo

#endif  // EXPORTADVANCEDVIDEODIALOG_H
