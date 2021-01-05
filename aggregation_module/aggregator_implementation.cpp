#include "aggregator_implementation.hpp"

using namespace utility_space;
using namespace aggregation_space;
using namespace siem_ex_space;

#define SOURCE_LOG_PATH "source_logs"
#define ID_STR "id"

typedef std::vector<std::unique_ptr<AggregationInfoNode>> AggregationNodes;

static std::map<grabberCategory, std::string> categoryStringGrabMap = 
    {
        {IPTABLES, "iptables"}, {APACHE, "apache"}
    };

static details::AggregationInfoNodeBuffer resolveMainInfo(std::string const &oneLogStr, 
    std::vector<std::unique_ptr<AggregationInfoNode>> const &infoAggrVec);

static std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> resolveFormatNodes(
    std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> bufCfg);

static std::string resolveById(std::vector<details::AggregationInfoNodeBuffer> const &bufCfg, std::string const formatStr);

static std::vector<int> getNodesId(std::string formatStr);
static std::vector<std::pair<RegexSiem, std::string>> getRegDstPairById(std::vector<details::AggregationInfoNodeBuffer> const &cfgVec,
    std::vector<int> const &idVec);

static std::string replaceByIdNodes(std::string const &src, std::vector<std::pair<int, std::string>> const &keyNames);

/*-----------------------------------------------------------------*/
/*----------------------SYMPTOM GRABBER----------------------------*/
/*-----------------------------------------------------------------*/
SymptomGrabber::SymptomGrabber(AggregationInfo const &info, grabberCategory grabType) :
    _grabType(grabType), _info(info)
{

}

SymptomGrabber::SymptomGrabber(SymptomGrabber const &other) :
    _grabType(other._grabType), _info(other._info)
{

}

SymptomGrabber::SymptomGrabber(SymptomGrabber &&other) :
    _grabType(std::move(other._grabType)), _info(std::move(other._info))
{

}

SymptomGrabber::~SymptomGrabber()
{

}

void SymptomGrabber::tryAggregationInfo()
{
    _parser = getJsonData(_info.jsonFilename);
    FileManipulator logFile(_info.logFilename, FileManipulator::READONLY | FileManipulator::LARGE_FILE);

    std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> bufferAggrCfg = getFilledCfgBuf(logFile);

    for(size_t i(0); i < bufferAggrCfg->size(); i++)
    {
        putAggrInfoToJson(bufferAggrCfg->at(i));
    }

    setJsonData(logFile->getStream(), _parser); // Not multithread safety
}

/*-----------------------------------------------------------------*/
/*-----------------------PRIVATE-----------------------------------*/
/*-----------------------------------------------------------------*/
std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> SymptomGrabber::getFilledCfgBuf(FileManipulator &fileManip)
{
    std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> bufCfg = 
        std::make_unique<std::vector<details::AggregationInfoNodeBuffer>>();

    std::cout << bufCfg->size() << std::endl;

    std::string fileLine;
    while(fileManip->readLine(fileLine) == std::ios_base::goodbit)
    {
        details::AggregationInfoNodeBuffer resolveAggrInfo = resolveMainInfo(fileLine, _info.aggregationsInfoCfg);
 
        if(resolveAggrInfo.keyName.size() != 0)
        {
            bufCfg->push_back(resolveAggrInfo);
        }
    }

    bufCfg = resolveFormatNodes(std::move(bufCfg));

    return bufCfg;
}

void SymptomGrabber::putAggrInfoToJson(details::AggregationInfoNodeBuffer const &oneAggrInfo)
{
    
}

/*-----------------------------------------------------------------*/
/*-----------------AGREGATION EXCEPTION----------------------------*/
/*-----------------------------------------------------------------*/
SymptomGrabber::AggregationException::AggregationException(std::string const &exMsg, int errCode, int grabType) :
    SIEMExecption(exMsg, errCode), _grabCategory(grabType)
{

}

SymptomGrabber::AggregationException::AggregationException(std::string &&exMsg, int errCode, int grabType) :
    SIEMExecption(std::move(exMsg), errCode), _grabCategory(grabType)
{

}

SymptomGrabber::AggregationException::~AggregationException()
{

}

int SymptomGrabber::AggregationException::getGrabberType() const noexcept
{
    return _grabCategory;
}

/*-----------------------------------------------------------------*/
/*----------------------GRABBER GATEGORY RESOLVER------------------*/
/*-----------------------------------------------------------------*/
grabberCategory GrabberCategoryResolver::stringToGrabberCategory(std::string const &grabName)
{
    for(std::map<grabberCategory, std::string>::const_iterator it = categoryStringGrabMap.begin();
        it != categoryStringGrabMap.end(); it++)
    {
        if(it->second == grabName)
        {
            return it->first;
        }
    }

    throw SymptomGrabber::AggregationException("Invalid aggregation string parameter",
        SymptomGrabber::AggregationException::INCORRECT_AGGR_STRING,
        static_cast<int>(grabberCategory::GRAB_NONE_TYPE));
}

std::string GrabberCategoryResolver::grabberCategoryToString(grabberCategory grabCategory)
{
	auto it = categoryStringGrabMap.find(grabCategory);
    if(it != categoryStringGrabMap.end())
    {
        return it->second;
    }

    throw SymptomGrabber::AggregationException("Invalid aggregation category parameter",
        SymptomGrabber::AggregationException::INCORRECT_AGGR_CATEGORY,
        static_cast<int>(grabberCategory::GRAB_NONE_TYPE));
}

/*----------------------------------------------------------------------------*/
/*-------------------------STATIC FUNCTIONS-----------------------------------*/
/*----------------------------------------------------------------------------*/
details::AggregationInfoNodeBuffer resolveMainInfo(std::string const &oneLogStr, 
    std::vector<std::unique_ptr<AggregationInfoNode>> const &infoAggrVec)
{
    details::AggregationInfoNodeBuffer resolveAggrInfo;
    
    std::string keyName;
    std::string valueName;

    for(const auto &aggrCfgPtr : infoAggrVec)
    {
        int groupRegKey = aggrCfgPtr->regexInfo.keyRegGroup;
        int groupRegValue = aggrCfgPtr->regexInfo.valueRegGroup;

        if(groupRegKey == EMPTY_REGEX_GROUP) continue;

        RegexSiem regFormula = aggrCfgPtr->regexInfo.keyFindRegex;
        keyName = utility_space::findByRegex(oneLogStr, regFormula, groupRegKey);

        if(keyName.size() == 0) continue;

        if(groupRegValue != EMPTY_REGEX_GROUP)
        {
            RegexSiem regFormula = aggrCfgPtr->regexInfo.valueFindRegex;
            valueName = utility_space::findByRegex(oneLogStr, regFormula, groupRegValue);

            if(valueName.size() == 0) continue;
        }

        resolveAggrInfo.keyName = keyName;
        resolveAggrInfo.valueName = valueName;
        resolveAggrInfo.nodeId = aggrCfgPtr->nodeId;
        resolveAggrInfo.grabType = aggrCfgPtr->grabType;
        resolveAggrInfo.typeNode = aggrCfgPtr->typeNode;
        resolveAggrInfo.parentPath = aggrCfgPtr->parentNodePath;
        break;
    }

    return resolveAggrInfo;
}

std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> resolveFormatNodes(
    std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> bufCfg)
{
    std::sort(bufCfg->begin(), bufCfg->end(), 
        [](details::AggregationInfoNodeBuffer const &first, details::AggregationInfoNodeBuffer const &second)
        { return first.nodeId > second.nodeId; });

    auto &bufCfgVec = *bufCfg.get();

    for(size_t i(0); i < bufCfgVec.size(); i++)
    {
        bufCfgVec[i].parentPath = resolveById(bufCfgVec, bufCfgVec[i].parentPath);

        bufCfgVec[i].valueName = resolveById(bufCfgVec, bufCfgVec[i].valueName);
    }

    return bufCfg;
}

std::string resolveById(std::vector<details::AggregationInfoNodeBuffer> const &bufCfgVec, std::string const formatStr)
{
    std::string replacedStr = formatStr;
    std::vector<int> idVec = getNodesId(formatStr);

    if(idVec.size())
    {
        std::vector<std::pair<RegexSiem, std::string>> keyNames = getRegDstPairById(bufCfgVec, idVec);
        replacedStr = utility_space::replaceByRegexVec(replacedStr, keyNames);
    }

    return replacedStr;
}

std::vector<int> getNodesId(std::string formatStr)
{
    std::vector<int> nodesVec;
    std::vector<std::string> nodeIdExprs = StringManager::getVecStrBetweenSymbols(formatStr, L_SQ_BRACKET, R_SQ_BRACKET);

    for(std::string idExpr : nodeIdExprs)
    {
        if(StringManager::isSubstrIn(idExpr, ID_STR))
        {
            std::string idStr = StringManager::getAfterSymbol(idExpr, EQUAL);
            
            int idNum = atoi(idStr.c_str());
            nodesVec.push_back(idNum);
        }
    }

    return nodesVec;
}

std::vector<std::pair<RegexSiem, std::string>> getRegDstPairById(std::vector<details::AggregationInfoNodeBuffer> const &cfgVec,
    std::vector<int> const &idVec)
{
    std::vector<std::pair<RegexSiem, std::string>> keyNames;

    for(int id : idVec)
    {
        auto const &cfgIter = std::lower_bound(cfgVec.begin(), cfgVec.end(), id, 
            [](details::AggregationInfoNodeBuffer const &aggrInfo, int val) -> bool
        {
            return aggrInfo.nodeId > val;
        });

        if(cfgIter->nodeId == id)
        {
            RegexSiem regexp("[id=" + std::to_string(id) + "]");
            keyNames.push_back({regexp, cfgIter->keyName});
        }
    }

    return keyNames;
}