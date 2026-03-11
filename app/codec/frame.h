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

#ifndef FRAME_H
#define FRAME_H

#include <arcvideo/foundation/foundation.h>

#include <QVector>
#include <memory>

#include "common/define.h"
#include "render/videoparams.h"

namespace arcvideo {

class Frame;
using FramePtr = std::shared_ptr<Frame>;

/**
 * @brief Video frame data or audio sample data from a Decoder
 */
class Frame {
public:
    Frame();

    ~Frame();

    DISABLE_COPY_MOVE(Frame)

    static FramePtr Create();

    [[nodiscard]] const VideoParams& video_params() const;
    void set_video_params(const VideoParams& params);

    static FramePtr Interlace(FramePtr top, FramePtr bottom);

    static int generate_linesize_bytes(int width, PixelFormat format, int channel_count);

    [[nodiscard]] int linesize_pixels() const { return linesize_pixels_; }

    [[nodiscard]] int linesize_bytes() const { return linesize_; }

    [[nodiscard]] int width() const { return params_.effective_width(); }

    [[nodiscard]] int height() const { return params_.effective_height(); }

    [[nodiscard]] PixelFormat format() const { return params_.format(); }

    [[nodiscard]] int channel_count() const { return params_.channel_count(); }

    [[nodiscard]] Color get_pixel(int x, int y) const;
    [[nodiscard]] bool contains_pixel(int x, int y) const;
    void set_pixel(int x, int y, const Color& c);

    /**
     * @brief Get frame's timestamp.
     *
     * This timestamp is always a rational that will equate to the time in seconds.
     */
    [[nodiscard]] const rational& timestamp() const { return timestamp_; }

    void set_timestamp(const rational& timestamp) { timestamp_ = timestamp; }

    /**
     * @brief Get the data buffer of this frame
     */
    char* data() { return data_; }

    /**
     * @brief Get the const data buffer of this frame
     */
    [[nodiscard]] const char* const_data() const { return data_; }

    /**
     * @brief Allocate memory buffer to store data based on parameters
     *
     * For video frames, the width(), height(), and format() must be set for this function to work.
     *
     * If a memory buffer has been previously allocated without destroying, this function will destroy it.
     */
    bool allocate();

    /**
     * @brief Return whether the frame is allocated or not
     */
    [[nodiscard]] bool is_allocated() const { return data_; }

    /**
     * @brief Destroy a memory buffer allocated with allocate()
     */
    void destroy();

    /**
     * @brief Returns the size of the array returned in data() in bytes
     *
     * Returns 0 if nothing is allocated.
     */
    [[nodiscard]] int allocated_size() const { return data_size_; }

    [[nodiscard]] FramePtr convert(PixelFormat format) const;

private:
    VideoParams params_;

    char* data_ = nullptr;
    int data_size_;

    rational timestamp_;

    int linesize_;

    int linesize_pixels_;
};

}  // namespace arcvideo

Q_DECLARE_METATYPE(arcvideo::FramePtr)

#endif  // FRAME_H
