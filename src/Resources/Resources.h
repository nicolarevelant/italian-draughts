#ifndef ITALIAN_DRAUGHTS_STRING_RESOURCES_H
#define ITALIAN_DRAUGHTS_STRING_RESOURCES_H

#include "wx/wx.h"
#include <bits/stdc++.h>
#include "ProjectVariables.h"

class Resources {
public:
	explicit Resources(const std::string &theme = "");

	typedef std::map<std::string, wxColour> ColorsMap;

	const wxColour &getColor(const std::string &key, const wxColour &def = wxNullColour) const;

private:
	Resources(const Resources &); // prevents copy-constructor
	ColorsMap m_colors;   // colors for current theme

	static bool fillColorsMap(ColorsMap &colors,
	                          const std::string &path = SYSTEM_CFG_PATH,
	                          const std::string &theme = "default");
};


#endif //ITALIAN_DRAUGHTS_STRING_RESOURCES_H
