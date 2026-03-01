# Copyright (C) 2022 Olive Team
# SPDX-License-Identifier: GPL-3.0-or-later

# Build image (default):
#  docker build -t arcvideoeditor/ci-package-crashpad -f ci-crashpad/Dockerfile .

ARG ARCVIDEO_ORG=arcvideoeditor
ARG CI_COMMON_VERSION=2

FROM ${ARCVIDEO_ORG}/ci-common:${CI_COMMON_VERSION} as ci-crashpad

ARG ARCVIDEO_ORG
ARG CI_COMMON_VERSION

LABEL maintainer="arcvideoeditor@gmail.com"
LABEL org.opencontainers.image.name="$ARCVIDEO_ORG/ci-crashpad"
LABEL org.opencontainers.image.description="CentOS Crashpad Build Image"
LABEL org.opencontainers.image.url="http://arcvideoeditor.org"
LABEL org.opencontainers.image.source="https://github.com/arcvideo-editor/olive"
LABEL org.opencontainers.image.vendor="ArcVideo Team"
LABEL org.opencontainers.image.version="1.0"
LABEL org.opencontainers.image.licenses="GPL-3.0-or-later"

COPY scripts/build_crashpad.sh \
     /tmp/

ENV ARCVIDEO_ORG=${ARCVIDEO_ORG} \
    CI_COMMON_VERSION=${CI_COMMON_VERSION} \
    ARCVIDEO_INSTALL_PREFIX=/usr/local

RUN /tmp/before_build.sh && \
    /tmp/build_crashpad.sh && \
    /tmp/copy_new_files.sh

FROM scratch as ci-package-crashpad

COPY --from=ci-crashpad /package /
