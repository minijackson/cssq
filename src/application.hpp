#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <list>
#include <string>

class Application {
public:
	Application(int argc, char* argv[]);
	int exec();

private:
	void parseOpts(int argc, char* argv[]);

	void printHelp() const;
	void printVersion() const;

	int select();

	std::string execName;
	std::string selector;
	std::list<std::string> filenames = {"/dev/stdin"};
	enum ProgramMode {
		NORMAL,
		HELP,
		VERSION,
		CLI_ERROR
	} programMode = NORMAL;
};

#endif
