// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_STRING_RESOURCES_H
#define ITALIAN_DRAUGHTS_STRING_RESOURCES_H

#include "wx/wx.h"
#include <bits/stdc++.h>
#include "ProjectVariables.h"

class Resources {
public:
	explicit Resources();

	typedef std::map<std::string, wxColour> ColorsMap;

	/**
	 * Adds or overwrites colors read from a file that represents the specified theme
	 * @param theme Theme to add
	 */
	void addTheme(const std::string &theme);

	const wxColour &getColor(const std::string &key, const wxColour &def = wxNullColour) const;

private:
	Resources(const Resources &); // prevents copy-constructor
	ColorsMap m_colors;   // colors for current theme

	static bool fillColorsMap(ColorsMap &colors,
	                          const std::string &path = SYSTEM_CFG_PATH,
	                          const std::string &theme = "default");
};


#endif //ITALIAN_DRAUGHTS_STRING_RESOURCES_H
