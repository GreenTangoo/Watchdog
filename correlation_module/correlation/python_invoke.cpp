#include "python_invoke.hpp"

using namespace correlation_space;

PythonInvoke::PythonInvoke()
{
    Initialize();
}

PythonInvoke::PythonInvoke(std::wstring scriptName, size_t amountArgs, std::vector<std::wstring> args) :
    m_ScriptName(scriptName), m_AmountArgs(amountArgs), m_Args(args)
{
    Initialize();
}

PythonInvoke::PythonInvoke(const PythonInvoke &other) :
    m_ScriptName(other.m_ScriptName), m_AmountArgs(other.m_AmountArgs), m_Args(other.m_Args)
{
    Initialize();
}

PythonInvoke::~PythonInvoke()
{
    Uninitialize();
}

PythonInvoke const& PythonInvoke::operator=(const PythonInvoke &other)
{
    if(this != &other)
    {
        Uninitialize();

        m_ScriptName = other.m_ScriptName;
        m_AmountArgs = other.m_AmountArgs;
        m_Args = other.m_Args;
    }

    return *this;
}

std::wstring PythonInvoke::GetScriptName() const
{
    return m_ScriptName;
}

void PythonInvoke::SetScriptName(std::wstring scriptName)
{
    m_ScriptName = scriptName;
}

size_t PythonInvoke::GetAmountArgs() const
{
    return m_AmountArgs;
}

void PythonInvoke::SetAmountArgs(size_t amountArgs)
{
    m_AmountArgs = amountArgs;
}

std::vector<std::wstring> PythonInvoke::GetArgs() const
{
    return m_Args;
}

void PythonInvoke::SetArgs(std::vector<std::wstring> args)
{
    m_Args = args;
}

void PythonInvoke::RunScript()
{
    if(!m_IsPrepared)
        Prepare();

    std::string scriptName(m_ScriptName.begin(), m_ScriptName.end());
    FILE *file = nullptr;

    file = fopen(scriptName.c_str(), "r");

    PyRun_SimpleFile(file, scriptName.c_str());

    fclose(file);
}

void PythonInvoke::Initialize()
{
    Py_Initialize();
}

void PythonInvoke::Uninitialize()
{
    if(Py_IsInitialized())
    {
        Py_Finalize();
    }
}

void PythonInvoke::Prepare()
{
    PrepareScriptName();
    PrepareArgs();

    m_IsPrepared = true;
}

void PythonInvoke::PrepareScriptName()
{
    Py_SetProgramName(m_ScriptName.c_str());
}

void PythonInvoke::PrepareArgs()
{
    const int amountArgs = static_cast<int>(m_AmountArgs);
    wchar_t *args[amountArgs];

    for(size_t i{0}; i < m_Args.size(); i++)
    {
        args[i] = const_cast<wchar_t*>(m_Args[i].c_str());
    }

    PySys_SetArgv(amountArgs, args);
}

bool PythonInvoke::IsPreparedArgs() const
{
    return m_IsPrepared;
}
