#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace siem_ex_space
{
	class SIEMExecption : public std::exception
	{
	protected:
		std::string _msg;
		int _errorCode;
	public:
		enum MainSIEMErrorCode { BAD_FILE = 0 };
		SIEMExecption(std::string const &exMsg, int errCode) : 
			_msg(exMsg), _errorCode(errCode) {}

		SIEMExecption(std::string &&exMsg, int errCode) : 
			_msg(std::move(exMsg)), _errorCode(errCode) {}

		~SIEMExecption() {}
		const char *what() const noexcept
		{
			return _msg.c_str();
		}
		int getErrorCode() const noexcept
		{
			return _errorCode;
		}
	};

	class DateTimeException : public SIEMExecption
	{
	public:
		enum DateTimeErrorCode { BAD_MONTH = 0,  BAD_DAY, BAD_HOUR, BAD_MINUTE,
								 BAD_SECOND, BAD_MILLISECOND, INCOMPITABLE_MONTH_DAY,
								 BAD_FORMAT_SYMBOL, BAD_FORMAT_STRING, INCOMPITABLE_STRINGS,
								 BAD_TIME_STRING, BAD_MATH_OPERATION, BAD_DATETIME_FORMAT };
		DateTimeException(std::string const &exMsg, int errCode) :
			SIEMExecption(exMsg, errCode) {}

		DateTimeException(std::string &&exMsg, int errCode) : 
			SIEMExecption(std::move(exMsg), errCode) {}

		~DateTimeException() {}
	};

	class JsonException : public SIEMExecption
	{
	public:
		enum JsonErrorCode { BAD_PATH = 0, BAD_NAME, BAD_NODE, EMPTY_CONTAINER };
		JsonException(std::string const &exMsg, int errCode) : 
			SIEMExecption(exMsg, errCode) {}

		JsonException(std::string &&exMsg, int errCode) : 
			SIEMExecption(std::move(exMsg), errCode) {}

		~JsonException() {}
	};

	class ConfigurationException : public SIEMExecption
	{
	public:
		enum ConfigErrorCode { BAD_SEARCH_STRUCTURE = 0, BAD_AGGR_STRUCTURE };
		ConfigurationException(std::string const &exMsg, int errCode) :
			SIEMExecption(exMsg, errCode) {}

		ConfigurationException(std::string &&exMsg, int errCode) : 
			SIEMExecption(std::move(exMsg), errCode) {}

		~ConfigurationException() {}
	};

	class SyntaxAnalyzeException : public SIEMExecption
	{
	public:
		enum SyntaxAnalyzerErrorCode { BAD_COMPARE_PARAMETER = 0, 
									   BAD_RELATIONSHIP_PARAMETER, 
									   BAD_SUBNODE_LEVEL };
		SyntaxAnalyzeException(std::string const &exMsg, int errCode) : 
			SIEMExecption(exMsg, errCode) {}

		SyntaxAnalyzeException(std::string &&exMsg, int errCode) :
			SIEMExecption(std::move(exMsg), errCode) {}

		~SyntaxAnalyzeException() {}
	};

	class DescriptionException : public SIEMExecption
	{
	public:
		enum DescriptionErrorCode { INVALID_SEARCH_CONFIG = 0, INVALID_AGGREGATION_CONFIG,
									INVALID_VALUE_STRING, NOT_FOUND_JSONLOGFILE, 
									NOT_FOUND_NODE, INVALID_CONFIG_TYPE, INVALID_PARAMETER };
		DescriptionException(std::string const &exMsg, int errCode) : 
			SIEMExecption(exMsg, errCode) {}

		DescriptionException(std::string &&exMsg, int errCode) : 
			SIEMExecption(std::move(exMsg), errCode) {}

		~DescriptionException() {}
	};
}
#endif // EXCEPTIONS_HPP