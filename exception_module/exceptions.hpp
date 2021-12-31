#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace siem_ex_space
{
    class SIEMException : public std::exception
	{
	protected:
		std::string _msg;
		int _errorCode;
	public:
		enum MainSIEMErrorCode { BAD_FILE = 0 };
        SIEMException(std::string const &exMsg, int errCode) :
			_msg(exMsg), _errorCode(errCode) {}

        SIEMException(std::string &&exMsg, int errCode) :
			_msg(std::move(exMsg)), _errorCode(errCode) {}

        ~SIEMException() = default;
		const char *what() const noexcept
		{
			return _msg.c_str();
		}
		int getErrorCode() const noexcept
		{
			return _errorCode;
		}
	};

    class DateTimeException : public SIEMException
	{
	public:
		enum DateTimeErrorCode { BAD_MONTH = 1,  BAD_DAY, BAD_HOUR, BAD_MINUTE,
								 BAD_SECOND, BAD_MILLISECOND, INCOMPITABLE_MONTH_DAY,
								 BAD_FORMAT_SYMBOL, BAD_FORMAT_STRING, INCOMPITABLE_STRINGS,
								 BAD_TIME_STRING, BAD_MATH_OPERATION, BAD_DATETIME_FORMAT };
		DateTimeException(std::string const &exMsg, int errCode) :
            SIEMException(exMsg, errCode) {}

		DateTimeException(std::string &&exMsg, int errCode) : 
            SIEMException(std::move(exMsg), errCode) {}

        ~DateTimeException() = default;
	};

    class JsonException : public SIEMException
	{
	public:
		enum JsonErrorCode { BAD_PATH = 1, BAD_NAME, BAD_NODE, EMPTY_CONTAINER };
		JsonException(std::string const &exMsg, int errCode) : 
            SIEMException(exMsg, errCode) {}

		JsonException(std::string &&exMsg, int errCode) : 
            SIEMException(std::move(exMsg), errCode) {}

        ~JsonException() = default;
	};

    class ConfigurationException : public SIEMException
	{
	public:
		enum ConfigErrorCode { BAD_SEARCH_STRUCTURE = 1, BAD_AGGR_STRUCTURE, 
							   BAD_SUBNODE_LEVEL };
		ConfigurationException(std::string const &exMsg, int errCode) :
            SIEMException(exMsg, errCode) {}

		ConfigurationException(std::string &&exMsg, int errCode) : 
            SIEMException(std::move(exMsg), errCode) {}

        ~ConfigurationException() = default;
	};

    class DescriptionException : public SIEMException
	{
	public:
		enum DescriptionErrorCode { INVALID_SEARCH_CONFIG = 1, INVALID_AGGREGATION_CONFIG,
									INVALID_VALUE_STRING, NOT_FOUND_JSONLOGFILE, 
									NOT_FOUND_NODE, INVALID_CONFIG_TYPE, INVALID_PARAMETER };
		DescriptionException(std::string const &exMsg, int errCode) : 
            SIEMException(exMsg, errCode) {}

		DescriptionException(std::string &&exMsg, int errCode) : 
            SIEMException(std::move(exMsg), errCode) {}

        ~DescriptionException() = default;
	};

    class CorrelationException : public SIEMException
	{
	public:
        enum CorrelationErrorCode { INCORRECT_SEARCH_CATEGORY = 1, INCORRECT_SEARCH_STRING };
		CorrelationException(std::string const &exMsg, int errCode) :
            SIEMException(exMsg, errCode) {}

		CorrelationException(std::string &&exMsg, int errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~CorrelationException() = default;
	};

    class ThreadPoolException : public SIEMException
    {
    public:
        enum ThreadPoolErrorCode { EMPTY_TASK_POOL = 0 };
        ThreadPoolException(std::string const &exMsg, int errCode) :
            SIEMException(exMsg, errCode) {}

        ThreadPoolException(std::string &&exMsg, int errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~ThreadPoolException() = default;
    };
}
#endif // EXCEPTIONS_HPP
