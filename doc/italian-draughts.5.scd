italian-draughts(5)

# NAME
italian-draughts - application data

# DESCRIPTION
italian-draughts reads the application data from the following path:

- /usr/share/italian-draughts/

# THEMES

For each resource type this program first reads from the 'default' file
located inside the directory of the resource type.

Then, if the user specified a theme, from the file whose name corresponds
to the theme name. New values can be added or overwrite the default ones.

Every line of a resource file must be a <key>=<value> pair without whitespaces.
Empty lines or those starting with *#* are ignored.

## colors

Used colors are:

- border: Chessboard border
- bg: Backgroud
- fg: Foregroud (text color)
- light: Light grid cels
- dark: Dark grid cels

## images

Images are considered a resource type but every theme is a directory instead of a regular file.
So for each theme directory there must be an index where every line is the image key
that is a image filename without extension.
Every image file is called _key.png_ and there must be the following image keys
at least in the default theme:

- pcDame: PC dame/king
- pcPawn: PC pawn/man
- plDame: Player dame/king
- plPawn: Player pawn/man

Note: all images must be the same size

# SEE ALSO
*italian-draughts*(1)

GitHub repository: https://github.com/nicolarevelant/italian-draughts

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant@outlook.com> and by other open
source contributors.
