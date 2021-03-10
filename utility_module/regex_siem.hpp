#ifndef REGEX_SIEM_HPP
#define REGEX_SIEM_HPP

#include <regex>
#include <string>
#include <vector>

namespace utility_space
{
    class RegexSiem : public std::regex
    {
    public:
        explicit RegexSiem();
        explicit RegexSiem(std::string expr);
        RegexSiem(RegexSiem const &other);
        RegexSiem(RegexSiem &&other);
        ~RegexSiem();
        RegexSiem const& operator=(std::string const &str);
        RegexSiem const& operator=(RegexSiem const &other);
        RegexSiem const& operator=(RegexSiem &&other);
        std::string getExpressison();
    private:
        std::string expression;
    };

    std::string findByRegex(std::string const &line, std::regex const reg, int regGroup);
    std::string replaceByRegexVec(std::string const &srcStr, std::vector<std::pair<std::regex, std::string>> regDstPair);
    std::string replaceByRegexVec(std::string const &srcStr, std::vector<std::pair<RegexSiem, std::string>> regDstPair);
}

#endif // REGEX_SIEM_HPP