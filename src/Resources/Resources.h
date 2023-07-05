// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_STRING_RESOURCES_H
#define ITALIAN_DRAUGHTS_STRING_RESOURCES_H

#include "wx/wx.h"
#include <bits/stdc++.h>
#include "ProjectVariables.h"

class Resources {
public:
	typedef std::map<std::string, wxColour> ColorMap;
	typedef std::map<std::string, wxBitmap> BitmapMap;

	/**
	 * Initialize a new object with the default theme
	 * @param path Resource path
	 */
	explicit Resources(const std::string &path);

	/**
	 * Adds or overwrites colors read from a file that represents the specified theme
	 * @param path Resource path
	 * @param theme Theme to add
	 */
	void addTheme(const std::string &path, const std::string &theme);

	const wxColour &getColor(const std::string &key, const wxColour &def = wxNullColour) const;

	const wxBitmap &getBitmap(const std::string &key, const wxBitmap &def = wxNullBitmap) const;

private:
	Resources(const Resources &); // prevents copy-constructor
	ColorMap mColors;
	BitmapMap mBitmaps;

	static bool fillColorMap(ColorMap &colors, const std::string &path, const std::string &theme = "default");

	static bool fillBitmapMap(BitmapMap &bitmaps, const std::string &path, const std::string &theme = "default");
};


#endif //ITALIAN_DRAUGHTS_STRING_RESOURCES_H
