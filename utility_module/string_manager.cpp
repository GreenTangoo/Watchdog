#include "string_manager.hpp"

using namespace utility_space;

StringManager::StringManager()
{

}

StringManager::~StringManager()
{

}

std::string StringManager::getSubStr(std::string const &sourceStr, std::string const &subStr)
{
	size_t subStrStartIndex = sourceStr.find(subStr);
	if(subStrStartIndex == std::string::npos)
	{
		return std::string("");
	}
	
	return sourceStr.substr(subStrStartIndex, subStr.size());
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