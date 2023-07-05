// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

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
