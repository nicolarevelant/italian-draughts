italian-draughts(5)

# NAME
italian-draughts - resource files

# DESCRIPTION
italian-draughts searches the resources in following path:

. /etc/italian-draughts/themes/

First it reads from 'default' file and then, if the user specified a theme,
from the file whose name corresponds to the theme name. New values can be added
or overwrite the default ones.

Every line of a resource file must be a <key>=<value> pair without whitespaces.
Empty lines or those starting with # are ignored.

# SEE ALSO
*italian-draughts*(1)

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant44@gmail.com>. Up-to-date sources
can be found at https://gitlab.com/nicolarevelant44/italian-draughts and
bugs/patches can be submitted via e-mail or via GitLab.
