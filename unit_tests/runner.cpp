/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_DateTimeTest_init = false;
#include "date_time_test.hpp"

static DateTimeTest suite_DateTimeTest;

static CxxTest::List Tests_DateTimeTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_DateTimeTest( "date_time_test.hpp", 13, "DateTimeTest", suite_DateTimeTest, Tests_DateTimeTest );

static class TestDescription_suite_DateTimeTest_testPutGetTime : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_DateTimeTest_testPutGetTime() : CxxTest::RealTestDescription( Tests_DateTimeTest, suiteDescription_DateTimeTest, 16, "testPutGetTime" ) {}
 void runTest() { suite_DateTimeTest.testPutGetTime(); }
} testDescription_suite_DateTimeTest_testPutGetTime;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
