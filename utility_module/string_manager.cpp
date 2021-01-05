#include "string_manager.hpp"

using namespace utility_space;

StringManager::StringManager()
{

}

StringManager::~StringManager()
{

}

std::string StringManager::getStrBetweenSymbols(std::string const &sourceStr, char first, char second)
{
	size_t firstSymbolPos = sourceStr.find(first);
	if(firstSymbolPos == std::string::npos)
	{
		return std::string("");
	}

	size_t secondSymbolPos = sourceStr.find(second, firstSymbolPos + 1);
	if(secondSymbolPos == std::string::npos)
	{
		if(second == '\0')
		{
			secondSymbolPos = sourceStr.size() + 1;
		}
		else
		{
			return std::string("");
		}
	}

	return sourceStr.substr(firstSymbolPos + 1, secondSymbolPos - firstSymbolPos - 1);
}

std::vector<std::string> StringManager::getVecStrBetweenSymbols(std::string const &sourceStr, char first, char second)
{

}

std::vector<std::string> StringManager::parseByDelimiter(std::string const &sourceStr, std::string const &delimiter)
{
	std::vector<std::string> returnVec;
	size_t foundPos = sourceStr.find(delimiter);
	size_t lastPos = 0;
	std::string tempStr;

	while(foundPos != std::string::npos)
	{
		std::copy(sourceStr.begin() + lastPos, sourceStr.begin() + foundPos, 
			std::back_inserter(tempStr));

		if(tempStr.length() > 0)
			returnVec.push_back(tempStr);

		tempStr = "";
		lastPos = foundPos + delimiter.size();
		foundPos = sourceStr.find(delimiter, foundPos + delimiter.size());
	}

    std::copy(sourceStr.begin() + lastPos, sourceStr.end(), std::back_inserter(tempStr));

    if(tempStr.length() > 0)
          returnVec.push_back(tempStr);

    return returnVec;
}

std::string StringManager::deleteSymbol(std::string const &sourceStr, symbolType symbol)
{
	std::string cropedString = sourceStr;
	cropedString.erase(std::remove(cropedString.begin(), cropedString.end(), symbol), 
		cropedString.end());
	return cropedString;
}

std::string StringManager::deleteSymbols(std::string const &sourceStr, std::vector<symbolType> const &symbolsVec)
{
	std::string cropedString = sourceStr;
	for(size_t i(0); i < symbolsVec.size(); i++)
		cropedString.erase(std::remove(cropedString.begin(), cropedString.end(), symbolsVec[i]), 
			cropedString.end());

	return cropedString;
}

std::string StringManager::deleteSymbols(std::string const &sourceStr, std::string const &symbolsString)
{
	std::string cropedString = sourceStr;
	for(size_t i(0); i < symbolsString.size(); i++)
		cropedString.erase(std::remove(cropedString.begin(), cropedString.end(), symbolsString[i]), 
			cropedString.end());

	return cropedString;
}

std::string StringManager::cropFromEnd(std::string const &sourceStr, symbolType delimiterSymbol)
{
	std::string cropedString = sourceStr;
	char ch = 0;

	while((cropedString.size() > 0) && 
				((ch = cropedString.back()) != delimiterSymbol))
	{
		cropedString.pop_back();
	}

	if(cropedString.size() != 0)
	{
		cropedString.pop_back();
		return cropedString;
	}
	else
	{
		return sourceStr;
	}
}

std::string StringManager::cropFromBegin(std::string const &sourceStr, symbolType delimiterSymbol)
{
	std::string cropedString = sourceStr;
	size_t cropIndex = 0;
	size_t stringSize = cropedString.size();

	while((cropIndex < stringSize) && 
				(cropedString[cropIndex] != delimiterSymbol))
	{
		cropIndex++;
	}

	return cropIndex <= stringSize - 1 ? cropedString.substr(cropIndex + 1, stringSize - 1) : 
		sourceStr;
}

std::string StringManager::getBeforeSymbol(std::string const &sourceStr, symbolType delimiterSymbol)
{
	size_t symbolPos = sourceStr.find(delimiterSymbol);
	if(symbolPos == std::string::npos)
	{
		return std::string("");
	}

	return sourceStr.substr(0, symbolPos);
}

std::string StringManager::getAfterSymbol(std::string const &sourceStr, symbolType delimiterSymbol)
{
	size_t symbolPos = sourceStr.find(delimiterSymbol);
	if(symbolPos == std::string::npos)
	{
		return std::string("");
	}

	return sourceStr.substr(symbolPos + 1, sourceStr.size() - symbolPos - 1);
}

std::string StringManager::constructPath(symbolType delimPath, std::vector<std::string> const &args)
{
	std::string pathStr = args[0];
	for(size_t i(0); i < args.size() - 1; i++)
	{
		pathStr += delimPath;
		pathStr += args[i + 1];
	}

	return pathStr;
}

std::string StringManager::getSubstring(std::string const &sourceStr, std::string const &subStr)
{
	std::string resultStr;

	size_t foundedPos = sourceStr.find(subStr);
	if(foundedPos != std::string::npos)
	{
		resultStr = sourceStr.substr(foundedPos, subStr.size());
	}

	return resultStr;
}

bool StringManager::isSubstrIn(std::string const &sourceStr, std::string const &subStr)
{
	size_t pos = sourceStr.find(subStr);

	return pos != std::string::npos ? true : false;
}