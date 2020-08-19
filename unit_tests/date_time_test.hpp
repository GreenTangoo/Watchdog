#ifndef DATE_TIME_TEST_HPP
#define DATE_TIME_TEST_HPP

#include <string>

#include <cxxtest/TestSuite.h>

#include "../utility_module/date_time.hpp"
#include "../utility_module/date_time.cpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

class DateTimeTest : public CxxTest::TestSuite
{
public:
	void testPutGetTime(void)
	{
		try
		{
			TS_TRACE("Start PutGet test");
			DateTime timeObj("4.9.1998", "%D.%M.%Y");
			std::string timeStr = timeObj.getFormatTime("%Y.%M.%D");
			TS_ASSERT_EQUALS(timeStr, "1998.9.4");
		}
		catch(DateTimeException const &ex)
		{
			std::string exStr = "PutGet exception catched! Code: " + std::to_string(ex.getErrorCode());
			TS_FAIL(exStr);
		}
	}

	void testMathTimeOperations(void)
	{
		try
		{
			TS_TRACE("Start MathTimeOperations test");
			DateTime timeObj("1998:09:04:17:30:50", "%Y:%M:%D:%h:%m:%s");
			DateTime addingTime("30:5", "%m:%s");

			timeObj.add(addingTime);
			std::string timeStr = timeObj.getFormatTime("%Y:%M:%D:%h:%m:%s");
			TS_ASSERT_EQUALS(timeStr, "1998:9:4:18:0:55");
		}
		catch(DateTimeException const &ex)
		{
			std::string exStr = "MathTimeOperations exception catched! Code: " + std::to_string(ex.getErrorCode());
			TS_FAIL(exStr);
		}
	}

	void testCompareTimeOperations(void)
	{
		TS_TRACE("Start CompareTimeOperations test");
		DateTime time1("1998:9:4", "%Y:%M:%D");
		DateTime time2("1998:9:4", "%Y:%M:%D");
		DateTime time3("1998:9:5", "%Y:%M:%D");

		TS_ASSERT_EQUALS(time1, time2);
		TS_ASSERT_LESS_THAN_EQUALS(time1, time2);
		TS_ASSERT_LESS_THAN_EQUALS(time1, time3);
		TS_ASSERT_DIFFERS(time1, time3);
	}

	void testGetSetOperations(void)
	{
		TS_TRACE("Start GetSetOperations test");
		DateTime timeObj;

		timeObj.setYears(2000);
		timeObj.setMonths(12);
		timeObj.setDays(21);
		timeObj.setHours(17);
		timeObj.setMinutes(21);
		timeObj.setSeconds(59);
		timeObj.setMilliseconds(555);

		TS_ASSERT_EQUALS(timeObj.getYears(), 2000);
		TS_ASSERT_EQUALS(timeObj.getMonths(), 12);
		TS_ASSERT_EQUALS(timeObj.getDays(), 21);
		TS_ASSERT_EQUALS(timeObj.getHours(), 17);
		TS_ASSERT_EQUALS(timeObj.getMinutes(), 21);
		TS_ASSERT_EQUALS(timeObj.getSeconds(), 59);
		TS_ASSERT_EQUALS(timeObj.getMilliseconds(), 555);
	}
};

#endif // DATE_TIME_TEST_HPP