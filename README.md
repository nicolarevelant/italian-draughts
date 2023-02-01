# Italian Draughts

This program allows you to play italian draughts against the computer
using the [Minimax](https://en.wikipedia.org/wiki/Minimax) algorithm.

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

From the source directory:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build
/usr/bin/italian-draughts
```

To set a custom application data path (instead of /usr/share/italian-draughts/)
you can set the `DATA_PATH` cmake variable.

If CMAKE_BUILD_TYPE equals Debug and DATA_PATH is not set then DATA_PATH is set
to the absolute path of the project root directory.

If you set a custom path the manual pages and the installation rules will be ignored.

# Copyright and license

Third-party software used:

- [wxWidgets](https://www.wxwidgets.org/) (C++ library for GUI)
- [scdoc](https://git.sr.ht/~sircmpwn/scdoc/) (Compiling manual pages)

Copyright (C) 2023 Nicola Revelant

This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
