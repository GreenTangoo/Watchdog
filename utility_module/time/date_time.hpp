#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

#include <cstddef>
#include <cctype>
#include <string>
#include <map>
#include <algorithm>

namespace utility_space
{
	class DateTime
	{
	private:
		enum monthType { JAN = 1, FEB = 2,  MARCH = 3, APRIL = 4,
						 MAY = 5, JUN = 6,  JUL = 7,   AUG = 8,
						 SEP = 9, OCT = 10, NOV = 11,  DEC = 12};
		int years;
		int months;
		int days;
		int hours;
		int minutes;
		int seconds;
		int milliseconds;
	private:
		enum compareType { EQUAL = 0, FIRST_BIGGER = 1, SECOND_BIGGER = -1 };
		enum formatSymbolType { Y = 'Y', M = 'M', D = 'D', h = 'h', m = 'm', s = 's', l = 'l'};
		std::map<monthType, int> monthDaysAmountMap;
	private:
		void initializeDaysMap();
		bool isValidDateTime();
		bool isSpecialYear(int year);
		compareType compare(DateTime const &first, DateTime const &second);
		std::string getSubTime(char subTimeSymbol);
		void putSubTime(std::string timeStr, formatSymbolType symbolTimeType);
		bool isNumberString(std::string str);
	public:
		DateTime();
		DateTime(std::string const &timeString, std::string const &formatString);
		DateTime(DateTime const &other);
		DateTime(DateTime &&other) = delete;
		~DateTime();
		DateTime const& operator=(DateTime const &other);
		DateTime const& operator=(DateTime &&other) = delete;
		bool operator==(DateTime const &other);
		bool operator<(DateTime const &other);
		bool operator>(DateTime const &other);
		bool operator<=(DateTime const &other);
		bool operator>=(DateTime const &other);
		bool operator!=(DateTime const &other);
		std::string getFormatTime(std::string const &formatString);
		void putFormatTime(std::string const &stringTime, std::string const &formatString);
		void add(DateTime const &other);
		void add(std::string const &timeString, std::string const &formatString);
		void sub(DateTime const &other);
		void sub(std::string const &timeString, std::string const &formatString);
		int getYears() const;
		int getMonths() const;
		int getDays() const;
		int getHours() const;
		int getMinutes() const;
		int getSeconds() const;
		int getMilliseconds() const;
		void setYears(int value);
		void setMonths(int value);
		void setDays(int value);
		void setHours(int value);
		void setMinutes(int value);
		void setSeconds(int value);
		void setMilliseconds(int value);
	};
}

#endif // DATE_TIME_HPP