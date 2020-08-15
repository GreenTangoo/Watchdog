#include <string>
#include <iostream>

#include <cxxtest/TestSuite.h>

#include "../utility_module/time/date_time.hpp"
#include "../utility_module/time/date_time.cpp"
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
			std::cout << "Start PutGetTime test" << std::endl;
			DateTime timeObj("4.9.1998", "%D.%M.%Y");
			std::string timeStr = timeObj.getFormatTime("%Y:%M:%D");
			TS_ASSERT_EQUALS(timeStr, "1998.9.4");
		}
		catch(DateTimeException const &ex)
		{
			std::string exStr = "PutGet exception catched! Code: " + std::to_string(ex.getErrorCode());
			TS_FAIL(exStr);
		}
	}
};