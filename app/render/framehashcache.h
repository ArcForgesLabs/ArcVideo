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

#ifndef VIDEORENDERFRAMECACHE_H
#define VIDEORENDERFRAMECACHE_H

#include "codec/frame.h"
#include "render/playbackcache.h"
#include "render/videoparams.h"

namespace arcvideo {

class FrameHashCache : public PlaybackCache {
    Q_OBJECT

public:
    FrameHashCache(QObject* parent = nullptr);

    [[nodiscard]] const rational& GetTimebase() const { return timebase_; }

    void SetTimebase(const rational& tb);

    void ValidateTimestamp(const int64_t& ts);
    void ValidateTime(const rational& time);

    [[nodiscard]] bool IsFrameCached(const rational& time) const { return GetValidatedRanges().contains(time); }

    [[nodiscard]] QString GetValidCacheFilename(const rational& time) const;

    static bool SaveCacheFrame(const QString& filename, FramePtr frame);
    [[nodiscard]] bool SaveCacheFrame(const int64_t& time, FramePtr frame) const;
    static bool SaveCacheFrame(const QString& cache_path, const QUuid& uuid, const int64_t& time, FramePtr frame);
    static bool SaveCacheFrame(const QString& cache_path, const QUuid& uuid, const rational& time, const rational& tb,
                               FramePtr frame);
    static FramePtr LoadCacheFrame(const QString& cache_path, const QUuid& uuid, const int64_t& time);
    [[nodiscard]] FramePtr LoadCacheFrame(const int64_t& time) const;
    static FramePtr LoadCacheFrame(const QString& fn);

    void SetPassthrough(PlaybackCache* cache) override;

protected:
    void LoadStateEvent(QDataStream& stream) override;
    void SaveStateEvent(QDataStream& stream) override;

private:
    [[nodiscard]] rational ToTime(const int64_t& ts) const;
    [[nodiscard]] int64_t ToTimestamp(const rational& ts, Timecode::Rounding rounding = Timecode::kRound) const;

    /**
     * @brief Return the path of the cached image at this time
     */
    [[nodiscard]] QString CachePathName(const int64_t& time) const;
    [[nodiscard]] QString CachePathName(const rational& time) const;

    static QString CachePathName(const QString& cache_path, const QUuid& cache_id, const int64_t& time);
    static QString CachePathName(const QString& cache_path, const QUuid& cache_id, const rational& time,
                                 const rational& tb);

    rational timebase_;

private slots:
    void HashDeleted(const QString& path, const QString& filename);

    void ProjectInvalidated(Project* p);
};

class ThumbnailCache : public FrameHashCache {
    Q_OBJECT

public:
    ThumbnailCache(QObject* parent = nullptr) : FrameHashCache(parent) { SetTimebase(rational(1, 10)); }
};

}  // namespace arcvideo

#endif  // VIDEORENDERFRAMECACHE_H
