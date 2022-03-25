italian-draughts(5)

# NAME
italian-draughts - resource files

# DESCRIPTION
italian-draughts searches the resources in following paths:

. /etc/italian-draughts
. ~/.config/italian-draughts

These are read in cascade from the first path to the last one, that is, from
an empty set every resourse will be added to it or it's value will be updated.

Not all resourses are read, but those that are required to the program (see
*italian-draughts*(1)) and the parent's ones recursively up to the default ones.

In general the resourses are located in <path>/<type>/<modifier>, with <path>
the resourses path, <type> the resourse type (i.e. strings or colors) and
<modifier> the file in which there are some resourses.

To support different configurations (i.e. different languages, different themes)
the resourses of a specific type are splitted in 1 or more files.

For each type of resourse, all the default values are located in "default" file,
that should be always present in every <type> directory.

# MODIFIERS
Modifiers allows you to create a variant of a specific type (i.e. localize a
program, set a custom theme, etc.). Below is indicated which modifier to use
(that is the file name) for the most used resourse types, and in brackets the
hierarchy used in case the modifier don't overwrite the parent resourse. The
invalid characters are: "/" and "\\".

## strings (default > language > language_country)
The language ISO code (i.e. en_US) taking to account that the possible presence
of a file with ISO code that includes the country is children of what includes
only the language and the latter must be present. If the requested language
contains the charset (i.e. en_US.UTF-8) the latter will be ignored (this program
uses only exclusively UTF-8).

## colors (default > theme)
The theme name. There is no specific hierarchy.

# SEE ALSE
*italian-draughts*(1)

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant44@gmail.com>. Up-to-date sources
can be found at https://gitlab.com/nicolarevelant44/italian-draughts and
bugs/patches can be submitted by e-mail.
