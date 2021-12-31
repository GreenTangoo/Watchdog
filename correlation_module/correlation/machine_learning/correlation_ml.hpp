#ifndef CORRELATION_ML_HPP
#define CORRELATION_ML_HPP

#include "../correlation.hpp"

namespace correlation_space
{
    class CorrelationMachineLearning : public ICorrelation
    {
    public:
        enum MachineLearningType {noneMl = 0, kNeighboorMl, logisticRegressionMl, decisionTreeMl, randomForestMl};
        CorrelationMachineLearning() = default;
        CorrelationMachineLearning(CorrelationMachineLearning const &other) = default;
        CorrelationMachineLearning(CorrelationMachineLearning &&other) = default;
        virtual ~CorrelationMachineLearning() = default;
        virtual void startCorrelation() = 0;
    };

    class CorrelationKNeighboor : public CorrelationMachineLearning
    {
    public:
        CorrelationKNeighboor() = default;
        CorrelationKNeighboor(CorrelationKNeighboor const &other) = default;
        CorrelationKNeighboor(CorrelationKNeighboor &&other) = default;
        virtual ~CorrelationKNeighboor() = default;
        virtual void startCorrelation();

    };
}

#endif // CORRELATION_ML_HPP
