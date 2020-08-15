#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace siem_ex_space
{
	class SIEMExcpetion : public std::exception
	{
	protected:
		std::string msg;
		int errorCode;
	public:
		SIEMExcpetion(std::string const &exMsg, int errCode) : msg(exMsg), errorCode(errCode) {}
		SIEMExcpetion(std::string &&exMsg, int errCode) : msg(std::move(exMsg)), errorCode(errCode) {}
		~SIEMExcpetion() {}
		const char *what() const noexcept
		{
			return msg.c_str();
		}
		int getErrorCode() const noexcept
		{
			return errorCode;
		}
	};

	class DateTimeException : public SIEMExcpetion
	{
	public:
		enum DateTimeErrorCode { BAD_MONTH = 0,  BAD_DAY = 1, BAD_HOUR = 2, BAD_MINUTE = 3,
								 BAD_SECOND = 4, BAD_MILLISECOND = 5, INCOMPITABLE_MONTH_DAY = 6,
								 BAD_FORMAT_SYMBOL = 7, BAD_FORMAT_STRING = 8, INCOMPITABLE_STRINGS = 9,
								 BAD_TIME_STRING = 10, BAD_MATH_OPERATION = 11, BAD_DATETIME_FORMAT = 12 };
		DateTimeException(std::string const &exMsg, int errCode) : SIEMExcpetion(exMsg, errCode) {}
		DateTimeException(std::string &&exMsg, int errCode) : SIEMExcpetion(std::move(exMsg), errCode) {}
		~DateTimeException() {}
	};
}
#endif // EXCEPTIONS_HPP