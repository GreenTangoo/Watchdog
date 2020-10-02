/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
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
bool suite_SyntaxAnalyzerTest_init = false;
#include "syntax_analyzer_test.hpp"

static SyntaxAnalyzerTest suite_SyntaxAnalyzerTest;

static CxxTest::List Tests_SyntaxAnalyzerTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SyntaxAnalyzerTest( "syntax_analyzer_test.hpp", 21, "SyntaxAnalyzerTest", suite_SyntaxAnalyzerTest, Tests_SyntaxAnalyzerTest );

static class TestDescription_suite_SyntaxAnalyzerTest_testRelationshipTransforms : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SyntaxAnalyzerTest_testRelationshipTransforms() : CxxTest::RealTestDescription( Tests_SyntaxAnalyzerTest, suiteDescription_SyntaxAnalyzerTest, 24, "testRelationshipTransforms" ) {}
 void runTest() { suite_SyntaxAnalyzerTest.testRelationshipTransforms(); }
} testDescription_suite_SyntaxAnalyzerTest_testRelationshipTransforms;

static class TestDescription_suite_SyntaxAnalyzerTest_testCompareTransforms : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SyntaxAnalyzerTest_testCompareTransforms() : CxxTest::RealTestDescription( Tests_SyntaxAnalyzerTest, suiteDescription_SyntaxAnalyzerTest, 41, "testCompareTransforms" ) {}
 void runTest() { suite_SyntaxAnalyzerTest.testCompareTransforms(); }
} testDescription_suite_SyntaxAnalyzerTest_testCompareTransforms;

static class TestDescription_suite_SyntaxAnalyzerTest_testCompareFound : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SyntaxAnalyzerTest_testCompareFound() : CxxTest::RealTestDescription( Tests_SyntaxAnalyzerTest, suiteDescription_SyntaxAnalyzerTest, 58, "testCompareFound" ) {}
 void runTest() { suite_SyntaxAnalyzerTest.testCompareFound(); }
} testDescription_suite_SyntaxAnalyzerTest_testCompareFound;

static class TestDescription_suite_SyntaxAnalyzerTest_testNextRelationShipFound : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SyntaxAnalyzerTest_testNextRelationShipFound() : CxxTest::RealTestDescription( Tests_SyntaxAnalyzerTest, suiteDescription_SyntaxAnalyzerTest, 76, "testNextRelationShipFound" ) {}
 void runTest() { suite_SyntaxAnalyzerTest.testNextRelationShipFound(); }
} testDescription_suite_SyntaxAnalyzerTest_testNextRelationShipFound;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
