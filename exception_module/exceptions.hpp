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
            short _errorCode;
    public:
        SIEMException(std::string const &exMsg, short errCode) :
            _msg(exMsg), _errorCode(errCode) {}

        SIEMException(std::string &&exMsg, short errCode) :
            _msg(std::move(exMsg)), _errorCode(errCode) {}

        ~SIEMException() = default;

        const char *what() const noexcept
        {
            return _msg.c_str();
        }
        short getErrorCode() const noexcept
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

        DateTimeException(std::string const &exMsg, short errCode) :
            SIEMException(exMsg, errCode) {}

        DateTimeException(std::string &&exMsg, short errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~DateTimeException() = default;
    };

    class JsonException : public SIEMException
    {
    public:
        enum class JsonErrorCode : short
        {
            BAD_PATH = 1, BAD_NAME = 2, BAD_NODE = 3, EMPTY_CONTAINER = 4,
            JSON_STREAM_SYNTAX_ERROR = 5
        };

        JsonException(std::string const &exMsg, short errCode) :
            SIEMException(exMsg, errCode) {}

        JsonException(std::string &&exMsg, short errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~JsonException() = default;
    };

    class JsonStreamDeserializerException : public JsonException
    {
    private:
        char m_ReadedSymbol;
    public:
        JsonStreamDeserializerException(std::string const &exMsg, short errCode, char symbol = 0) :
            JsonException(exMsg, errCode), m_ReadedSymbol(symbol) {}

        JsonStreamDeserializerException(std::string &&exMsg, short errCode, char symbol = 0) :
            JsonException(std::move(exMsg), errCode), m_ReadedSymbol(symbol) {}

        ~JsonStreamDeserializerException() = default;

        char GetLastSymbol() const { return m_ReadedSymbol; }
    };

    class ConfigurationException : public SIEMException
    {
    public:
        enum ConfigErrorCode { BAD_SEARCH_STRUCTURE = 1, BAD_AGGR_STRUCTURE,
                                                       BAD_SUBNODE_LEVEL };

        ConfigurationException(std::string const &exMsg, short errCode) :
            SIEMException(exMsg, errCode) {}

        ConfigurationException(std::string &&exMsg, short errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~ConfigurationException() = default;
    };

    class DescriptionException : public SIEMException
    {
    public:
        enum DescriptionErrorCode { INVALID_SEARCH_CONFIG = 1, INVALID_AGGREGATION_CONFIG,
                                                                INVALID_VALUE_STRING, NOT_FOUND_JSONLOGFILE,
                                                                NOT_FOUND_NODE, INVALID_CONFIG_TYPE, INVALID_PARAMETER };

        DescriptionException(std::string const &exMsg, short errCode) :
            SIEMException(exMsg, errCode) {}

        DescriptionException(std::string &&exMsg, short errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~DescriptionException() = default;
    };

    class CorrelationException : public SIEMException
    {
    public:
        enum CorrelationErrorCode { INCORRECT_SEARCH_CATEGORY = 1, INCORRECT_SEARCH_STRING };

        CorrelationException(std::string const &exMsg, short errCode) :
            SIEMException(exMsg, errCode) {}

        CorrelationException(std::string &&exMsg, short errCode) :
            SIEMException(std::move(exMsg), errCode) {}

        ~CorrelationException() = default;
    };

    class AggregationException : public SIEMException
    {
    public:
        enum AggregationErrorCode { INVALID_GRABBER_TYPE = 1 , INVALID_GRABBER_SETTINGS_TYPE,
                                    CANNOT_CREATE_RECORD, INVALID_GRABBER_SERIALIZER_TYPE };

        AggregationException(std::string const &exMsg, short errCode) :
            SIEMException(exMsg, errCode) {};

        ~AggregationException() = default;
    };
}
#endif // EXCEPTIONS_HPP
