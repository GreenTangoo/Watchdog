#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace siem_ex_space
{
	class SIEMExecption : public std::exception
	{
	protected:
		std::string msg;
		int errorCode;
	public:
		enum MainSIEMErrorCode { BAD_FILE = 0 };
		SIEMExecption(std::string const &exMsg, int errCode) : msg(exMsg), errorCode(errCode) {}
		SIEMExecption(std::string &&exMsg, int errCode) : msg(std::move(exMsg)), errorCode(errCode) {}
		~SIEMExecption() {}
		const char *what() const noexcept
		{
			return msg.c_str();
		}
		int getErrorCode() const noexcept
		{
			return errorCode;
		}
	};

	class DateTimeException : public SIEMExecption
	{
	public:
		enum DateTimeErrorCode { BAD_MONTH = 0,  BAD_DAY, BAD_HOUR, BAD_MINUTE,
								 BAD_SECOND, BAD_MILLISECOND, INCOMPITABLE_MONTH_DAY,
								 BAD_FORMAT_SYMBOL, BAD_FORMAT_STRING, INCOMPITABLE_STRINGS,
								 BAD_TIME_STRING, BAD_MATH_OPERATION, BAD_DATETIME_FORMAT };
		DateTimeException(std::string const &exMsg, int errCode) : SIEMExecption(exMsg, errCode) {}
		DateTimeException(std::string &&exMsg, int errCode) : SIEMExecption(std::move(exMsg), errCode) {}
		~DateTimeException() {}
	};

	class JsonException : public SIEMExecption
	{
	public:
		enum JsonErrorCode { BAD_PATH = 0, BAD_NAME };
		JsonException(std::string const &exMsg, int errCode) : SIEMExecption(exMsg, errCode) {}
		JsonException(std::string &&exMsg, int errCode) : SIEMExecption(std::move(exMsg), errCode) {}
		~JsonException() {}
	};
}
#endif // EXCEPTIONS_HPP