#include <getopt.h>
#include "Frame/Frame.h"
#include "wx/wx.h"

static option longOptions[] = {
		{"locale", required_argument, nullptr, 'l'},
		{"theme",  required_argument, nullptr, 't'}
};

class MyApp : public wxApp {
public:
	bool OnInit() override {
		wxLog::SetActiveTarget(new wxLogStderr);
		std::string locale;
		std::string theme;
		int c;
		while ((c = getopt_long(argc, argv, ":l:t:", longOptions, nullptr)) != -1) {
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
				default:
					std::cout << "Error parsing arguments" << std::endl;
					return false;
			}
		}

		if (locale.empty()) locale = std::setlocale(LC_CTYPE, "");
		auto *frame = new Frame(locale, theme);
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
