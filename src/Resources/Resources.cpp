#include "Resources.h"

Resources::Resources(const std::string &locale, const std::string &theme) {
	if (locale.empty()) {
		fillStringsMap(m_strings, SYSTEM_CFG_PATH);
		fillStringsMap(m_strings, USER_CFG_PATH);
	} else {
		fillStringsMap(m_strings, SYSTEM_CFG_PATH);
		fillStringsMap(m_strings, USER_CFG_PATH);

		std::string newLocale = locale;
		if (locale.length() > LOCALE_MAX_LEN) {
			// if locale = en_US.UTF-8 remove the charset
			newLocale = locale.substr(0, LOCALE_MAX_LEN);
			fillStringsMap(m_strings, SYSTEM_CFG_PATH, newLocale);
			fillStringsMap(m_strings, USER_CFG_PATH, newLocale);

			newLocale = newLocale.substr(0, LOCALE_LANGUAGE_LEN);
			fillStringsMap(m_strings, SYSTEM_CFG_PATH, newLocale);
			fillStringsMap(m_strings, USER_CFG_PATH, newLocale);
		} else {
			fillStringsMap(m_strings, SYSTEM_CFG_PATH, newLocale);
			fillStringsMap(m_strings, USER_CFG_PATH, newLocale);

			if (newLocale.length() > LOCALE_LANGUAGE_LEN) {
				newLocale = newLocale.substr(0, LOCALE_LANGUAGE_LEN);
				fillStringsMap(m_strings, SYSTEM_CFG_PATH, newLocale);
				fillStringsMap(m_strings, USER_CFG_PATH, newLocale);
			}
		}
	}

	if (theme.empty()) {
		fillColorsMap(m_colors, SYSTEM_CFG_PATH);
		fillColorsMap(m_colors, USER_CFG_PATH);
	} else {
		fillColorsMap(m_colors, SYSTEM_CFG_PATH);
		fillColorsMap(m_colors, USER_CFG_PATH);
		fillColorsMap(m_colors, SYSTEM_CFG_PATH, theme);
		fillColorsMap(m_colors, USER_CFG_PATH, theme);
	}

}

bool Resources::fillStringsMap(StringsMap &strings, const std::string &path, const std::string &locale) {
	std::ifstream file(path + "strings/" + locale);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == '#') continue;

			auto delimPos = line.find('=');
			if (delimPos == std::string::npos) continue;

			auto key = line.substr(0, delimPos);
			auto value = line.substr(delimPos + 1);

			strings[key] = wxString::FromUTF8(value.c_str());
		}

		file.close();
		return true;
	}

	return false;
}

bool Resources::fillColorsMap(ColorsMap &colors, const std::string &path, const std::string &theme) {
	std::ifstream file(path + "colors/" + theme);
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

const wxString &Resources::getString(const std::string &key, const wxString &def) const {
	try {
		return m_strings.at(key);
	} catch (std::exception &e) {
		return def;
	}
}

const wxColour &Resources::getColor(const std::string &key, const wxColour &def) const {
	try {
		return m_colors.at(key);
	} catch (std::exception &e) {
		return def;
	}
}
