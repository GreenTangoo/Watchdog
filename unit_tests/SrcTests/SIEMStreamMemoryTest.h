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
    SIEMStreamMemory stream(m_StreamData.length() / 4);
    stream.Read(m_StreamData.c_str(), m_StreamData.length());

    TS_ASSERT_EQUALS(stream.GetStreamSize(), m_StreamData.length());
    TS_ASSERT_EQUALS(stream.GetPosR(), m_StreamData.length());
  }

  void testWrite(void)
  {
    SIEMStreamMemory stream(m_StreamData.length() / 4);
    stream.Read(m_StreamData.c_str(), m_StreamData.length());
    stream.SetPosR(0);

    TS_ASSERT_EQUALS(stream.GetPosR(), 0);

    std::vector<char> resultBuf(m_StreamData.length());
    stream.Write(resultBuf.data(), m_StreamData.length());

    TS_ASSERT_SAME_DATA(m_StreamData.c_str(), resultBuf.data(), m_StreamData.length());
  }

  void testGet(void)
  {
    SIEMStreamMemory stream;
    
    for(size_t i{0}; i < m_StreamData.length(); i++)
      stream.Get(m_StreamData[i]);

    stream.SetPosR(0);
    std::vector<char> resultBuf(m_StreamData.length());
    stream.Write(resultBuf.data(), m_StreamData.length());

    TS_ASSERT_SAME_DATA(m_StreamData.c_str(), resultBuf.data(), m_StreamData.length());
  }

  void testPut(void)
  {
    SIEMStreamMemory stream;
    stream.Read(m_StreamData.c_str(), m_StreamData.length());
    stream.SetPosR(0);

    for(size_t i{0}; i < m_StreamData.length(); i++)
    {
      char ch = 0;
      stream.Put(ch);

      TS_ASSERT_EQUALS(ch, m_StreamData[i]);
    }
  } 
private:
  const std::string m_StreamData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris luctus.";
};
