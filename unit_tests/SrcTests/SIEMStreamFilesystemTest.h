#include <cxxtest/TestSuite.h>
#include <stream_siem.hpp>
#include <stream_siem.cpp>

#include <string>

using namespace utility_space;

class SIEMStreamFilesystemTestSuite : public CxxTest::TestSuite
{
public:
    void testRead(void)
    {
      SIEMStreamFilesystem stream(m_StreamData.length() / 4);
      stream.Read(m_StreamData.c_str(), m_StreamData.length());

      TS_ASSERT_EQUALS(stream.GetStreamSize(), m_StreamData.length());
      TS_ASSERT_EQUALS(stream.GetPosR(), m_StreamData.length());
    }

    void testWrite(void)
    {
      SIEMStreamFilesystem stream(m_StreamData.length() / 4);
      stream.Read(m_StreamData.c_str(), m_StreamData.length());
      stream.SetPosR(0);

      TS_ASSERT_EQUALS(stream.GetPosR(), 0);

      std::vector<char> resultBuf(m_StreamData.length());
      stream.Write(resultBuf.data(), m_StreamData.length());

      TS_ASSERT_SAME_DATA(m_StreamData.c_str(), resultBuf.data(), m_StreamData.length());
    }
private:
  const std::string m_StreamData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris luctus.";
};