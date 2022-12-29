#include <cxxtest/TestSuite.h>
#include <stream_siem.hpp>
#include <stream_siem.cpp>

#include <string>

using namespace utility_space;

class SIEMStreamMemoryTestSuite : public CxxTest::TestSuite
{
public:
  void testRead(void)
  {
    SIEMStreamMemory stream;
    stream.Read(m_StreamData.c_str(), m_StreamData.length());

    TS_ASSERT_EQUALS(stream.GetStreamSize(), m_StreamData.length());
    TS_ASSERT_EQUALS(stream.GetPos(), m_StreamData.length());
  }
private:
  const std::string m_StreamData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris luctus.";
};
