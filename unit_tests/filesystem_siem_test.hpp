#ifndef DATE_TIME_TEST_HPP
#define DATE_TIME_TEST_HPP

#include <string>
#include <thread>
#include <cstdlib>
#include <cstring>

#include <cxxtest/TestSuite.h>

#include "../utility_module/filesystem_siem.hpp"
#include "../utility_module/filesystem_siem.cpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define TEST_STR "Hello world!"

void write_data(FileManipulator &obj, std::string str, int strIndex)
{
    int sleepTime = rand() % 500;
    for(int i(0); i < 5; i++)
    {
        obj->writeLine(str + std::to_string(strIndex) + "\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

class DateTimeTest : public CxxTest::TestSuite
{
public:
    void testSyncWriteLine(void)
    {
        //Using same length strings for check synchronization access to filestream
        FileManipulator first("file.txt", 
            FileManipulator::CREATE | FileManipulator::WRITEONLY | FileManipulator::TRUNCATE);

        FileManipulator second("file.txt", 
            FileManipulator::WRITEONLY | FileManipulator::APPEND);

        std::thread th1(write_data, std::ref(first), "SampleString", 1);
        std::thread th2(write_data, std::ref(second), "SampleString", 2);

        th1.join();
        th2.join();

        second->synchronizationStream();
        first.closeManipulator();
        second.closeManipulator();

        int standartStringsLen = strlen(TEST_STR) + 1;

        FileManipulator third("file.txt", FileManipulator::READONLY);

        std::string readStr;
        int iteration = 0;
        while(third->readLine(readStr) == std::ifstream::goodbit)
        {
            TS_ASSERT_EQUALS(readStr.length(), standartStringsLen)
        }
    }
    void testAddrFileObject(void)
    {
        // Check that both FileManipulator have same FileObject instance
        FileManipulator first("file.txt", FileManipulator::READONLY);
        FileManipulator second("file.txt", FileManipulator::READONLY);

        FileObject *firstAddr = first.operator->().get();
        FileObject *secondAddr = second.operator->().get();

        TS_ASSERT_EQUALS(firstAddr, secondAddr);
    }
};

#endif // DATE_TIME_TEST_HPP