#include "aggregation_module_initializer.hpp"

using namespace aggregation_space;

// //////////////////////////////////////////////////////////////
// GrabberFactory
// Class-factory for creating grabbers.
// //////////////////////////////////////////////////////////////
GrabberFactory& GrabberFactory::GetInstance()
{
    static GrabberFactory factory;
    return factory;
}

IGrabberPtr GrabberFactory::Create(AggrGrabber const aggrType, AggregationSettings const &settings)
{
    switch(aggrType)
    {
    case AggrGrabber::aggrIPtables:

        return std::make_shared<AggregationIpTables>(
                    std::dynamic_pointer_cast<AggregationIpTablesSettings>(
                        settings.GetSettingsByType(AggrGrabber::aggrIPtables)));

    case AggrGrabber::aggrVsftpd:

        return std::make_shared<AggregationVsftpd>(
                    std::dynamic_pointer_cast<AggregationVsftpdSettings>(
                        settings.GetSettingsByType(AggrGrabber::aggrVsftpd)));

    case AggrGrabber::aggrSshd:

        return std::make_shared<AggregationSshd>(
                    std::dynamic_pointer_cast<AggregationSshdSettings>(
                        settings.GetSettingsByType(AggrGrabber::aggrSshd)));

    case AggrGrabber::aggrApache:

        return std::make_shared<AggregationApache>(
                    std::dynamic_pointer_cast<AggregationApacheSettings>(
                        settings.GetSettingsByType(AggrGrabber::aggrApache)));

    case AggrGrabber::aggrPam:

        return std::make_shared<AggregationPam>(
                    std::dynamic_pointer_cast<AggregationPamSettings>(
                        settings.GetSettingsByType(AggrGrabber::aggrPam)));

    default:
        throw AggregationException("Cannot create aggregator by aggregation type",
                                   AggregationException::INVALID_GRABBER_TYPE);
    }
}


// ///////////////////////////////////////////////////////////////
// AggregationInitializer
// Initialization SIEM aggregation cycle class.
// ///////////////////////////////////////////////////////////////
AggregationInitializer::AggregationInitializer(SettingsSIEM const &settings) :
    m_AmountThreads(settings.GetAmountAggregationThreads())
{
    m_GrabbersSet.resize(m_AmountThreads);
    InitDefaultGrabbers(settings);
}

void AggregationInitializer::StartCycle()
{
    std::vector<std::thread> threads;
    threads.reserve(m_AmountThreads);

    for(GrabberVec &grabbers : m_GrabbersSet)
    {
        std::thread th([&grabbers]()
        {
            for(IGrabberPtr grabber : grabbers)
            {
                if(grabber->StartAggregate())
                {
                    const auto& pVisitor = CreateVisitor(grabber->GetSerializeType());
                    grabber->Accept(pVisitor);
                }
            }
        });

        threads.push_back(std::move(th));
    }

    for(std::thread &thread : threads)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
}

void AggregationInitializer::InitDefaultGrabbers(SettingsSIEM const &settings)
{
    AggregationSettings aggrSettings = settings.GetAggregationSettings();
    std::vector<AggrGrabber> typesVec = aggrSettings.GetGrabbersTypes();

    GrabberFactory &factory = GrabberFactory::GetInstance();
    size_t vectorNum = 0;

    // Create information grabbers and immediately
    // distribute them between threads.
    for(const auto grabberType : typesVec)
    {
        try
        {
            IGrabberPtr pGrabber = factory.Create(grabberType, aggrSettings);
            m_GrabbersSet[vectorNum].push_back(pGrabber);

            vectorNum++;
            if(vectorNum >= m_AmountThreads)
            {
                vectorNum = 0;
            }
        }
        catch(AggregationException const &ex)
        {
            SiemLogger &logger = SiemLogger::GetInstance();

            logger.WriteLog("AggregationInitializer",
                            ex.what() + std::to_string(ex.getErrorCode()) + "| grabber type: " + AggrGrabber2String(grabberType),
                            0);
        }
    }
}
