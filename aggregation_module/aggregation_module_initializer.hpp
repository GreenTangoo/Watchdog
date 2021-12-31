#ifndef AGGREGATION_MODULE_INITIALIZER_HPP
#define AGGREGATION_MODULE_INITIALIZER_HPP

#include <map>
#include <vector>

#include "../siem_startup.hpp"
#include "../utility_module/thread_pool.hpp"
#include "../description_module/description_table.hpp"

using namespace main_siem_space;
using namespace description_space;
using namespace utility_space;

namespace aggregation_space
{

    class AggregationInitializer
    {
    public:
        explicit AggregationInitializer(SettingsSIEM const &settings);
        AggregationInitializer(AggregationInitializer const &other) = delete;
        AggregationInitializer(AggregationInitializer &&other);
        ~AggregationInitializer();
        AggregationInitializer const& operator=(AggregationInitializer const &other) = delete;
        AggregationInitializer const& operator=(AggregationInitializer &&other);
        void startCycle();
    private:
        void initCustomGrabbers(SettingsSIEM const &settings);
        void initDefaultGrabbers(SettingsSIEM const &settings);
    private:
        size_t _amountThreads;
        //std::vector<SymptomGrabber> _grabbers;
    };

}

#endif // AGGREGATION_MODULE_INITIALIZER_HPP
