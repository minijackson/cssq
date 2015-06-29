#ifndef CLI_EXCEPTION_HPP
#define CLI_EXCEPTION_HPP

#include "fatal_exception.hpp"

#include <string>

class CliException : public FatalException {
public:
	CliException();
	explicit CliException(const char* errorMsg);
	explicit CliException(std::string errorMsg);
};

#endif
