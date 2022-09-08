#include <iostream>
#include <string>
#include <atomic>
#include <csignal>

#include "siem_startup.hpp"
#include "utility_module/json_proc.hpp"
#include "aggregation_module/aggregation_module_initializer.hpp"

using namespace utility_space;
using namespace main_siem_space;
using namespace aggregation_space;

#define MAIN_OPTIONS_PATH "configs/options.json"

void interrupt_handler(int sig)
{
	std::string answer;

	std::cout << "Are you sure to close?[Y/N]" << std::endl;
	std::getline(std::cin, answer);

	if((answer == "Y") || (answer == "y"))
	{
		std::terminate();
	}
	else if((answer == "N") || (answer == "n"))
	{
		return;
	}
	else
	{
		std::cout << "Incorrect input." << std::endl;
		return;
	}
}

int main(int argc, char** argv)
{
    signal(SIGINT, interrupt_handler);

    JsonFileDeserializer reader(MAIN_OPTIONS_PATH);

    IJsonContainerPtr pConfig = reader.Read();
    SettingsSIEM startUpSettings(pConfig);




    return 0;
}
