#include <iostream>
#include <string>
#include <atomic>
#include <csignal>

#include "siem_startup.hpp"
#include "utility_module/json.hpp"
#include "aggregation_module/aggregation_module_initializer.hpp"
#include <Python.h>

using namespace utility_space;
using namespace main_siem_space;
using namespace aggregation_space;

#define MAIN_OPTIONS_PATH "configs/options.json"

static std::atomic<bool> isStopWork(false);

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

    JsonObject startupSettingsJson = getJsonData(MAIN_OPTIONS_PATH);
    SettingsSIEM settings(startupSettingsJson);

    AggregationInitializer aggrInit(settings);

    while(!isStopWork.load())
    {
        aggrInit.startCycle();
    }

	return 0;
}
