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

#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QMenuBar>

#include "dialog/actionsearch/actionsearch.h"
#include "widget/menu/menu.h"

namespace arcvideo {

class MainWindow;

/**
 * @brief ArcVideo's main menubar attached to its main window.
 *
 * Responsible for creating the menu, connecting signals/slots, and retranslating the items on a language change.
 */
class MainMenu : public QMenuBar
{
  Q_OBJECT
public:
  MainMenu(MainWindow *parent);

protected:
  /**
   * @brief changeEvent
   *
   * Qt changeEvent override to catch a QEvent::LanguageEvent.
   *
   * @param e
   */
  virtual void changeEvent(QEvent* e);

private slots:
  /**
   * @brief A slot for the Tool selection items
   *
   * Assumes a QAction* sender() and its data() is a member of enum Tool::Item. Uses the data() to signal a
   * Tool change throughout the rest of the application.
   */
  void ToolItemTriggered();

  /**
   * @brief Slot triggered just before the File menu shows
   */
  void FileMenuAboutToShow();

  /**
   * @brief Slot triggered just before the Edit menu shows
   */
  void EditMenuAboutToShow();
  void EditMenuAboutToHide();

  /**
   * @brief Slot triggered just before the View menu shows
   */
  void ViewMenuAboutToShow();

  /**
   * @brief Slot triggered just before the Tools menu shows
   */
  void ToolsMenuAboutToShow();

  /**
   * @brief Slot triggered just before the Playback menu shows
   */
  void PlaybackMenuAboutToShow();

  /**
   * @brief Slot triggered just before the Sequence menu shows
   */
  void SequenceMenuAboutToShow();

  /**
   * @brief Slot triggered just before the Window menu shows
   */
  void WindowMenuAboutToShow();

  /**
   * @brief Adds items to open recent menu
   */
  void PopulateOpenRecent();

  void RepopulateOpenRecent();

  /**
   * @brief Clears open recent items when menu closes
   */
  void CloseOpenRecentMenu();

  /**
   * @brief Slot for zooming in
   *
   * Finds the currently focused panel and sends it a "zoom in" signal
   */
  void ZoomInTriggered();

  /**
   * @brief Slot for zooming out
   *
   * Finds the currently focused panel and sends it a "zoom out" signal
   */
  void ZoomOutTriggered();

  void IncreaseTrackHeightTriggered();
  void DecreaseTrackHeightTriggered();

  void GoToStartTriggered();
  void PrevFrameTriggered();

  /**
   * @brief Slot for play/pause
   *
   * Finds the currently focused panel and sends it a "play/pause" signal
   */
  void PlayPauseTriggered();

  void PlayInToOutTriggered();

  void LoopTriggered(bool enabled);

  void NextFrameTriggered();
  void GoToEndTriggered();

  void SelectAllTriggered();
  void DeselectAllTriggered();

  void InsertTriggered();
  void OverwriteTriggered();

  void RippleToInTriggered();
  void RippleToOutTriggered();
  void EditToInTriggered();
  void EditToOutTriggered();

  void NudgeLeftTriggered();
  void NudgeRightTriggered();
  void MoveInToPlayheadTriggered();
  void MoveOutToPlayheadTriggered();

  void ActionSearchTriggered();

  void ShuttleLeftTriggered();
  void ShuttleStopTriggered();
  void ShuttleRightTriggered();

  void GoToPrevCutTriggered();
  void GoToNextCutTriggered();

  void SetMarkerTriggered();

  void FullScreenViewerTriggered();

  void ToggleShowAllTriggered();

  void DeleteInOutTriggered();
  void RippleDeleteInOutTriggered();

  void GoToInTriggered();
  void GoToOutTriggered();

  void OpenRecentItemTriggered();

  void SequenceCacheTriggered();
  void SequenceCacheInOutTriggered();
  void SequenceCacheClearTriggered();

  void HelpFeedbackTriggered();

private:
  /**
   * @brief Set strings based on the current application language.
   */
  void Retranslate();

  Menu* file_menu_ = nullptr;
  Menu* file_new_menu_ = nullptr;
  QAction* file_open_item_ = nullptr;
  Menu* file_open_recent_menu_ = nullptr;
  QAction* file_open_recent_separator_ = nullptr;
  QAction* file_open_recent_clear_item_ = nullptr;
  QAction* file_save_item_ = nullptr;
  QAction* file_save_as_item_ = nullptr;
  QAction* file_revert_item_ = nullptr;
  QAction* file_import_item_ = nullptr;
  Menu* file_export_menu_ = nullptr;
  QAction* file_export_media_item_ = nullptr;
  QAction* file_project_properties_item_ = nullptr;
  QAction* file_exit_item_ = nullptr;

  Menu* edit_menu_ = nullptr;
  QAction* edit_undo_item_ = nullptr;
  QAction* edit_redo_item_ = nullptr;
  QAction* edit_delete2_item_ = nullptr;
  QAction* edit_select_all_item_ = nullptr;
  QAction* edit_deselect_all_item_ = nullptr;
  QAction* edit_insert_item_ = nullptr;
  QAction* edit_overwrite_item_ = nullptr;
  QAction* edit_ripple_to_in_item_ = nullptr;
  QAction* edit_ripple_to_out_item_ = nullptr;
  QAction* edit_edit_to_in_item_ = nullptr;
  QAction* edit_edit_to_out_item_ = nullptr;
  QAction* edit_nudge_left_item_ = nullptr;
  QAction* edit_nudge_right_item_ = nullptr;
  QAction* edit_move_in_to_playhead_item_ = nullptr;
  QAction* edit_move_out_to_playhead_item_ = nullptr;
  QAction* edit_delete_inout_item_ = nullptr;
  QAction* edit_ripple_delete_inout_item_ = nullptr;
  QAction* edit_set_marker_item_ = nullptr;

  Menu* view_menu_ = nullptr;
  QAction* view_zoom_in_item_ = nullptr;
  QAction* view_zoom_out_item_ = nullptr;
  QAction* view_increase_track_height_item_ = nullptr;
  QAction* view_decrease_track_height_item_ = nullptr;
  QAction* view_show_all_item_ = nullptr;
  QAction* view_full_screen_item_ = nullptr;
  QAction* view_full_screen_viewer_item_ = nullptr;

  Menu* playback_menu_ = nullptr;
  QAction* playback_gotostart_item_ = nullptr;
  QAction* playback_prevframe_item_ = nullptr;
  QAction* playback_playpause_item_ = nullptr;
  QAction* playback_playinout_item_ = nullptr;
  QAction* playback_nextframe_item_ = nullptr;
  QAction* playback_gotoend_item_ = nullptr;
  QAction* playback_prevcut_item_ = nullptr;
  QAction* playback_nextcut_item_ = nullptr;
  QAction* playback_gotoin_item_ = nullptr;
  QAction* playback_gotoout_item_ = nullptr;
  QAction* playback_shuttleleft_item_ = nullptr;
  QAction* playback_shuttlestop_item_ = nullptr;
  QAction* playback_shuttleright_item_ = nullptr;
  QAction* playback_loop_item_ = nullptr;

  Menu* sequence_menu_ = nullptr;
  QAction* sequence_cache_item_ = nullptr;
  QAction* sequence_cache_in_to_out_item_ = nullptr;
  QAction* sequence_disk_cache_clear_item_ = nullptr;

  Menu* window_menu_ = nullptr;
  QAction* window_menu_separator_ = nullptr;
  QAction* window_maximize_panel_item_ = nullptr;
  QAction* window_reset_layout_item_ = nullptr;

  Menu* tools_menu_ = nullptr;
  QActionGroup* tools_group_ = nullptr;
  QAction* tools_pointer_item_ = nullptr;
  QAction* tools_trackselect_item_ = nullptr;
  QAction* tools_edit_item_ = nullptr;
  QAction* tools_ripple_item_ = nullptr;
  QAction* tools_rolling_item_ = nullptr;
  QAction* tools_razor_item_ = nullptr;
  QAction* tools_slip_item_ = nullptr;
  QAction* tools_slide_item_ = nullptr;
  QAction* tools_hand_item_ = nullptr;
  QAction* tools_zoom_item_ = nullptr;
  QAction* tools_transition_item_ = nullptr;
  QAction* tools_add_item_ = nullptr;
  QAction* tools_record_item_ = nullptr;
  QAction* tools_snapping_item_ = nullptr;
  QAction* tools_preferences_item_ = nullptr;
  Menu *tools_add_item_menu_;

#ifndef NDEBUG
  QAction* tools_magic_item_ = nullptr;
#endif

  Menu* help_menu_ = nullptr;
  QAction* help_action_search_item_ = nullptr;
  QAction* help_feedback_item_ = nullptr;
  QAction* help_about_item_ = nullptr;

};

}

#endif // MAINMENU_H
