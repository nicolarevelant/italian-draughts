//
// Created by nicola
//

#ifndef ITALIAN_DRAUGHTS_STRING_RESOURCES_H
#define ITALIAN_DRAUGHTS_STRING_RESOURCES_H

#include "wx/wx.h"
#include <bits/stdc++.h>
#include "ProjectVariables.h"

#define LOCALE_LANGUAGE_LEN 2
#define LOCALE_MAX_LEN 5

class StringResources {
public:
    explicit StringResources(const std::string &locale = "");
    virtual ~StringResources();

    typedef std::map<std::string, wxString> StringsMap;

    [[nodiscard]] const wxString &get(const std::string &key, const wxString &def = wxEmptyString) const;
    const wxString &operator[](const std::string &key) const {
        return get(key);
    }

private:
    StringsMap m_strings; // strings for current locale
    StringResources *m_parent;

    static bool fillStringsMap(StringsMap &strings,
                               const std::string &path = SYSTEM_CFG_PATH,
                               const std::string& locale = "default");
};

class ColorResources {
public:
    explicit ColorResources(const std::string &theme = "");
    virtual ~ColorResources();

    typedef std::map<std::string, wxColour> ColorsMap;

    [[nodiscard]] const wxColour &get(const std::string &key, const wxColour &def = wxNullColour) const;
    const wxColour &operator[](const std::string &key) const {
        return get(key);
    }

private:
    ColorsMap m_colors;   // colors for current theme
    ColorResources *m_parent;

    static bool fillColorsMap(ColorsMap &colors,
                              const std::string &path = SYSTEM_CFG_PATH,
                              const std::string &theme = "default");
};


#endif //ITALIAN_DRAUGHTS_STRING_RESOURCES_H
