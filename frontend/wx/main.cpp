/*
    Copyright (C) 2023-2024  Nicola Revelant

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <getopt.h>
#include "Frame/Frame.h"
#include "wx/wx.h"

static option longOptions[] = {
		{"locale", required_argument, nullptr, 'l'},
		{"theme",  required_argument, nullptr, 't'},
		{"help",   no_argument,       nullptr, 'h'}
};

class MyApp : public wxApp {
public:
	bool OnInit() override {
		wxLog::SetActiveTarget(new wxLogStderr);
		char *locale = nullptr;
		std::string theme;
		int c;
		while ((c = getopt_long(argc, argv, ":l:t:h", longOptions, nullptr)) != -1) {
			switch (c) {
				case '?':
					std::cout << "Unknown option: " << optind << std::endl;
					return false;
				case 'l':
					// locale
					locale = optarg;
					break;
				case 't':
					// theme
					theme = optarg;
					break;
				case 'h':
					// help message
					printHelpMessage();
					return false;
				default:
					std::cout << "Error parsing arguments" << std::endl;
					return false;
			}
		}

		if (locale)
			std::setlocale(LC_ALL, locale);

		auto *frame = new Frame();
		if (frame->Create(nullptr, theme)) {
			frame->Show(true);
		} else {
			frame->Destroy();
			std::cerr << "Cannot open GUI" << std::endl;
		}
		return true;
	}

	bool OnExceptionInMainLoop() override {
		std::cerr << "Exception in main loop" << std::endl;
		return false;
	}

	void OnUnhandledException() override {
		std::cerr << "Unhandled Exception" << std::endl;
	}

	void printHelpMessage() {
		std::cout << "Usage: " << argv[0] << " [OPTIONS]" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "  -l, --locale=LOCALE        Use LOCALE as language" << std::endl;
		std::cout << "  -t, --theme=THEME          Use THEME as theme name" << std::endl;
	}
};

wxIMPLEMENT_APP(MyApp);
