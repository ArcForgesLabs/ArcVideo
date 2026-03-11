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

#ifndef EXPORTVIDEOTAB_H
#define EXPORTVIDEOTAB_H

#include <QCheckBox>
#include <QComboBox>
#include <QWidget>

#include "common/qtutils.h"
#include "dialog/export/codec/av1section.h"
#include "dialog/export/codec/cineformsection.h"
#include "dialog/export/codec/codecstack.h"
#include "dialog/export/codec/h264section.h"
#include "dialog/export/codec/imagesection.h"
#include "node/color/colormanager/colormanager.h"
#include "widget/colorwheel/colorspacechooser.h"
#include "widget/slider/integerslider.h"
#include "widget/standardcombos/standardcombos.h"

namespace arcvideo {

class ExportVideoTab : public QWidget {
    Q_OBJECT

public:
    ExportVideoTab(ColorManager* color_manager, QWidget* parent = nullptr);

    int SetFormat(ExportFormat::Format format);

    [[nodiscard]] bool IsImageSequenceSet() const;
    void SetImageSequence(bool e) const;

    [[nodiscard]] rational GetStillImageTime() const { return image_section_->GetTime(); }

    [[nodiscard]] ExportCodec::Codec GetSelectedCodec() const {
        return static_cast<ExportCodec::Codec>(codec_combobox()->currentData().toInt());
    }

    void SetSelectedCodec(ExportCodec::Codec c) const { QtUtils::SetComboBoxData(codec_combobox(), c); }

    [[nodiscard]] QComboBox* codec_combobox() const { return codec_combobox_; }

    [[nodiscard]] IntegerSlider* width_slider() const { return width_slider_; }

    [[nodiscard]] IntegerSlider* height_slider() const { return height_slider_; }

    [[nodiscard]] QCheckBox* maintain_aspect_checkbox() const { return maintain_aspect_checkbox_; }

    [[nodiscard]] QComboBox* scaling_method_combobox() const { return scaling_method_combobox_; }

    [[nodiscard]] rational GetSelectedFrameRate() const { return frame_rate_combobox_->GetFrameRate(); }

    void SetSelectedFrameRate(const rational& fr) {
        frame_rate_combobox_->SetFrameRate(fr);
        UpdateFrameRate(fr);
    }

    QString CurrentOCIOColorSpace() { return color_space_chooser_->input(); }

    void SetOCIOColorSpace(const QString& s) { color_space_chooser_->set_input(s); }

    [[nodiscard]] CodecSection* GetCodecSection() const {
        return static_cast<CodecSection*>(codec_stack_->currentWidget());
    }

    void SetCodecSection(CodecSection* section) {
        if (section) {
            codec_stack_->setVisible(true);
            codec_stack_->setCurrentWidget(section);
        } else {
            codec_stack_->setVisible(false);
        }
    }

    [[nodiscard]] InterlacedComboBox* interlaced_combobox() const { return interlaced_combobox_; }

    [[nodiscard]] PixelAspectRatioComboBox* pixel_aspect_combobox() const { return pixel_aspect_combobox_; }

    [[nodiscard]] PixelFormatComboBox* pixel_format_field() const { return pixel_format_field_; }

    [[nodiscard]] const int& threads() const { return threads_; }

    void SetThreads(int t) { threads_ = t; }

    [[nodiscard]] const QString& pix_fmt() const { return pix_fmt_; }
    void SetPixFmt(const QString& s) { pix_fmt_ = s; }

    [[nodiscard]] VideoParams::ColorRange color_range() const { return color_range_; }
    void SetColorRange(VideoParams::ColorRange c) { color_range_ = c; }

public slots:
    void VideoCodecChanged();

    void SetTime(const rational& time);

signals:
    void ColorSpaceChanged(const QString& colorspace);

    void ImageSequenceCheckBoxChanged(bool e);

    void TimeChanged(const rational& time);

private:
    QWidget* SetupResolutionSection();
    QWidget* SetupColorSection();
    QWidget* SetupCodecSection();

    QComboBox* codec_combobox_ = nullptr;
    FrameRateComboBox* frame_rate_combobox_ = nullptr;
    QCheckBox* maintain_aspect_checkbox_ = nullptr;
    QComboBox* scaling_method_combobox_ = nullptr;

    CodecStack* codec_stack_ = nullptr;
    ImageSection* image_section_ = nullptr;
    H264Section* h264_section_ = nullptr;
    H264Section* h265_section_ = nullptr;
    AV1Section* av1_section_ = nullptr;
    CineformSection* cineform_section_;

    ColorSpaceChooser* color_space_chooser_ = nullptr;

    IntegerSlider* width_slider_ = nullptr;
    IntegerSlider* height_slider_ = nullptr;

    ColorManager* color_manager_ = nullptr;

    InterlacedComboBox* interlaced_combobox_ = nullptr;
    PixelAspectRatioComboBox* pixel_aspect_combobox_ = nullptr;
    PixelFormatComboBox* pixel_format_field_ = nullptr;

    int threads_;

    QString pix_fmt_;
    VideoParams::ColorRange color_range_;

    ExportFormat::Format format_;

private slots:
    void MaintainAspectRatioChanged(bool val);

    void OpenAdvancedDialog();

    void UpdateFrameRate(rational r);
};

}  // namespace arcvideo

#endif  // EXPORTVIDEOTAB_H
