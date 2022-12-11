#ifndef ITALIAN_DRAUGHTS_STRING_RESOURCES_H
#define ITALIAN_DRAUGHTS_STRING_RESOURCES_H

#include "wx/wx.h"
#include <bits/stdc++.h>
#include "ProjectVariables.h"

#define LOCALE_LANGUAGE_LEN 2
#define LOCALE_MAX_LEN 5

class Resources {
public:
	explicit Resources(const std::string &locale = "", const std::string &theme = "");

	typedef std::map<std::string, wxString> StringsMap;
	typedef std::map<std::string, wxColour> ColorsMap;

	const wxString &getString(const std::string &key, const wxString &def = wxEmptyString) const;
	const wxColour &getColor(const std::string &key, const wxColour &def = wxNullColour) const;


private:
	Resources(const Resources &);
	StringsMap m_strings; // strings for current locale
	ColorsMap m_colors;   // colors for current theme

	static bool fillStringsMap(StringsMap &strings,
	                           const std::string &path = SYSTEM_CFG_PATH,
	                           const std::string &locale = "default");

	static bool fillColorsMap(ColorsMap &colors,
	                          const std::string &path = SYSTEM_CFG_PATH,
	                          const std::string &theme = "default");
};


#endif //ITALIAN_DRAUGHTS_STRING_RESOURCES_H
