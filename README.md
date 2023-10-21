# Italian Draughts

Play Italian Draughts against the computer with 10 difficulty levels.

Instructions on how to play can be found
[here](https://en.wikipedia.org/wiki/Italian_draughts).

## Documentation

Manual pages are available in section 1 (command invocation) and section 5
(config and resource files syntax). Manual pages are written
in [scdoc](https://git.sr.ht/~sircmpwn/scdoc).

Info pages are not available (yet).

## Git branches

Main branch offers a stable version it should compile correctly.
Other branches do not guarantee it compiles (specially those starting with dev/).

When a new version is available the corresponding commit will be tagged
with the version code (MAJOR.MINOR.PATCH).

## Dependencies

At build-time:

- scdoc (compile manual pages)
- CMake
- Ninja (or make)

At runtime and build-time:

- wxWidgets

## Installation

To install into ``/usr/local``, run:

```bash
cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build
```

CMake cache variables:

| Variable name | Description | Type | Default value |
| - | - | - | - |
| CMAKE_INSTALL_PREFIX | Installation path | String | ``/usr/local`` |
| DATA_PATH | Application data path | String | ``${CMAKE_INSTALL_PREFIX}/share/italian-draughts`` |
| DOCS | Install documentation | Boolean | OFF

Windows and macOS are not supported yet.

# Copyright and license

Third-party software used:

- [wxWidgets](https://www.wxwidgets.org/) (C++ library for GUI)
- [scdoc](https://git.sr.ht/~sircmpwn/scdoc/) (Compile manual pages)

Copyright (C) 2023 Nicola Revelant

This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
