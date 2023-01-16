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

# RESOURCES
italian-draughts searches the resources into the following path:

. /etc/italian-draughts/themes/

First it reads from 'default' file and then, if the user specified a theme,
from the file whose name corresponds to the theme name. New values can be added
or overwrite the default ones.

For information on the resource file format, see *italian-draughts*(5).

# SEE ALSO
*italian-draughts*(5)

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant44@gmail.com>. Up-to-date sources
can be found at https://gitlab.com/nicolarevelant44/italian-draughts and
bugs/patches can be submitted via e-mail or via GitLab.
