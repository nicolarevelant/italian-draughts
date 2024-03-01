# Resource Management

Class for accessing an application's resources.

## Internationalization (i18n)

This program can be started with a specific locale, or it will use the system language.

Localized messages are stored in:

- Unix/Linux: /usr/share/locale/**lang**/LC_MESSAGES/italian-draughts.mo
- MacOS: italian-draughts.app/Contents/Resources/**lang**.lproj/italian-draughts.mo
- Windows: C:\\Program Files\\italian-draughts\\**lang**\\italian-draughts.mo

Please refer to [wxWidgets wiki](https://docs.wxwidgets.org/3.2.1/overview_i18n.html)

## Themes

This program can be started with a specific theme, or it will use the default one.

Assuming the chosen theme is **theme**, for each resource type this program reads from
the following path in a simple format as described below.

For each entry if the file does not exist, it is unreadable or the entry is not available,
it fall back to the default one.

- /etc/italian-draughts/**resource_type**/**theme**

### File format

Each line can start with a "#" to indicate a comment or be a key=value pair divided by a "=" without
whitespaces of any type.

### Resource types

- colors: UI colors
- images: Bitmaps in PNG format (keys does not include extension)
