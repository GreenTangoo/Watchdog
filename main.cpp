#include <iostream>
#include <string>
#include <atomic>
#include <csignal>

#include "siem_startup.hpp"
#include "siem_defines.hpp"
#include "utility_module/json_proc.hpp"
#include "utility_module/stream_siem.hpp"
#include "aggregation_module/aggregation_module_initializer.hpp"

using namespace utility_space;
using namespace main_siem_space;
using namespace aggregation_space;



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
//    signal(SIGINT, interrupt_handler);

//    JsonFileDeserializer reader(g_SIEMConfigPath);

//    IJsonContainerPtr pConfig = reader.Read();
//    SettingsSIEM startUpSettings(pConfig);

//    AggregationInitializer aggInit(startUpSettings);

//    aggInit.StartCycle();

    const std::string m_StreamData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris luctus.";
    SIEMStreamMemory stream;

    for(size_t i{0}; i < m_StreamData.length(); i++)
      stream.Get(m_StreamData[i]);

    stream.SetPosW(0);
    std::vector<char> resultBuf(m_StreamData.length());
    stream.Write(resultBuf.data(), m_StreamData.length());
    return 0;
}
