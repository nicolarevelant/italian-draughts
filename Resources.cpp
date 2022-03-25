#include "Resources.h"

StringResources::StringResources(const std::string &locale) {
    if (locale.empty()) {
        fillStringsMap(m_strings, SYSTEM_CFG_PATH);
        fillStringsMap(m_strings, USER_CFG_PATH);
        m_parent = nullptr;
        return;
    }

    if (locale.length() > LOCALE_MAX_LEN) {
        // if locale = en_US.UTF-8 remove the charset
        fillStringsMap(m_strings, SYSTEM_CFG_PATH, locale.substr(0, LOCALE_MAX_LEN));
        fillStringsMap(m_strings, USER_CFG_PATH, locale.substr(0, LOCALE_MAX_LEN));
    } else {
        fillStringsMap(m_strings, SYSTEM_CFG_PATH, locale);
        fillStringsMap(m_strings, USER_CFG_PATH, locale);
    }

    m_parent = new StringResources(locale.length() > LOCALE_LANGUAGE_LEN ? locale.substr(0, LOCALE_LANGUAGE_LEN) : "");
}

ColorResources::ColorResources(const std::string &theme) {
    if (theme.empty()) {
        fillColorsMap(m_colors, SYSTEM_CFG_PATH);
        fillColorsMap(m_colors, USER_CFG_PATH);
        m_parent = nullptr;
        return;
    }

    fillColorsMap(m_colors, SYSTEM_CFG_PATH, theme);
    fillColorsMap(m_colors, USER_CFG_PATH, theme);

    m_parent = new ColorResources();
}

StringResources::~StringResources() {
    delete m_parent;
}

ColorResources::~ColorResources() {
    delete m_parent;
}

bool StringResources::fillStringsMap(StringsMap &strings, const std::string& path, const std::string& locale) {
    std::ifstream file(path + "strings/" + locale);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line.starts_with('#')) continue;

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

bool ColorResources::fillColorsMap(ColorsMap &colors, const std::string& path, const std::string& theme) {
    std::ifstream file(path + "colors/" + theme);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line.starts_with('#')) continue;

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

const wxString &StringResources::get(const std::string &key, const wxString &def) const {
    try {
        return m_strings.at(key);
    } catch (std::exception &e) {
        // checks the parent
        if (m_parent == nullptr) return def;
        return m_parent->get(key, def);
    }
}

const wxColour &ColorResources::get(const std::string &key, const wxColour &def) const {
    try {
        return m_colors.at(key);
    } catch (std::exception &e) {
        // checks the parent
        if (m_parent == nullptr) return def;
        return m_parent->get(key, def);
    }
}
