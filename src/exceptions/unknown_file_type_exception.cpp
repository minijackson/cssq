#include "unknown_file_type_exception.hpp"

UnknownFileTypeException::UnknownFileTypeException()
	: FatalException("Unknown file type",
			std::error_code(ENOTSUP, std::generic_category())) {
}
