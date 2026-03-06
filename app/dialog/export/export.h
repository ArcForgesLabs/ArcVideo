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

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QProgressBar>

#include "codec/exportcodec.h"
#include "codec/exportformat.h"
#include "dialog/export/exportformatcombobox.h"
#include "exportaudiotab.h"
#include "exportsubtitlestab.h"
#include "exportvideotab.h"
#include "task/export/export.h"
#include "widget/nodeparamview/nodeparamviewwidgetbridge.h"
#include "widget/viewer/viewer.h"

namespace arcvideo {

class ExportDialog : public QDialog
{
  Q_OBJECT
public:
  ExportDialog(ViewerOutput* viewer_node, bool stills_only_mode, QWidget* parent = nullptr);
  ExportDialog(ViewerOutput* viewer_node, QWidget* parent = nullptr) :
    ExportDialog(viewer_node, false, parent)
  {}

  rational GetSelectedTimebase() const;
  void SetSelectedTimebase(const rational &r);

  EncodingParams GenerateParams() const;
  void SetParams(const EncodingParams &e);

  virtual bool eventFilter(QObject *o, QEvent *e) override;

public slots:
  virtual void done(int r) override;

signals:
  void RequestImportFile(const QString &s);

private:
  void AddPreferencesTab(QWidget *inner_widget, const QString &title);

  void LoadPresets();
  void SetDefaultFilename();

  bool SequenceHasSubtitles() const;

  void SetDefaults();

  ViewerOutput* viewer_node_ = nullptr;

  ExportFormat::Format previously_selected_format_;

  rational GetExportLength() const;
  int64_t GetExportLengthInTimebaseUnits() const;

  enum RangeSelection {
    kRangeEntireSequence,
    kRangeInToOut
  };

  enum AutoPreset {
    kPresetDefault = -1,
    kPresetLastUsed = -2,
  };

  QTabWidget* preferences_tabs_ = nullptr;

  QComboBox* preset_combobox_ = nullptr;
  QComboBox* range_combobox_ = nullptr;
  std::vector<EncodingParams> presets_;

  QCheckBox* video_enabled_ = nullptr;
  QCheckBox* audio_enabled_ = nullptr;
  QCheckBox* subtitles_enabled_ = nullptr;

  ViewerWidget* preview_viewer_ = nullptr;
  QLineEdit* filename_edit_ = nullptr;
  ExportFormatComboBox* format_combobox_ = nullptr;

  ExportVideoTab* video_tab_ = nullptr;
  ExportAudioTab* audio_tab_ = nullptr;
  ExportSubtitlesTab* subtitle_tab_ = nullptr;

  double video_aspect_ratio_;

  ColorManager* color_manager_ = nullptr;

  QWidget* preferences_area_ = nullptr;
  QCheckBox *export_bkg_box_;
  QCheckBox *import_file_after_export_;

  bool stills_only_mode_;

  bool loading_presets_;

private slots:
  void BrowseFilename();

  void FormatChanged(ExportFormat::Format current_format);

  void ResolutionChanged();

  void UpdateViewerDimensions();

  void StartExport();

  void ExportFinished();

  void ImageSequenceCheckBoxChanged(bool e);

  void SavePreset();

  void PresetComboBoxChanged();

};

}

#endif // EXPORTDIALOG_H
