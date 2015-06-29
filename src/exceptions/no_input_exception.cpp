#include "no_input_exception.hpp"

NoInputException::NoInputException()
	: FatalException("No such file or directory",
			std::error_code(ENOENT, std::generic_category())) {
}

NoInputException::NoInputException(std::string filename)
	: FatalException(("No such file or directory: '" + filename + "'").c_str(),
			std::error_code(ENOENT, std::generic_category())) {
}
