#include "application.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <sysexits.h>
#include <getopt.h>
#include <htmlcxx/html/ParserDom.h>
#include <hcxselect.h>
#include <tinyxml2.h>

Application::Application(int argc, char* argv[]) : execName(argv[0]) {
	if(argc > 1) {
		parseOpts(argc, argv);
	}
}

int Application::exec() {
	switch(programMode) {
		case HELP:
			printHelp();
			return 0;
		case VERSION:
			printVersion();
			return 0;
		case CLI_ERROR:
			std::cerr << "Error while parsing command line options. Try `"
			          << execName << " --help' for more informations" << std::endl;
			return EX_USAGE;
		case NORMAL:
			select();
			return 0;
		default:
			std::cerr << "Fatal error: wrong value of program mode" << std::endl;
	}

	return 0;
}

void Application::parseOpts(int argc, char* argv[]) {
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
				break;
			case 'V':
				programMode = VERSION;
				break;
			case '?':
			case ':':
				programMode = CLI_ERROR;
				break;
			default:
				exit(-1);
		}
	}

	if(optind == argc) {
		std::cerr << "You must specify a selector." << std::endl;
		exit(64);
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
	std::cout << "CSSQ version 1.0~alpha" << std::endl;
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
			std::ifstream file(filename, std::ifstream::binary);

			if(file) {
				file.seekg(0, file.end);
				int size = file.tellg();
				file.seekg(0, file.beg);

				if(size > 0) {
					fileContent.resize(size);
					char* begin = &*fileContent.begin();

					file.read(begin, size);
					file.close();
				} else if(size == -1) {
					// TODO
					std::cerr << "To be implemented (pipes)" << std::endl;
					eixt(EX_SOFTWARE);
				} else {
					std::cerr << "Error: wrong file size" << std::endl;
					exit(EX_IOERR);
				}

			} else {
				std::cerr << "Error while opening file: " << filename << std::endl;
				exit(EX_NOINPUT);
			}
		}

		htmlcxx::HTML::ParserDom parser;
		tree<htmlcxx::HTML::Node> dom = parser.parseTree(fileContent);
		hcxselect::Selector s(dom);

		try {
			s= s.select(selector);
		} catch (hcxselect::ParseException e) {
			std::cerr << "Parse error." << std::endl;
		} catch (...) {
			std::cerr << "Error." << std::endl;
		}

		for(auto it = s.begin() ; it != s.end() ; ++it) {
			std::cout << fileContent.substr((*it)->data.offset(), (*it)->data.length()) << std::endl;
		}

	}
}
