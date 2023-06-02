# Italian Draughts

Play Italian Draughts against the computer.

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

## Installation

To install into ``/usr/local``, run:

```bash
git clone https://github.com/nicolarevelant/italian-draughts.git
cd italian-draughts
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j8
sudo cmake --install build
```

The installation path can be changed using the ``-DCMAKE_INSTALL_PREFIX=<installation path>``
argument for ``cmake``.

To set a custom application data path (instead of ``<installation path>/share/italian-draughts``)
you can set the `DATA_PATH` cmake variable.

If CMAKE_BUILD_TYPE equals Debug and DATA_PATH is not set then DATA_PATH is set
to the absolute path of the project root directory.

If you set a custom DATA_PATH (or CMAKE_BUILD_TYPE equals Debug) the manual pages
and the installation target will be ignored.

Windows and macOS are not supported yet.

# Copyright and license

Third-party software used:

- [wxWidgets](https://www.wxwidgets.org/) (C++ library for GUI)
- [scdoc](https://git.sr.ht/~sircmpwn/scdoc/) (Compile manual pages)

Copyright (C) 2023 Nicola Revelant

This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
