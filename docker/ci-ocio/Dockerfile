# Copyright (C) 2022 Olive Team
# SPDX-License-Identifier: GPL-3.0-or-later

# Build image (default):
#  docker build -t arcvideoeditor/ci-package-ocio:2022-2.1.1 -f ci-ocio/Dockerfile .

ARG ARCVIDEO_ORG=arcvideoeditor
ARG ASWF_PKG_ORG=aswftesting
ARG CI_COMMON_VERSION=2
ARG VFXPLATFORM_VERSION=2022
# OCIO repo branch or tag name
ARG OCIO_VERSION=v2.1.1
# Latest configs ~3 GB because of ACES 1.0.x.
# Upstream only copies nuke-default, therefore we also use the older configs.
ARG OCIO_CONFIGS_VERSION=1.0_r2

FROM ${ARCVIDEO_ORG}/ci-common:${CI_COMMON_VERSION} as ci-ocio

ARG ARCVIDEO_ORG
ARG CI_COMMON_VERSION
ARG VFXPLATFORM_VERSION
ARG OCIO_VERSION
ARG OCIO_CONFIGS_VERSION

LABEL maintainer="arcvideoeditor@gmail.com"
LABEL com.vfxplatform.version=$VFXPLATFORM_VERSION
LABEL org.opencontainers.image.name="$ARCVIDEO_ORG/ci-otio"
LABEL org.opencontainers.image.description="CentOS OpenColorIO Build Image"
LABEL org.opencontainers.image.url="http://arcvideoeditor.org"
LABEL org.opencontainers.image.source="https://github.com/arcvideo-editor/olive"
LABEL org.opencontainers.image.vendor="ArcVideo Team"
LABEL org.opencontainers.image.version="1.0"
LABEL org.opencontainers.image.licenses="GPL-3.0-or-later"

ENV ARCVIDEO_ORG=${ARCVIDEO_ORG} \
    CI_COMMON_VERSION=${CI_COMMON_VERSION} \
    VFXPLATFORM_VERSION=${VFXPLATFORM_VERSION} \
    OCIO_VERSION=${OCIO_VERSION} \
    OCIO_CONFIGS_VERSION=${OCIO_CONFIGS_VERSION} \
    ARCVIDEO_INSTALL_PREFIX=/usr/local

COPY scripts/build_ocio.sh \
     /tmp/

RUN /tmp/before_build.sh && \
    /tmp/build_ocio.sh && \
    /tmp/copy_new_files.sh

FROM scratch as ci-package-ocio

COPY --from=ci-ocio /package/. /
