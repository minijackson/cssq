#ifndef UNKNOWN_FILE_TYPE_EXCEPTION_HPP
#define UNKNOWN_FILE_TYPE_EXCEPTION_HPP

#include "fatal_exception.hpp"

class UnknownFileTypeException : public FatalException {
public:
	UnknownFileTypeException();
};

#endif
