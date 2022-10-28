#ifndef AGGREGATION_MODULE_INITIALIZER_HPP
#define AGGREGATION_MODULE_INITIALIZER_HPP

#include <map>
#include <vector>

#include "net_grabbers.hpp"

#include "../utility_module/thread_pool.hpp"

using namespace main_siem_space;
using namespace utility_space;

namespace aggregation_space
{
    class GrabberFactory
    {
    public:
        GrabberFactory(GrabberFactory const &other) = delete;
        GrabberFactory(GrabberFactory &&other) = delete;
        GrabberFactory const& operator=(GrabberFactory const &other) = delete;
        GrabberFactory const& operator=(GrabberFactory &&other) = delete;
        static GrabberFactory& GetInstance();
        IGrabberPtr Create(AggrGrabber const aggrType, AggregationSettings const &settings);
    private:
        GrabberFactory() = default;
    };

    class AggregationInitializer
    {
    public:
        explicit AggregationInitializer(SettingsSIEM const &settings);
        AggregationInitializer(AggregationInitializer const &other) = delete;
        AggregationInitializer(AggregationInitializer &&other) = delete;
        ~AggregationInitializer() = default;
        AggregationInitializer const& operator=(AggregationInitializer const &other) = delete;
        AggregationInitializer const& operator=(AggregationInitializer &&other) = delete;
        void StartCycle();
    private:
        typedef std::vector<IGrabberPtr> GrabberVec;
    private:
        void InitDefaultGrabbers(SettingsSIEM const &settings);
        void SeparateByThreads();
    private:
        size_t m_AmountThreads;
        std::vector<GrabberVec> m_GrabbersSet;
    };

}

#endif // AGGREGATION_MODULE_INITIALIZER_HPP
