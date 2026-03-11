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

#ifndef CLIPBLOCK_H
#define CLIPBLOCK_H

#include "audio/audiovisualwaveform.h"
#include "codec/decoder.h"
#include "node/block/block.h"
#include "node/input/multicam/multicamnode.h"
#include "node/output/track/track.h"

namespace arcvideo {

class ViewerOutput;

/**
 * @brief Node that represents a block of Media
 */
class ClipBlock : public Block {
    Q_OBJECT

public:
    ClipBlock();

    NODE_DEFAULT_FUNCTIONS(ClipBlock)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QString Description() const override;

    void set_length_and_media_out(const rational& length) override;
    void set_length_and_media_in(const rational& length) override;

    [[nodiscard]] Track::Type GetTrackType() const {
        if (track()) {
            return track()->type();
        } else {
            return Track::kNone;
        }
    }

    [[nodiscard]] rational media_in() const;
    void set_media_in(const rational& media_in);

    [[nodiscard]] bool IsAutocaching() const { return GetStandardValue(kAutoCacheInput).toBool(); }
    void SetAutocache(bool e);

    void DiscardCache();

    void InvalidateCache(const TimeRange& range, const QString& from, int element,
                         InvalidateCacheOptions options) override;

    [[nodiscard]] TimeRange InputTimeAdjustment(const QString& input, int element, const TimeRange& input_time,
                                                bool clamp) const override;

    [[nodiscard]] TimeRange OutputTimeAdjustment(const QString& input, int element,
                                                 const TimeRange& input_time) const override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void Retranslate() override;

    void RequestInvalidatedFromConnected(bool force_all = false, const TimeRange& intersect = TimeRange());

    [[nodiscard]] double speed() const { return GetStandardValue(kSpeedInput).toDouble(); }

    [[nodiscard]] bool reverse() const { return GetStandardValue(kReverseInput).toBool(); }

    void set_reverse(bool e) { SetStandardValue(kReverseInput, e); }

    [[nodiscard]] bool maintain_audio_pitch() const { return GetStandardValue(kMaintainAudioPitchInput).toBool(); }

    void set_maintain_audio_pitch(bool e) { SetStandardValue(kMaintainAudioPitchInput, e); }

    TransitionBlock* in_transition() { return in_transition_; }

    void set_in_transition(TransitionBlock* t) { in_transition_ = t; }

    TransitionBlock* out_transition() { return out_transition_; }

    void set_out_transition(TransitionBlock* t) { out_transition_ = t; }

    [[nodiscard]] const QVector<Block*>& block_links() const { return block_links_; }

    [[nodiscard]] FrameHashCache* connected_video_cache() const {
        if (Node* n = GetConnectedOutput(kBufferIn)) {
            return n->video_frame_cache();
        } else {
            return nullptr;
        }
    }

    [[nodiscard]] AudioPlaybackCache* connected_audio_cache() const {
        if (Node* n = GetConnectedOutput(kBufferIn)) {
            return n->audio_playback_cache();
        } else {
            return nullptr;
        }
    }

    FrameHashCache* thumbnails() {
        if (Node* n = GetConnectedOutput(kBufferIn)) {
            return n->thumbnail_cache();
        } else {
            return nullptr;
        }
    }

    AudioWaveformCache* waveform() {
        if (Node* n = GetConnectedOutput(kBufferIn)) {
            return n->waveform_cache();
        } else {
            return nullptr;
        }
    }

    void AddCachePassthroughFrom(ClipBlock* other);

    [[nodiscard]] ViewerOutput* connected_viewer() const { return connected_viewer_; }

    [[nodiscard]] TimeRange GetVideoCacheRange() const override { return {0, length()}; }

    [[nodiscard]] TimeRange GetAudioCacheRange() const override { return {0, length()}; }

    void ConnectedToPreviewEvent() override;

    [[nodiscard]] TimeRange media_range() const;

    /**
     * @brief Get currently set loop mode
     */
    [[nodiscard]] LoopMode loop_mode() const { return static_cast<LoopMode>(GetStandardValue(kLoopModeInput).toInt()); }

    void set_loop_mode(LoopMode l) { SetStandardValue(kLoopModeInput, int(l)); }

    MultiCamNode* FindMulticam();

    static const QString kBufferIn;
    static const QString kMediaInInput;
    static const QString kSpeedInput;
    static const QString kReverseInput;
    static const QString kMaintainAudioPitchInput;
    static const QString kLoopModeInput;

    static const QString kAutoCacheInput;

protected:
    void LinkChangeEvent() override;

    void InputConnectedEvent(const QString& input, int element, Node* output) override;

    void InputDisconnectedEvent(const QString& input, int element, Node* output) override;

    void InputValueChangedEvent(const QString& input, int element) override;

private:
    enum SequenceToMediaTimeFlag {
        kSTMNone = 0x0,
        kSTMIgnoreReverse = 0x1,
        kSTMIgnoreSpeed = 0x2,
        kSTMIgnoreLoop = 0x4
    };

    [[nodiscard]] rational SequenceToMediaTime(const rational& sequence_time, uint64_t flags = kSTMNone) const;

    [[nodiscard]] rational MediaToSequenceTime(const rational& media_time) const;

    void RequestRangeFromConnected(const TimeRange& range);

    void RequestRangeForCache(PlaybackCache* cache, const TimeRange& max_range, const TimeRange& range, bool invalidate,
                              bool request);
    void RequestInvalidatedForCache(PlaybackCache* cache, const TimeRange& max_range);

    bool GetAdjustedThumbnailRange(TimeRange* r) const;

    QVector<Block*> block_links_;

    TransitionBlock* in_transition_ = nullptr;
    TransitionBlock* out_transition_ = nullptr;

    ViewerOutput* connected_viewer_;

private:
    rational last_media_in_;
};

}  // namespace arcvideo

#endif  // TIMELINEBLOCK_H
