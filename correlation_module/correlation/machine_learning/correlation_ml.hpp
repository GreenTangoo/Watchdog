#ifndef CORRELATION_ML_HPP
#define CORRELATION_ML_HPP

#include <functional>


#include "../correlation.hpp"
#include "../python_invoke.hpp"
#include "../utility_module/json.hpp"


namespace correlation_space
{
    class CorrelationMachineLearning : public ICorrelation
    {
    public:
        enum MachineLearningType {noneMl = 0, kNeighboorMl, logisticRegressionMl, decisionTreeMl, randomForestMl, neuralNetMl};

        CorrelationMachineLearning() = default;
        CorrelationMachineLearning(size_t amountArgs, std::wstring scriptName);
        CorrelationMachineLearning(CorrelationMachineLearning const &other);
        CorrelationMachineLearning(CorrelationMachineLearning &&other) = delete;
        virtual ~CorrelationMachineLearning() = default;

        CorrelationMachineLearning const& operator=(CorrelationMachineLearning const &other);
        CorrelationMachineLearning const& operator=(CorrelationMachineLearning &&other) = delete;

        virtual void startCorrelation() = 0;
        PythonInvoke& GetInvoker();

        std::wstring GetScriptName() const;
        void SetScriptName(std::wstring scriptName);

        size_t GetAmountArgs() const;
        void SetAmountArgs(size_t amountArgs);
    protected:
        virtual std::vector<std::wstring> PrepareScriptParams() = 0;
        void SetInvokerParameters(std::vector<std::wstring> params);
        std::wstring MlType2Wstring(MachineLearningType mlType);
    private:
        size_t m_AmountArgs = 0;
        std::wstring m_ScriptName = L"";
        PythonInvoke m_Invoker;
    };

    class CorrelationKNeighboor : public CorrelationMachineLearning
    {
    public:
        CorrelationKNeighboor() = default;
        CorrelationKNeighboor(size_t amountArgs, std::wstring scriptName);
        CorrelationKNeighboor(CorrelationKNeighboor const &other);
        CorrelationKNeighboor(CorrelationKNeighboor &&other) = delete;
        virtual ~CorrelationKNeighboor() override = default;

        CorrelationKNeighboor const& operator=(CorrelationKNeighboor const &other);
        CorrelationKNeighboor const& operator=(CorrelationKNeighboor &&other) = delete;

        virtual void startCorrelation() override;
    protected:
        virtual std::vector<std::wstring> PrepareScriptParams() override;
    };
}

#endif // CORRELATION_ML_HPP
