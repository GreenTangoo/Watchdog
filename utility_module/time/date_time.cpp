#include "date_time.hpp"

#include "../../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;


/*--------------------------------PUBLIC----------------------------------------*/
DateTime::DateTime() : years(0), months(0), days(0), hours(0), minutes(0), seconds(0), milliseconds(0)
{
	initializeDaysMap();
}

DateTime::DateTime(std::string const &timeString, std::string const &formatString) :
					years(0), months(0), days(0), hours(0), minutes(0), seconds(0), milliseconds(0)
{
	putFormatTime(timeString, formatString);
	initializeDaysMap();
}

DateTime::DateTime(DateTime const &other) : years(other.years), months(other.months), days(other.days),
	 					hours(other.days), minutes(other.days), seconds(other.seconds), milliseconds(other.milliseconds)
{
	initializeDaysMap();
}

DateTime::~DateTime()
{

}

DateTime const& DateTime::operator=(DateTime const &other)
{
	if(this != &other)
	{
		this->years = other.years;
		this->months = other.months;
		this->days = other.days;
		this->hours = other.hours;
		this->minutes = other.minutes;
		this->seconds = other.seconds;
		this->milliseconds = other.milliseconds;
		initializeDaysMap();
	}

	return *this;
}

bool DateTime::operator==(DateTime const &other)
{
	return compare(*this, other) == EQUAL ? true : false;
}

bool DateTime::operator!=(DateTime const &other)
{
	return compare(*this, other) != EQUAL ? true : false;
}

bool DateTime::operator>(DateTime const &other)
{
	return compare(*this, other) == FIRST_BIGGER ? true : false;
}

bool DateTime::operator<(DateTime const &other)
{
	return compare(*this, other) == SECOND_BIGGER ? true : false;
}

bool DateTime::operator<=(DateTime const &other)
{
	int result = compare(*this, other);
	return (result == EQUAL) || (result == SECOND_BIGGER);
}

bool DateTime::operator>=(DateTime const &other)
{
	int result = compare(*this, other);
	return (result = EQUAL) || (result == FIRST_BIGGER);
}

std::string DateTime::getFormatTime(std::string const &formatString)
{
	std::string resultTimeString;

	size_t formatStrLength = formatString.size();

	for(size_t i(0); i < formatStrLength; i++)
	{
		if(formatString[i] == '%')
		{
			if(i >= formatStrLength - 1)
			{
				throw DateTimeException("Incorrect format string format", DateTimeException::BAD_FORMAT_STRING);
			}

			char formatSymbol = formatString[i + 1];
			std::string subTimeStr = getSubTime(formatSymbol);
			resultTimeString += subTimeStr;
			i++;
			continue;
		}
		resultTimeString.push_back(formatString[i]);
	}

	return resultTimeString;
}

void DateTime::putFormatTime(std::string const &timeString, std::string const &formatString)
{
	size_t timeStringLen = timeString.size();
	size_t formatStrLen = formatString.size();
	size_t formatSymbolPos = 0;
	size_t timeStrBeginSubTimePos = 0;

	while(true)
	{
		formatSymbolPos = formatString.find('%', formatSymbolPos);

		if(formatSymbolPos == std::string::npos)
			return;
		
		if(formatSymbolPos == formatStrLen - 1)
		{
			throw DateTimeException("Bad format string format", DateTimeException::BAD_FORMAT_STRING);
		}

		formatSymbolType timeSymbol = (formatSymbolType)formatString[formatSymbolPos + 1];
		if(formatSymbolPos + 2 >= formatStrLen)
		{
			std::string subTimeStr = timeString.substr(timeStrBeginSubTimePos, 
															timeStringLen - timeStrBeginSubTimePos);
			putSubTime(subTimeStr, timeSymbol);
			return;
		}
		else
		{
			char behindFormatSymbol = formatString[formatSymbolPos + 2];
			size_t findPos = timeStrBeginSubTimePos;

			size_t timeStringEndPos = timeString.find(behindFormatSymbol, findPos);
			if(timeStringEndPos == std::string::npos)
			{
				throw DateTimeException("Time string ins't equal format string", DateTimeException::INCOMPITABLE_STRINGS);
			}

			std::string subTimeStr = timeString.substr(timeStrBeginSubTimePos, 
															timeStringEndPos - timeStrBeginSubTimePos);

			putSubTime(subTimeStr, timeSymbol);
			formatSymbolPos++;
			timeStrBeginSubTimePos = timeStringEndPos + 1;
		}
	}

	if(isValidDateTime() == false)
		throw DateTimeException("Incorrect DateTime class format", DateTimeException::BAD_DATETIME_FORMAT);
}

void DateTime::add(DateTime const &other)
{
	int additionalSeconds = (this->milliseconds + other.milliseconds) / 1000;
	this->milliseconds = (this->milliseconds + other.milliseconds) % 1000;

	int additionalMinutes = (this->seconds + other.seconds + additionalSeconds) / 60;
	this->seconds = (this->seconds + other.seconds + additionalSeconds) % 60;

	int additionalHours = (this->minutes + other.minutes + additionalMinutes) / 60;
	this->minutes = (this->minutes + other.minutes + additionalMinutes) % 60;

	int additionalDays = (this->hours + other.hours + additionalHours) / 24;
	this->hours = (this->hours + other.hours + additionalHours) % 24;

	int maxMonthDays = monthDaysAmountMap[(monthType)months];
	int monthIter = this->months;
	int additionalMonths = 0;
	while(true)
	{
		int monthDays = monthDaysAmountMap[(monthType)monthIter];
		if((additionalDays - monthDays) <= 0)
		{
			break;
		}
		else
		{
			additionalDays -= monthDays;
			additionalMonths++;
			monthIter = (monthIter == 12 ? 1 : monthIter + 1);
		}
	}
	this->days = (this->days + other.days + additionalDays) % maxMonthDays;

	int additionalYears = (this->months + other.months + additionalMonths) / 12;
	this->months = (this->months + other.months + additionalMonths) % 12;

	this->years = this->years + other.years + additionalYears;
}

void DateTime::add(std::string const &timeString, std::string const &formatString)
{
	DateTime newDateTime(timeString, formatString);
	this->add(newDateTime);
}

void DateTime::sub(DateTime const &other)
{
	this->milliseconds -= other.milliseconds;
	if(this->milliseconds < 0)
	{
		this->milliseconds += 1000;
		this->seconds--;
	}

	this->seconds -= other.seconds;
	if(this->seconds < 0)
	{
		this->seconds += 60;
		this->minutes--;
	}

	this->minutes -= other.minutes;
	if(this->minutes < 0)
	{
		this->minutes += 60;
		this->hours--;
	}

	this->hours -= other.hours;
	if(this->hours < 0)
	{
		this->hours += 24;
		this->days--;
	}

	this->days -= other.days;
	if(this->days < 0)
	{
		this->days += monthDaysAmountMap[(monthType)months];
		this->months--;
	}

	this->months -= other.months;
	if(this->months < 0)
	{
		this->months += 12;
		this->years--;
	}

	if(this->years < 0)
		throw DateTimeException("Date result below zero", DateTimeException::BAD_MATH_OPERATION);
}

void DateTime::sub(std::string const &timeString, std::string const &formatString)
{
	DateTime newDateTime(timeString, formatString);
	this->sub(newDateTime);
}

int DateTime::getYears() const
{
	return years;
}

int DateTime::getMonths() const
{
	return months;
}

int DateTime::getDays() const
{
	return days;
}

int DateTime::getHours() const
{
	return hours;
}

int DateTime::getMinutes() const
{
	return minutes;
}

int DateTime::getSeconds() const
{
	return seconds;
}

int DateTime::getMilliseconds() const
{
	return milliseconds;
}

void DateTime::setYears(int value)
{
	years = value;
}

void DateTime::setMonths(int value)
{
	if(value > 12)
		throw DateTimeException("Month number bigger than 12", DateTimeException::BAD_MONTH);
	
	int maxDays = monthDaysAmountMap[(monthType)value];
	if(this->days > maxDays)
		throw DateTimeException("Amount days in input month are bigger than stored in class", 
								DateTimeException::INCOMPITABLE_MONTH_DAY);

	months = value;
}

void DateTime::setDays(int value)
{
	int maxDays = monthDaysAmountMap[(monthType)this->months];
	if(value > maxDays)
		throw DateTimeException("Day number bigger than " + std::to_string(maxDays), DateTimeException::BAD_DAY);
	days = value;
}

void DateTime::setHours(int value)
{
	if(value > 24)
		throw DateTimeException("Hours number bigger than 24", DateTimeException::BAD_HOUR);
	hours = value;
}

void DateTime::setMinutes(int value)
{
	if(value > 60)
		throw DateTimeException("Minutes number bigger than 60", DateTimeException::BAD_MINUTE);
	minutes = value;
}

void DateTime::setSeconds(int value)
{
	if(value > 60)
		throw DateTimeException("Seconds number bigger than 60", DateTimeException::BAD_SECOND);
	seconds = value;
}

void DateTime::setMilliseconds(int value)
{
	if(value > 1000)
		throw DateTimeException("Milliseconds number bigger than 1000", DateTimeException::BAD_MILLISECOND);
	milliseconds = value;
}

/*---------------------------------PRIVATE----------------------------*/
DateTime::compareType DateTime::compare(DateTime const &first, DateTime const &second)
{
	if(first.years != second.years)
		return first.years > second.years ? FIRST_BIGGER : SECOND_BIGGER;
	if(first.months != second.months)
		return first.months > second.years ? FIRST_BIGGER : SECOND_BIGGER;
	if(first.days != second.days)
		return first.days > second.days ? FIRST_BIGGER : SECOND_BIGGER;
	if(first.hours != second.hours)
		return first.hours > second.hours ? FIRST_BIGGER : SECOND_BIGGER;
	if(first.minutes != second.minutes)
		return first.minutes > second.minutes ? FIRST_BIGGER : SECOND_BIGGER;
	if(first.seconds != second.seconds)
		return first.seconds > second.seconds ? FIRST_BIGGER : SECOND_BIGGER;
	if(first.milliseconds != second.milliseconds)
		return first.milliseconds > second.milliseconds ? FIRST_BIGGER : SECOND_BIGGER;
	return EQUAL;
}

bool DateTime::isValidDateTime()
{
	int maxDays = monthDaysAmountMap[(monthType)months];
	if((years < 0)                             ||
	   (months < 0 || months > 12)             ||
	   (days < 0 || days > maxDays)            ||
	   (hours < 0 || hours > 23)               ||
	   (minutes < 0 || minutes > 59)           ||
	   (seconds < 0 || seconds > 59)           ||
	   (milliseconds < 0 || milliseconds > 999))
	{
		return false;
	}

	return true;
}

void DateTime::initializeDaysMap()
{
	monthDaysAmountMap[JAN] = 31;
	monthDaysAmountMap[FEB] = (isSpecialYear(this->years) == true ? 29 : 28);
	monthDaysAmountMap[MARCH] = 31;
	monthDaysAmountMap[APRIL] = 30;
	monthDaysAmountMap[MAY] = 31;
	monthDaysAmountMap[JUN] = 30;
	monthDaysAmountMap[JUL] = 31;
	monthDaysAmountMap[AUG] = 31;
	monthDaysAmountMap[SEP] = 30;
	monthDaysAmountMap[OCT] = 31;
	monthDaysAmountMap[NOV] = 30;
	monthDaysAmountMap[DEC] = 31;
}

bool DateTime::isSpecialYear(int year)
{
	if((year % 4 != 0) || (year % 100 == 0 && year % 400 != 0))
		return false;
	return true;
}

std::string DateTime::getSubTime(char formatSymbol)
{
	switch(formatSymbol)
	{
	case Y:
		return std::to_string(this->years);
		break;
	case M:
		return std::to_string(this->months);
		break;
	case D:
		return std::to_string(this->days);
		break;
	case h:
		return std::to_string(this->hours);
		break;
	case m:
		return std::to_string(this->minutes);
		break;
	case s:
		return std::to_string(this->seconds);
		break;
	case l:
		return std::to_string(this->milliseconds);
		break;
	default:
		throw DateTimeException("Bad symol in format string: " + formatSymbol, DateTimeException::BAD_FORMAT_SYMBOL);
	}
}

void DateTime::putSubTime(std::string timeStr, formatSymbolType symbolTimeType)
{
	if(!isNumberString(timeStr))
		throw DateTimeException("Incorrect sub time string format", DateTimeException::BAD_TIME_STRING);

	int numReprStr = std::stoul(timeStr);

	switch(symbolTimeType)
	{
	case Y:
		this->years = numReprStr;
		break;
	case M:
		this->months = numReprStr;
		break;
	case D:
		this->days = numReprStr;
		break;
	case h:
		this->hours = numReprStr;
		break;
	case m:
		this->minutes = numReprStr;
		break;
	case s:
		this->seconds = numReprStr;
		break;
	case l:
		this->milliseconds = numReprStr;
		break;
	default:
		throw DateTimeException("Bad symol in format string: " + symbolTimeType, DateTimeException::BAD_FORMAT_SYMBOL);
	}
}

bool DateTime::isNumberString(std::string str)
{
	for(size_t i(0); i < str.size(); i++)
	{
		if(!std::isdigit(str[i]))
			return false;
	}
	return true;
}