#include "grabber_base.hpp"

using namespace aggregation_space;
using namespace utility_space;

// /////////////////////////////////////////////////////////////////////////
// IGrabber
// Semi interface for all types aggregation, using in SIEM.
// ////////////////////////////////////////////////////////////////////////
IGrabber::IGrabber(SubAggregationBaseSettingsPtr const &settings) :
    m_SrcLogFile(settings->GetSrcFile()), m_DestLogFile(settings->GetDstFile()),
    m_DetailsLvl(static_cast<DetailsLevel>(settings->GetDetailsLevel())), m_SerializeType(settings->GetSerializeKind())
{

}

IGrabber::IGrabber(IGrabber const &other) :
    m_SrcLogFile(other.m_SrcLogFile), m_DestLogFile(other.m_DestLogFile),
    m_DetailsLvl(other.m_DetailsLvl), m_SerializeType(other.m_SerializeType)
{

}

IGrabber::IGrabber(IGrabber &&other) :
    m_SrcLogFile(std::move(other.m_SrcLogFile)), m_DestLogFile(std::move(other.m_DestLogFile)),
    m_DetailsLvl(std::move(other.m_DetailsLvl)), m_SerializeType(std::move(other.m_SerializeType))
{

}

IGrabber const& IGrabber::operator=(IGrabber const &other)
{
    if(this != &other)
    {
        m_SrcLogFile = other.m_SrcLogFile;
        m_DestLogFile = other.m_DestLogFile;
        m_DetailsLvl = other.m_DetailsLvl;
        m_SerializeType = other.m_SerializeType;
    }

    return *this;
}

IGrabber const& IGrabber::operator=(IGrabber &&other)
{
    if(this != &other)
    {
        m_SrcLogFile = std::move(other.m_SrcLogFile);
        m_DestLogFile = std::move(other.m_DestLogFile);
        m_DetailsLvl = std::move(other.m_DetailsLvl);
        m_SerializeType = std::move(other.m_SerializeType);
    }

    return *this;
}

// /////////////////////////////////////////////////////////////////////////
// GrabberBase
// Base class for all types aggregation, using in SIEM.
// ////////////////////////////////////////////////////////////////////////
//GrabberBase::GrabberBase(const SubAggregationBaseSettingsPtr &settings) :
//    m_SrcLogFile(settings->GetSrcFile()), m_DestLogFile(settings->GetDstFile(), FileManipulator::READ_WRITE),
//    m_DetailsLvl(static_cast<DetailsLevel>(settings->GetDetailsLevel())), m_SerializeType(settings->GetSerializeKind())
//{

//}
GrabberBase::GrabberBase(SubAggregationBaseSettingsPtr const &settings) :
    IGrabber(settings)
{

}


GrabberBase::GrabberBase(GrabberBase const &other) :
    IGrabber(other)
{

}

GrabberBase::GrabberBase(GrabberBase &&other) :
    IGrabber(std::move(other))
{

}

GrabberBase const& GrabberBase::operator=(GrabberBase const &other)
{
    if(this != &other)
    {
        IGrabber::operator=(other);
    }

    return *this;
}

GrabberBase const& GrabberBase::operator=(GrabberBase &&other)
{
    if(this != &other)
    {
        IGrabber::operator=(std::move(other));
    }

    return *this;
}
