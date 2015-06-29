#include "cli_exception.hpp"

CliException::CliException()
	: FatalException("Error while parsing CLI arguments",
			std::error_code(EINVAL, std::generic_category())) {
}

CliException::CliException(const char* errorMsg)
	: FatalException(errorMsg,
			std::error_code(EINVAL, std::generic_category())) {
}

CliException::CliException(std::string errorMsg)
	: FatalException(errorMsg.c_str(),
			std::error_code(EINVAL, std::generic_category())) {
}
