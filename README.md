# Italian Draughts

This program allows you to play italian draughts against the computer
using the [Minimax](https://en.wikipedia.org/wiki/Minimax) algorithm.

Instructions on how to play can be found [here](https://en.wikipedia.org/wiki/Italian_draughts).

## Manual pages

Manual pages are available in section 1 (command invocation) and section 5
(config files syntax). Man pages are written in [scdoc](https://git.sr.ht/~sircmpwn/scdoc).

## Git branches

Main branch offers a stable version it should compile correctly.
Other branches do not guarantee it compiles (specially those starting with dev/).

When a new version is available the corresponding commit will be tagged
with the version code (MAJOR.MINOR.PATCH).

## Installation

From the source directory

Portable:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DPORTABLE=1
cmake --build build
build/italian-draughts
```

System:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build
/usr/bin/italian-draughts
```
