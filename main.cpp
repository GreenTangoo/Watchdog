#include <iostream>
#include <string>

#include "utility_module/time/date_time.hpp"

using namespace utility_space;

int main()
{
	DateTime timeObj("4.9.1998", "%D.%M.%Y");
	std::string timeStr = timeObj.getFormatTime("%Y:%M:%D");

	std::cout << timeStr << std::endl;

	return 0;
}