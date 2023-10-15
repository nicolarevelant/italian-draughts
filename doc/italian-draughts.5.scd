italian-draughts(5)

# NAME
italian-draughts - application data

# DESCRIPTION
italian-draughts reads the application data from the following path:

- /usr/share/italian-draughts/

## IMAGES

In the images subdirectory this program reads the following images:

- pcDame.png: PC dame/king
- pcPawn.png: PC pawn/man
- plDame.png: Player dame/king
- plPawn.png: Player pawn/man

Note: all images must be the same size

## THEMES

In the themes subdirectory this program first reads from the 'default' file
and then, if the user specified a theme, from the file whose name corresponds
to the theme name. New values can be added or overwrite the default ones.

Every line of a resource file must be a <key>=<value> pair without whitespaces.
Empty lines or those starting with # are ignored.

# SEE ALSO
*italian-draughts*(1)

GitHub repository: https://github.com/nicolarevelant/italian-draughts

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant@outlook.com> and by other open
source contributors.
