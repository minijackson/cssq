#ifndef FATAL_EXCEPTION_HPP
#define FATAL_EXCEPTION_HPP

#include <exception>
#include <system_error>

/*!
 * \brief Basically an exception with an error code.
 */
class FatalException : public std::exception {
public:
	FatalException(const char* errorMsg, std::error_code errorCode);
	FatalException(const FatalException& other) = default;
	virtual const char* what() const noexcept;
	std::error_code getCode() const;
private:
	const char* errorMsg;
	std::error_code errorCode;
};

#endif
