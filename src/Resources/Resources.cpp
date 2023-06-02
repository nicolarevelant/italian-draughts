// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "Resources.h"

Resources::Resources(const std::string &path) {
	fillColorsMap(m_colors, path);
}

void Resources::addTheme(const std::string &path, const std::string &theme) {
	fillColorsMap(m_colors, path, theme);
}

const wxColour &Resources::getColor(const std::string &key, const wxColour &def) const {
	try {
		return m_colors.at(key);
	} catch (std::exception &e) {
		return def;
	}
}

bool Resources::fillColorsMap(ColorsMap &colors, const std::string &path, const std::string &theme) {
	std::ifstream file(path + "/themes/" + theme);
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
