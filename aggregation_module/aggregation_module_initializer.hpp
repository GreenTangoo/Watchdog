#ifndef AGGREGATION_MODULE_INITIALIZER_HPP
#define AGGREGATION_MODULE_INITIALIZER_HPP

#include <map>
#include <vector>

#include "grabbers.hpp"

#include "../siem_startup.hpp"
#include "../utility_module/thread_pool.hpp"

using namespace main_siem_space;
using namespace utility_space;

namespace aggregation_space
{

    class GrabbersListCreator
    {
    public:
        GrabbersListCreator() = default;
        GrabbersListCreator(GrabbersListCreator const &other);
        GrabbersListCreator(GrabbersListCreator &&other);
        ~GrabbersListCreator();
        std::vector<AggrGrabber> GetAggrsArr();
    };

    class GrabberFactory
    {
    public:
        GrabberFactory(GrabberFactory const &other) = delete;
        GrabberFactory(GrabberFactory &&other) = delete;
        GrabberFactory const& operator=(GrabberFactory const &other) = delete;
        GrabberFactory const& operator=(GrabberFactory &&other) = delete;
        static GrabberFactory& GetInstance();
    private:
        GrabberFactory();
    };

    class AggregationInitializer
    {
    public:
        explicit AggregationInitializer(SettingsSIEM const &settings);
        AggregationInitializer(AggregationInitializer const &other) = delete;
        AggregationInitializer(AggregationInitializer &&other) = delete;
        ~AggregationInitializer();
        AggregationInitializer const& operator=(AggregationInitializer const &other) = delete;
        AggregationInitializer const& operator=(AggregationInitializer &&other) = delete;
        void startCycle();
    private:
        void initDefaultGrabbers(SettingsSIEM const &settings);
    private:
        size_t _amountThreads;
        std::vector<IGrabberPtr> _grabbers;
    };

}

#endif // AGGREGATION_MODULE_INITIALIZER_HPP
