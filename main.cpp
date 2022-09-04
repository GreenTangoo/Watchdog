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

IJsonContainerPtr createJson()
{
    std::map<std::string, IJsonContainerPtr> mainContainer;
    for(int i(0); i < 5; i++)
    {
        const std::string value = std::string("val-") + std::to_string(i);
        IJsonContainerPtr temp = utility_space::CreateContainer(value);

        mainContainer.insert({std::to_string(i), temp});
    }

    std::map<std::string, IJsonContainerPtr> externContainer;
    externContainer.insert({"main", utility_space::CreateContainer(mainContainer)});

    return CreateContainer(externContainer);
}

int main(int argc, char** argv)
{
//    signal(SIGINT, interrupt_handler);

//    JsonObject startupSettingsJson = getJsonData(MAIN_OPTIONS_PATH);
//    SettingsSIEM settings(startupSettingsJson);

//    AggregationInitializer aggrInit(settings);

//    while(!isStopWork.load())
//    {
//        aggrInit.startCycle();
//    }

    JsonFileSerializer *serializer = new JsonFileSerializer(createJson());

    serializer->SetFile("test.json");
    serializer->Write();

    delete serializer;

    JsonFileDeserializer *deserializer = new JsonFileDeserializer;

    deserializer->SetFile("test.json");
    IJsonContainerPtr allJson = deserializer->Read();

    delete deserializer;

    serializer = new JsonFileSerializer(allJson);

    serializer->SetFile("test1.json");
    serializer->Write();

    delete serializer;

    return 0;
}
