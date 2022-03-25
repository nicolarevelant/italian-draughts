italian-draughts(1)

# NAME
italian-draughts - Italian Draughts Game

# SYNOPSIS
italian-draughts [options...]

# OPTIONS
## -h, --help
Display a simple help message

## -l, --locale
Sets the locale

## -t, --theme
Sets the theme

# DESCRIPTION
This program allows you to play italian draughts, a strategy board game for 2
players and it's played on a 8x8 checkerboard.

# CONFIGURATION
italian-draughts searches the resources into the following paths:

. /etc/italian-draughts
. ~/.config/italian-draughts

They are read in cascade from the first path to the last one, that is, from a
empty set every resource will be added or it's value will be updated.

For information on the config file format, see *italian-draughts*(5).

# SEE ALSO
*italian-draughts*(5)

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant44@gmail.com>. Up-to-date sources
can be found at https://gitlab.com/nicolarevelant44/italian-draughts and
bugs/patches can be submitted by e-mail.
