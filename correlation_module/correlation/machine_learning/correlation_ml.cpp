#include "correlation_ml.hpp"

#include <map>

using namespace correlation_space;

static std::map<CorrelationMachineLearning::MachineLearningType, std::wstring> mlTypeStringMap = {
    {CorrelationMachineLearning::noneMl, L""},
    {CorrelationMachineLearning::kNeighboorMl, L"kNeighbors"},
    {CorrelationMachineLearning::logisticRegressionMl, L"LogisticRegression"},
    {CorrelationMachineLearning::decisionTreeMl, L"DecisionTree"},
    {CorrelationMachineLearning::randomForestMl, L"RandomForest"},
    {CorrelationMachineLearning::neuralNetMl, L"NeuralNet"}
};

/*---------------------------------------------------------------*/
/*---------------CORRELATION_MACHINE_LEARNING--------------------*/
/*---------------------------------------------------------------*/
CorrelationMachineLearning::CorrelationMachineLearning(size_t amountArgs, std::wstring scriptName) :
    m_AmountArgs(amountArgs), m_ScriptName(scriptName)
{

}

CorrelationMachineLearning::CorrelationMachineLearning(CorrelationMachineLearning const &other):
    m_AmountArgs(other.m_AmountArgs),
    m_ScriptName(other.m_ScriptName), m_Invoker(other.m_Invoker)
{

}

CorrelationMachineLearning const& CorrelationMachineLearning::operator=(CorrelationMachineLearning const &other)
{
    if(this != &other)
    {
        m_AmountArgs = other.m_AmountArgs;
        m_ScriptName = other.m_ScriptName;
        m_Invoker = other.m_Invoker;
    }

    return *this;
}

PythonInvoke& CorrelationMachineLearning::GetInvoker()
{
    return m_Invoker;
}

std::wstring CorrelationMachineLearning::GetScriptName() const
{
    return m_ScriptName;
}

void CorrelationMachineLearning::SetScriptName(std::wstring scriptName)
{
    m_ScriptName = scriptName;
}

size_t CorrelationMachineLearning::GetAmountArgs() const
{
    return m_AmountArgs;
}

void CorrelationMachineLearning::SetAmountArgs(size_t amountArgs)
{
    m_AmountArgs = amountArgs;
}

void CorrelationMachineLearning::SetInvokerParameters(std::vector<std::wstring> params)
{
    m_Invoker.SetScriptName(m_ScriptName);
    m_Invoker.SetAmountArgs(m_AmountArgs);
    m_Invoker.SetArgs(params);
}

std::wstring CorrelationMachineLearning::MlType2Wstring(MachineLearningType mlType)
{
    return mlTypeStringMap[mlType];
}

/*---------------------------------------------------------------*/
/*---------------CORRELATION_K_NEIGHBOORS------------------------*/
/*---------------------------------------------------------------*/
CorrelationKNeighboor::CorrelationKNeighboor(size_t amountArgs, std::wstring scriptName) :
    CorrelationMachineLearning(amountArgs, scriptName)
{

}

CorrelationKNeighboor::CorrelationKNeighboor(CorrelationKNeighboor const &other) :
    CorrelationMachineLearning(other)
{

}

CorrelationKNeighboor const& CorrelationKNeighboor::operator=(CorrelationKNeighboor const &other)
{
    if(this != &other)
    {
        CorrelationMachineLearning::operator=(other);
    }

    return *this;
}

void CorrelationKNeighboor::startCorrelation()
{
    auto &invoker = GetInvoker();

    std::vector<std::wstring> paramsVec = PrepareScriptParams();
    SetInvokerParameters(paramsVec);

    invoker.RunScript();
}
std::vector<std::wstring> CorrelationKNeighboor::PrepareScriptParams()
{
    std::vector<std::wstring> args;
    args.push_back(GetScriptName());
    args.push_back(MlType2Wstring(kNeighboorMl));

    return args;
}
