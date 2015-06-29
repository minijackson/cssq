#include "application.hpp"

#include "exceptions/fatal_exception.hpp"
#include "exceptions/cli_exception.hpp"
#include "exceptions/no_input_exception.hpp"
#include "exceptions/unknown_file_type_exception.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>

#include <getopt.h>
#include <htmlcxx/html/ParserDom.h>
#include <hcxselect.h>
#include <tinyxml2.h>

#include <sys/stat.h>

Application::Application(int argc, char* argv[])
	: argc(argc), argv(argv), execName(argv[0]) {
}

int Application::exec() {
	try {
		parseOpts();
	} catch (FatalException e) {
		std::cerr << e.what() << std::endl;
		return e.getCode().value();
	}

	switch(programMode) {
		case HELP:
			printHelp();
			return 0;
		case VERSION:
			printVersion();
			return 0;
		case NORMAL:
			try {
				select();
			} catch (FatalException e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return e.getCode().value();
			}
			break;
		default:
			std::cerr << "Fatal error: wrong value of program mode:" << std::endl;
			return -1;
	}

	return 0;
}

void Application::parseOpts() {
	if(argc <= 1) {
		throw CliException("You didn't specify a selector.\n"
				"Usage: " + execName + " [options] selector [files]");
	}

	static struct option longOptions[] = {
		{"help"   , no_argument, 0, 'h'},
		{"version", no_argument, 0, 'V'},
		{0        , 0          , 0,  0 }
	};

	int option, optionIndex;
	while((option = getopt_long(argc, argv, "hV",
	                            longOptions, &optionIndex))) {
		if(option == -1)
			break;

		switch(option) {
			case 'h':
				programMode = HELP;
				return;
			case 'V':
				programMode = VERSION;
				return;
			case '?':
			case ':':
				throw CliException("Usage: " + execName + " [options] selector [files]"
						"Try `" + execName + " --help' for more informations");
				return;
			default:
				exit(-1);
		}
	}

	if(optind == argc) {
		throw CliException("You didn't specify a selector.\n"
				"Usage: " + execName + " [options] selector [files]");
		return;
	}

	selector = std::string(argv[optind++]);

	if(optind != argc) {
		filenames.clear();
	}

	for(int i = optind ; i < argc ; ++i) {
		std::string filename = argv[i];
		if(filename == "-") {
			filenames.push_back("/dev/stdin");
		} else {
			filenames.push_back(filename);
		}
	}
}

void Application::printHelp() const {
	std::cout << "Usage: " << execName << " [options] [file]\n\n"
	             "Options:\n\n"
	             "  -h, --help         Display this help and exit.\n"
	             "  -V, --version      Display version information and exit.\n"
	          << std::endl;
}

void Application::printVersion() const {
	std::cout << "CSSQ version 0.2" << std::endl;
}

void Application::select() {
	for(std::string const& filename : filenames) {

		std::string fileContent;

		if(filename == "/dev/stdin") {
			std::ostringstream ss;
			for(std::string line ; std::getline(std::cin, line) ; ) {
				ss << line << "\n";
			}
			fileContent = ss.str();
		} else {
			struct stat st;
			if(stat(filename.c_str(), &st) < 0) {
				throw NoInputException(filename);
			}

			std::ifstream file(filename);

			if(file) {
				if(S_ISREG(st.st_mode)) {
					file.seekg(0, file.end);
					int size = file.tellg();
					file.seekg(0, file.beg);

					if(size == 0) {
						return;
					}

					fileContent.resize(size);
					char* fileContentBegin = &*fileContent.begin();

					file.read(fileContentBegin, size);
					file.close();
				} else if(S_ISFIFO(st.st_mode)) {
					std::ostringstream ss;

					std::string line;
					std::getline(file, line);

					while(!file.eof()) {
						ss << line << "\n";
						std::getline(file, line);
					}

					file.close();

					fileContent = ss.str();
				} else {
					file.close();
					throw UnknownFileTypeException();
				}

			} else {
				throw NoInputException(filename);
			}
		}

		htmlcxx::HTML::ParserDom parser;
		tree<htmlcxx::HTML::Node> dom = parser.parseTree(fileContent);
		hcxselect::Selector s(dom);

		try {
			s= s.select(selector);
		} catch (hcxselect::ParseException e) {
			std::cerr << "Parse error: " << e.what() << std::endl;
		} catch (...) {
			std::cerr << "Error." << std::endl;
		}

		for(auto it = s.begin() ; it != s.end() ; ++it) {
			std::cout << fileContent.substr((*it)->data.offset(), (*it)->data.length()) << std::endl;
		}
	}
}
