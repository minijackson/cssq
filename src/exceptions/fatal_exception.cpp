#include "fatal_exception.hpp"

FatalException::FatalException(const char* errorMsg, std::error_code errorCode)
	: errorMsg(errorMsg), errorCode(errorCode) {
}

const char* FatalException::what() const noexcept {
	return errorMsg;
}

std::error_code FatalException::getCode() const {
	return errorCode;
}
