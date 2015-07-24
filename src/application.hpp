#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <list>
#include <string>

class Application {
public:
	Application(int argc, char* argv[]);
	int exec();

private:
	void parseOpts();

	void printHelp() const;
	void printVersion() const;

	void select();

	int argc;
	char** argv;

	std::string execName;
	std::string selector;
	std::list<std::string> filenames = {"/dev/stdin"};
	enum ProgramMode {
		NORMAL,
		HELP,
		VERSION
	} programMode = NORMAL;
	bool selectInnerText = false;
};

#endif
