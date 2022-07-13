#ifndef PYTHON_INVOKE_HPP
#define PYTHON_INVOKE_HPP

#include <string>
#include <vector>

#include <Python.h>

namespace correlation_space
{
    class PythonInvoke
    {
    public:
        PythonInvoke();
        PythonInvoke(std::wstring scriptName, size_t amountArgs, std::vector<std::wstring> args);
        PythonInvoke(const PythonInvoke &other);
        PythonInvoke(PythonInvoke &&other) = delete;
        ~PythonInvoke();
        PythonInvoke const& operator=(const PythonInvoke &other);
        PythonInvoke const& operator=(PythonInvoke &&other) = delete;
        std::wstring GetScriptName() const;
        void SetScriptName(std::wstring scriptName);
        size_t GetAmountArgs() const;
        void SetAmountArgs(size_t amountArgs);
        std::vector<std::wstring> GetArgs() const;
        void SetArgs(std::vector<std::wstring> args);
        void RunScript();
    private:
        void Initialize();
        void Uninitialize();
        void Prepare();
        void PrepareScriptName();
        void PrepareArgs();
        bool IsPreparedArgs() const;
    private:
        std::wstring m_ScriptName;
        size_t m_AmountArgs;
        std::vector<std::wstring> m_Args;
        bool m_IsPrepared = false;
    };
}

#endif // PYTHON_INVOKE_HPP
