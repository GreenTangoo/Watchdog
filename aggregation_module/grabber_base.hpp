#ifndef GRABBER_BASE_HPP
#define GRABBER_BASE_HPP

#include <string>

#include "../utility_module/filesystem_siem.hpp"
#include "../siem_startup.hpp"

using namespace main_siem_space;

namespace aggregation_space
{
    class IAggrSerializerVisitor;
    typedef std::shared_ptr<IAggrSerializerVisitor> IAggrSerializerVisitorPtr;

    class IGrabber
    {
    public:
        enum class DetailsLevel : size_t
        {
            LOW = 0,
            MEDIUM = 1,
            HIGH = 2
        };
    public:
        virtual bool StartAggregate() = 0;
        virtual void Accept(IAggrSerializerVisitorPtr const &pVisitor) = 0;
    public:
        IGrabber(SubAggregationBaseSettingsPtr const &settings);
        IGrabber(IGrabber const &other);
        IGrabber(IGrabber &&other);
        virtual ~IGrabber() = default;
        IGrabber const& operator=(IGrabber const &other);
        IGrabber const& operator=(IGrabber &&other);
    public:
        inline std::string GetSrcFile() const { return m_SrcLogFile; }
        inline std::string GetDstFile() const { return m_DestLogFile; }
        inline DetailsLevel GetAggregationDetailsLevel() const { return m_DetailsLvl; }
        inline SerializeType GetSerializeType() const { return m_SerializeType; }
    protected:
        std::string m_SrcLogFile;
        std::string m_DestLogFile;
        DetailsLevel m_DetailsLvl;
        SerializeType m_SerializeType;
    };

    class GrabberBase : public IGrabber
    {
    public:
        explicit GrabberBase(SubAggregationBaseSettingsPtr const &settings);
        explicit GrabberBase(GrabberBase const &other);
        explicit GrabberBase(GrabberBase &&other);
        ~GrabberBase() = default;
        GrabberBase const& operator=(GrabberBase const &other);
        GrabberBase const& operator=(GrabberBase &&other);
    public:
        virtual bool StartAggregate() = 0;
        virtual void Accept(IAggrSerializerVisitorPtr const &pVisitor) = 0;
    };

    typedef std::shared_ptr<IGrabber> IGrabberPtr;
}

#endif // GRABBER_BASE_HPP
