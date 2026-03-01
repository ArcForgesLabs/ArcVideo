# Docker Images

ArcVideo uses Docker containers for continuous integration on Linux.
No Docker images are involved for the Windows and macOS CI.

## Overview

`ci-common` is the shared build image with GCC, Clang and packages that are
needed by most dependent images. It is used to compile ArcVideo's dependencies
in a controlled environment. The final CI image `ci-arcvideo` is assembled from
images maintained by the ArcVideo team as well as from
[aswf-docker](https://github.com/AcademySoftwareFoundation/aswf-docker/).

Dependency hierarchy:

1. `ci-common`
2. `ci-otio`, `ci-crashpad`, `ci-ffmpeg`, `ci-ocio`
3. `ci-arcvideo`

## Usage

Pull images from [Docker Hub](https://hub.docker.com/u/arcvideoeditor):

```
docker pull arcvideoeditor/ci-common:2
docker pull arcvideoeditor/ci-package-otio:0.14.1
docker pull arcvideoeditor/ci-package-crashpad
docker pull arcvideoeditor/ci-package-ffmpeg:5.0
docker pull arcvideoeditor/ci-package-ocio:2022-2.1.1
docker pull arcvideoeditor/ci-arcvideo:2022.2
```

Use `ci-arcvideo` image as local build container, by mounting working copy at
`~/arcvideo` into guest system at `/opt/arcvideo/arcvideo`:

```bash
docker run --rm -it -v ~/arcvideo:/opt/arcvideo/arcvideo arcvideoeditor/ci-arcvideo:2022.2
mkdir build
cd build
cmake .. -G Ninja
cmake --build .
```

Rebuild all images locally:

```
cd docker
docker build -t arcvideoeditor/ci-common:2 -f ci-common/Dockerfile .
docker build -t arcvideoeditor/ci-package-otio:0.14.1 -f ci-otio/Dockerfile .
docker build -t arcvideoeditor/ci-package-crashpad -f ci-crashpad/Dockerfile .
docker build -t arcvideoeditor/ci-package-ffmpeg:5.0 -f ci-ffmpeg/Dockerfile .
docker build -t arcvideoeditor/ci-package-ocio:2022-2.1.1 -f ci-ocio/Dockerfile .
docker build -t arcvideoeditor/ci-arcvideo:2022.2 -f ci-arcvideo/Dockerfile .
```

Note that `2022` in `ci-arcvideo:2022.2` stands for the
[VFX Reference Platform](http://vfxplatform.com/) calendar year and `2` for the
build image revision (should be incremented each time a new image is published).

Publish images:

```
docker push arcvideoeditor/ci-common:2
docker push arcvideoeditor/ci-package-otio:0.14.1
docker push arcvideoeditor/ci-package-crashpad
docker push arcvideoeditor/ci-package-ffmpeg:5.0
docker push arcvideoeditor/ci-package-ocio:2022-2.1.1
docker push arcvideoeditor/ci-arcvideo:2022.2
```
