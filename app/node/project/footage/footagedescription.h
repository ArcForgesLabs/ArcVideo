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

#ifndef FOOTAGEDESCRIPTION_H
#define FOOTAGEDESCRIPTION_H

#include <algorithm>
#include <utility>

#include "node/output/track/track.h"
#include "render/subtitleparams.h"
#include "render/videoparams.h"

namespace arcvideo {

class FootageDescription {
public:
    // NOLINTNEXTLINE(performance-unnecessary-value-param) - intentional move
    FootageDescription(QString decoder = QString()) : decoder_(std::move(decoder)), total_stream_count_(0) {}

    [[nodiscard]] bool IsValid() const {
        return !decoder_.isEmpty() &&
               (!video_streams_.isEmpty() || !audio_streams_.isEmpty() || !subtitle_streams_.isEmpty());
    }

    [[nodiscard]] const QString& decoder() const { return decoder_; }

    void AddVideoStream(const VideoParams& video_params) {
        Q_ASSERT(!HasStreamIndex(video_params.stream_index()));

        video_streams_.append(video_params);
    }

    void AddAudioStream(const AudioParams& audio_params) {
        Q_ASSERT(!HasStreamIndex(audio_params.stream_index()));

        audio_streams_.append(audio_params);
    }

    void AddSubtitleStream(const SubtitleParams& sub_params) {
        Q_ASSERT(!HasStreamIndex(sub_params.stream_index()));

        subtitle_streams_.append(sub_params);
    }

    [[nodiscard]] Track::Type GetTypeOfStream(int index) const {
        if (StreamIsVideo(index)) {
            return Track::kVideo;
        } else if (StreamIsAudio(index)) {
            return Track::kAudio;
        } else if (StreamIsSubtitle(index)) {
            return Track::kSubtitle;
        } else {
            return Track::kNone;
        }
    }

    [[nodiscard]] bool StreamIsVideo(int index) const {
        return std::ranges::any_of(video_streams_,
                                   [index](const VideoParams& vp) { return vp.stream_index() == index; });
    }

    [[nodiscard]] bool StreamIsAudio(int index) const {
        return std::ranges::any_of(audio_streams_,
                                   [index](const AudioParams& ap) { return ap.stream_index() == index; });
    }

    [[nodiscard]] bool StreamIsSubtitle(int index) const {
        return std::ranges::any_of(subtitle_streams_,
                                   [index](const SubtitleParams& sp) { return sp.stream_index() == index; });
    }

    [[nodiscard]] bool HasStreamIndex(int index) const {
        return StreamIsVideo(index) || StreamIsAudio(index) || StreamIsSubtitle(index);
    }

    [[nodiscard]] int GetStreamCount() const { return total_stream_count_; }
    void SetStreamCount(int s) { total_stream_count_ = s; }

    bool Load(const QString& filename);

    [[nodiscard]] bool Save(const QString& filename) const;

    [[nodiscard]] const QVector<VideoParams>& GetVideoStreams() const { return video_streams_; }
    QVector<VideoParams>& GetVideoStreams() { return video_streams_; }

    [[nodiscard]] const QVector<AudioParams>& GetAudioStreams() const { return audio_streams_; }
    QVector<AudioParams>& GetAudioStreams() { return audio_streams_; }

    [[nodiscard]] const QVector<SubtitleParams>& GetSubtitleStreams() const { return subtitle_streams_; }
    QVector<SubtitleParams>& GetSubtitleStreams() { return subtitle_streams_; }

private:
    static constexpr unsigned kFootageMetaVersion = 6;

    QString decoder_;

    QVector<VideoParams> video_streams_;

    QVector<AudioParams> audio_streams_;

    QVector<SubtitleParams> subtitle_streams_;

    int total_stream_count_;
};

}  // namespace arcvideo

#endif  // FOOTAGEDESCRIPTION_H
