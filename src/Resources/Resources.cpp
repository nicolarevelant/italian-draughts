// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "Resources.h"

Resources::Resources(const std::string &path) {
	fillColorMap(mColors, path);
	fillBitmapMap(mBitmaps, path);
}

void Resources::addTheme(const std::string &path, const std::string &theme) {
	fillColorMap(mColors, path, theme);
	fillBitmapMap(mBitmaps, path, theme);
}

const wxColour &Resources::getColor(const std::string &key, const wxColour &def) const {
	try {
		return mColors.at(key);
	} catch (std::out_of_range &) {
		return def;
	}
}

const wxBitmap &Resources::getBitmap(const std::string &key, const wxBitmap &def) const {
	try {
		return mBitmaps.at(key);
	} catch (std::out_of_range &) {
		return def;
	}
}

bool Resources::fillColorMap(ColorMap &colors, const std::string &path, const std::string &theme) {
	std::string themePath = path;
	themePath += "/colors/";
	themePath += theme;

	std::ifstream file(themePath);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == '#') continue;

			auto delimPos = line.find('=');
			if (delimPos == std::string::npos) continue;

			auto key = line.substr(0, delimPos);
			auto value = line.substr(delimPos + 1);
			colors[key] = wxColour(value.c_str());
		}

		file.close();
		return true;
	}

	return false;
}

bool Resources::fillBitmapMap(BitmapMap &bitmaps, const std::string &path, const std::string &theme) {
	std::string themePath = path;
	themePath += "/images/";
	themePath += theme;

	std::ifstream indexFile(themePath + "/index");
	if (indexFile.is_open()) {
		themePath += "/";
		std::string line;

		while (std::getline(indexFile, line)) {
			if (line.empty() || line[0] == '#') continue;

			// add "line.png" bitmap
			std::string imagePath = themePath;
			imagePath += line;
			imagePath += ".png";

			bitmaps[line] = wxBitmap(imagePath);
		}
	}

	return false;
}
