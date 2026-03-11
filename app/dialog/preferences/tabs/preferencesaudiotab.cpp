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

#include "preferencesaudiotab.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "audio/audiomanager.h"
#include "config/config.h"

namespace arcvideo {

PreferencesAudioTab::PreferencesAudioTab() {
    auto* audio_tab_layout = new QVBoxLayout(this);

    {
        // Backend Layout
        auto* main_layout = new QGridLayout();
        main_layout->setContentsMargins(0, 0, 0, 0);

        int row = 0;

        main_layout->addWidget(new QLabel(tr("Backend:")), row, 0);

        audio_backend_combobox_ = new QComboBox();
        connect(audio_backend_combobox_, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                &PreferencesAudioTab::RefreshDevices);
        main_layout->addWidget(audio_backend_combobox_, row, 1);

        audio_tab_layout->addLayout(main_layout);
    }

    {
        auto* groupbox = new QGroupBox();
        audio_tab_layout->addWidget(groupbox);

        auto* layout = new QVBoxLayout(groupbox);

        int row = 0;

        {
            // Output Group
            auto* output_group = new QGroupBox();
            output_group->setTitle(tr("Output"));
            layout->addWidget(output_group);

            auto* output_layout = new QGridLayout(output_group);

            output_layout->addWidget(new QLabel(tr("Device:")), row, 0);

            audio_output_devices_ = new QComboBox();
            output_layout->addWidget(audio_output_devices_, row, 1);

            row++;

            {
                int output_row = 0;

                auto* output_param_group = new QGroupBox(tr("Advanced"));
                output_layout->addWidget(output_param_group, row, 0, 1, 2);

                auto* output_param_layout = new QGridLayout(output_param_group);

                output_param_layout->addWidget(new QLabel(tr("Sample Rate:")), output_row, 0);

                output_rate_combo_ = new SampleRateComboBox();
                output_rate_combo_->SetSampleRate(ARCVIDEO_CONFIG("AudioOutputSampleRate").toInt());
                output_param_layout->addWidget(output_rate_combo_, output_row, 1);

                output_row++;

                output_param_layout->addWidget(new QLabel(tr("Channel Layout:")), output_row, 0);

                output_ch_layout_combo_ = new ChannelLayoutComboBox();
                output_ch_layout_combo_->SetChannelLayout(ARCVIDEO_CONFIG("AudioOutputChannelLayout").toULongLong());
                output_param_layout->addWidget(output_ch_layout_combo_, output_row, 1);

                output_row++;

                output_param_layout->addWidget(new QLabel(tr("Sample Format:")), output_row, 0);

                output_fmt_combo_ = new SampleFormatComboBox();
                output_fmt_combo_->SetPackedFormats();
                output_fmt_combo_->SetSampleFormat(
                    SampleFormat::from_string(ARCVIDEO_CONFIG("AudioOutputSampleFormat").toString().toStdString()));
                output_param_layout->addWidget(output_fmt_combo_, output_row, 1);
            }
        }

        row = 0;

        {
            auto* input_group = new QGroupBox();
            input_group->setTitle(tr("Input"));
            layout->addWidget(input_group);

            auto* input_layout = new QGridLayout(input_group);

            input_layout->addWidget(new QLabel(tr("Device:")), row, 0);

            audio_input_devices_ = new QComboBox();
            input_layout->addWidget(audio_input_devices_, row, 1);

            row++;

            auto* recording_group = new QGroupBox(tr("Recording"));
            input_layout->addWidget(recording_group, row, 0, 1, 2);

            auto* recording_layout = new QVBoxLayout(recording_group);

            auto* fmt_layout = new QHBoxLayout();
            recording_layout->addLayout(fmt_layout);

            fmt_layout->addWidget(new QLabel(tr("Format:")));

            record_format_combo_ = new ExportFormatComboBox(ExportFormatComboBox::kShowAudioOnly);
            record_format_combo_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            record_format_combo_->SetFormat(
                static_cast<ExportFormat::Format>(ARCVIDEO_CONFIG("AudioRecordingFormat").toInt()));
            fmt_layout->addWidget(record_format_combo_);

            record_options_ = new ExportAudioTab();
            record_options_->SetFormat(record_format_combo_->GetFormat());
            record_options_->SetCodec(static_cast<ExportCodec::Codec>(ARCVIDEO_CONFIG("AudioRecordingCodec").toInt()));
            record_options_->sample_rate_combobox()->SetSampleRate(ARCVIDEO_CONFIG("AudioRecordingSampleRate").toInt());
            record_options_->channel_layout_combobox()->SetChannelLayout(
                ARCVIDEO_CONFIG("AudioRecordingChannelLayout").toULongLong());
            record_options_->bit_rate_slider()->SetValue(ARCVIDEO_CONFIG("AudioRecordingBitRate").toInt());
            record_options_->sample_format_combobox()->SetSampleFormat(
                SampleFormat::from_string(ARCVIDEO_CONFIG("AudioRecordingSampleFormat").toString().toStdString()));
            recording_layout->addWidget(record_options_);

            connect(record_format_combo_, &ExportFormatComboBox::FormatChanged, record_options_,
                    &ExportAudioTab::SetFormat);
        }

        auto* refresh_layout = new QHBoxLayout();
        layout->addLayout(refresh_layout);
        refresh_layout->addStretch();

        refresh_devices_btn_ = new QPushButton(tr("Refresh Devices"));
        refresh_layout->addWidget(refresh_devices_btn_);

        connect(refresh_devices_btn_, &QPushButton::clicked, this, &PreferencesAudioTab::HardRefreshBackends);
    }

    audio_tab_layout->addStretch();

    // Populate lists
    RefreshBackends();
}

void PreferencesAudioTab::Accept(MultiUndoCommand* command) {
    Q_UNUSED(command)

    // Get device indexes
    auto output_device = audio_output_devices_->currentData().value<PaDeviceIndex>();
    auto input_device = audio_input_devices_->currentData().value<PaDeviceIndex>();

    // Get device names, which seem to be the closest thing we have to a "unique identifier" for them
    ARCVIDEO_CONFIG("AudioOutput") = audio_output_devices_->currentText();
    ARCVIDEO_CONFIG("AudioInput") = audio_input_devices_->currentText();

    // Set devices to be used from now on
    AudioManager::instance()->SetOutputDevice(output_device);
    AudioManager::instance()->SetInputDevice(input_device);

    ARCVIDEO_CONFIG("AudioOutputSampleRate") = output_rate_combo_->GetSampleRate();
    ARCVIDEO_CONFIG("AudioOutputChannelLayout") = QVariant::fromValue(output_ch_layout_combo_->GetChannelLayout());
    ARCVIDEO_CONFIG("AudioOutputSampleFormat") =
        QString::fromStdString(output_fmt_combo_->GetSampleFormat().to_string());

    ARCVIDEO_CONFIG("AudioRecordingFormat") = record_format_combo_->GetFormat();
    ARCVIDEO_CONFIG("AudioRecordingCodec") = record_options_->GetCodec();
    ARCVIDEO_CONFIG("AudioRecordingSampleRate") = record_options_->sample_rate_combobox()->GetSampleRate();
    ARCVIDEO_CONFIG("AudioRecordingChannelLayout") =
        QVariant::fromValue(record_options_->channel_layout_combobox()->GetChannelLayout());
    ARCVIDEO_CONFIG("AudioRecordingBitRate") = QVariant::fromValue(record_options_->bit_rate_slider()->GetValue());
    ARCVIDEO_CONFIG("AudioRecordingSampleFormat") =
        QString::fromStdString(record_options_->sample_format_combobox()->GetSampleFormat().to_string());

    emit AudioManager::instance() -> OutputParamsChanged();
}

void PreferencesAudioTab::RefreshBackends() {
    audio_backend_combobox_->clear();
    for (PaHostApiIndex i = 0, end = Pa_GetHostApiCount(); i < end; i++) {
        const PaHostApiInfo* info = Pa_GetHostApiInfo(i);

        audio_backend_combobox_->addItem(info->name);
    }

    RefreshDevices();

    AttemptToSetDevicesFromConfig();
}

void PreferencesAudioTab::RefreshDevices() {
    if (audio_backend_combobox_->count() == 0) {
        return;
    }

    PaHostApiIndex host_index = audio_backend_combobox_->currentIndex();
    const PaHostApiInfo* host = Pa_GetHostApiInfo(host_index);

    audio_output_devices_->clear();
    audio_input_devices_->clear();

    for (int i = 0; i < host->deviceCount; i++) {
        PaDeviceIndex device_index = Pa_HostApiDeviceIndexToDeviceIndex(host_index, i);
        const PaDeviceInfo* device = Pa_GetDeviceInfo(device_index);

        if (device->maxOutputChannels) {
            audio_output_devices_->addItem(device->name, device_index);
        }

        if (device->maxInputChannels) {
            audio_input_devices_->addItem(device->name, device_index);
        }
    }
}

void PreferencesAudioTab::HardRefreshBackends() {
    AudioManager::instance()->HardReset();
    RefreshBackends();
}

void PreferencesAudioTab::AttemptToSetDevicesFromConfig() {
    // Load with currently active devices
    PaDeviceIndex current_output_index = AudioManager::instance()->GetOutputDevice();
    PaDeviceIndex current_input_index = AudioManager::instance()->GetInputDevice();

    const PaDeviceInfo *current_output = nullptr, *current_input = nullptr;
    if (current_output_index != paNoDevice) {
        current_output = Pa_GetDeviceInfo(current_output_index);
    }
    if (current_input_index != paNoDevice) {
        current_input = Pa_GetDeviceInfo(current_input_index);
    }

    if (current_output || current_input) {
        PaHostApiIndex host = current_output ? current_output->hostApi : current_input->hostApi;

        // Set backend accordingly
        audio_backend_combobox_->setCurrentIndex(host);

        // Device comboboxes should be populated correctly now
        if (current_output) {
            audio_output_devices_->setCurrentText(current_output->name);
        }

        if (current_input) {
            audio_input_devices_->setCurrentText(current_input->name);
        }
    }
}

}  // namespace arcvideo
