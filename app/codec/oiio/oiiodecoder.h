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

#ifndef OIIODECODER_H
#define OIIODECODER_H

#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imageio.h>

#include "codec/decoder.h"

namespace arcvideo {

class OIIODecoder : public Decoder {
    Q_OBJECT

public:
    OIIODecoder() = default;

    DECODER_DEFAULT_DESTRUCTOR(OIIODecoder)

    [[nodiscard]] QString id() const override;

    bool SupportsVideo() override { return true; }

    FootageDescription Probe(const QString& filename, CancelAtom* cancelled) const override;

protected:
    bool OpenInternal() override;
    TexturePtr RetrieveVideoInternal(const RetrieveVideoParams& p) override;
    void CloseInternal() override;

private:
    std::unique_ptr<OIIO::ImageInput> image_;

    static bool FileTypeIsSupported(const QString& fn);

    bool OpenImageHandler(const QString& fn, int subimage);

    void CloseImageHandle();

    static VideoParams GetVideoParamsFromImageSpec(const OIIO::ImageSpec& spec);

    PixelFormat pix_fmt_;
    OIIO::TypeDesc::BASETYPE oiio_pix_fmt_;

    Frame buffer_;
    RetrieveVideoParams last_params_;

    static QStringList supported_formats_;
};

}  // namespace arcvideo

#endif  // OIIODECODER_H
