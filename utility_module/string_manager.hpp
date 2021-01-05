#ifndef STRING_MANAGER_HPP
#define STRING_MANAGER_HPP

#include <vector>
#include <string>
#include <algorithm>

namespace utility_space
{

	enum symbolType { SPACE = ' ', QUOTE = '\'', DQUOTE = '\"', SLASH = '/',
					  RSLASH = '\\', NEW_LINE = '\n', TAB = '\t', COMMA = ',',
					  L_SQ_BRACKET = '[', R_SQ_BRACKET = ']', EQUAL = '=' };

	class StringManager
	{
	private:
		StringManager();
	public:
		~StringManager();
		StringManager(StringManager const &other) = delete;
		StringManager(StringManager &&other) = delete;
		StringManager const& operator=(StringManager const &other) = delete;
		StringManager const& operator=(StringManager &&other) = delete;
		static std::string getStrBetweenSymbols(std::string const &sourceStr, char first, char second);
		static std::vector<std::string> getVecStrBetweenSymbols(std::string const &sourceStr, char first, char second);
		static std::vector<std::string> parseByDelimiter(std::string const &sourceStr, std::string const &delimiter);
		static std::string deleteSymbol(std::string const &sourceStr, symbolType symbol);
		static std::string deleteSymbols(std::string const &sourceStr, std::vector<symbolType> const &symbolsVec);
		static std::string deleteSymbols(std::string const &sourceStr, std::string const &symbolsString);
		static std::string cropFromEnd(std::string const &sourceStr, symbolType delimiterSymbol);
		static std::string cropFromBegin(std::string const &sourceStr, symbolType delimiterSymbol);
		static std::string getBeforeSymbol(std::string const &sourceStr, symbolType delimiterSymbol);
		static std::string getAfterSymbol(std::string const &sourceStr, symbolType delimiterSymbol);
		static std::string constructPath(symbolType pathDelim, std::vector<std::string> const &args);
		static std::string getSubstring(std::string const &sourceStr, std::string const &subStr);
		static bool isSubstrIn(std::string const &sourceStr, std::string const &subStr);
	};

}

#endif // STRING_MANAGER_HPP