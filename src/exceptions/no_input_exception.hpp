#ifndef NO_INPUT_EXCEPTION_HPP
#define NO_INPUT_EXCEPTION_HPP

#include "fatal_exception.hpp"

#include <string>

class NoInputException : public FatalException {
public:
	NoInputException();
	explicit NoInputException(std::string filename);
};

#endif
