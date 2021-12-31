#include "json.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define ARRAY_NODE "array"
#define OBJECT_NODE "object"
#define STRING_NODE "string"
#define NONE_NODE ""

/*--------------------------------------------------------------------------------*/
/*----------------------------------JSON CONTAINER--------------------------------*/
/*--------------------------------------------------------------------------------*/
JsonContainer::JsonContainer()
{
    typeNode = NONE;
}

JsonContainer::JsonContainer(const JsonContainer &other)
{
    *this = other;
}

JsonContainer::JsonContainer(JsonContainer &&other)
{
    this->moveElements(*this, other);
}

JsonContainer& JsonContainer::operator=(const JsonContainer &other)
{
    if(this != &other)
        recursiveCopyElements(*this, other);

    return *this;
}

void JsonContainer::recursiveCopyElements(JsonContainer &destObj, const JsonContainer &srcObj)
{
    destObj.keyValue = srcObj.keyValue;
    destObj.typeNode = srcObj.typeNode;

    destObj.prevNode = srcObj.prevNode;
    destObj.parentNode = srcObj.parentNode;

    if(srcObj.childNode != nullptr)
    {
        if(destObj.childNode == nullptr)
            destObj.childNode = std::make_shared<JsonContainer>(JsonContainer());

        recursiveCopyElements(*(destObj.childNode.get()), *(srcObj.childNode.get()));
    }
    if(srcObj.nextNode != nullptr)
    {
        if(destObj.nextNode == nullptr)
            destObj.nextNode = std::make_shared<JsonContainer>(JsonContainer());

        recursiveCopyElements(*(destObj.nextNode.get()), *(srcObj.nextNode.get()));
    }
}

void JsonContainer::recursiveClearElements(std::shared_ptr<JsonContainer> parentNode)
{
    if(parentNode->childNode != nullptr)
        parentNode->childNode.reset();
    if(parentNode->nextNode != nullptr)
        parentNode->nextNode.reset();
}

void JsonContainer::moveElements(JsonContainer &destObj, JsonContainer &srcObj)
{
    destObj.keyValue = srcObj.keyValue;
    destObj.typeNode = srcObj.typeNode;

    srcObj.keyValue.first = srcObj.keyValue.second = "";
    srcObj.typeNode = NONE;

    if(srcObj.childNode != nullptr)
    {
        destObj.childNode = srcObj.childNode;
        srcObj.childNode = nullptr;
    }
    if(srcObj.nextNode != nullptr)
    {
        destObj.nextNode = srcObj.nextNode;
        srcObj.nextNode = nullptr;
    }
}

/*----------------------------------------------------------------------------------*/
/*-----------------------------------JSON OBJECT------------------------------------*/
/*----------------------------------------------------------------------------------*/
JsonObject::JsonObject()
{
    rootNode = std::make_shared<JsonContainer>(JsonContainer());
    rootNode->typeNode = OBJECT;
    rootNode->keyValue.first = "root";
}

JsonObject::JsonObject(const JsonObject &other)
{
    rootNode = std::make_shared<JsonContainer>(JsonContainer());
    rootNode = other.rootNode;
}

JsonObject::JsonObject(const JsonContainer &otherContainer)
{
    rootNode = std::make_shared<JsonContainer>(JsonContainer());
    rootNode->keyValue.first = "root";
    setContainer(otherContainer);
}

JsonObject::JsonObject(JsonObject &&other)
{
    rootNode = std::move(other.rootNode);
}

JsonObject::~JsonObject()
{

}

JsonObject& JsonObject::operator=(const JsonObject &other)
{
    if(this != &other)
    {
        rootNode = other.rootNode;
    }

    return *this;
}

void JsonObject::setContainer(const JsonContainer &otherContainer)
{
    rootNode->childNode = std::make_shared<JsonContainer>();
    rootNode->recursiveCopyElements(*(rootNode->childNode.get()), otherContainer);
}

void JsonObject::addArrayElement(const std::string &value, const std::string &parentPath)
{
    std::shared_ptr<JsonContainer> foundedArrayNode = findByPath(rootNode, parentPath);

    if(foundedArrayNode == nullptr)
    {
        throw JsonException("Cannot find node by path: " + parentPath + " in addArrayElement method", 
			JsonException::BAD_PATH);
    }

    JsonContainer newArrayElementContainer;
    newArrayElementContainer.keyValue.first = value;
    newArrayElementContainer.typeNode = ARRAY_ELEMENT;
    addChild(foundedArrayNode, newArrayElementContainer);
}

void JsonObject::addArray(const std::string &keyNode, const std::vector<std::string> &values, const std::string &parentPath)
{
    JsonContainer newContainer;
    newContainer.keyValue.first = keyNode;
    newContainer.typeNode = ARRAY;

    std::shared_ptr<JsonContainer> newContainerPtr = std::make_shared<JsonContainer>(newContainer);

    for(size_t i(0); i < values.size(); i++)
    {
        JsonContainer newArrayElementContainer;
        newArrayElementContainer.keyValue.first = values[i];
        newArrayElementContainer.typeNode = ARRAY_ELEMENT;
        this->addChild(newContainerPtr, newArrayElementContainer);
    }

    std::shared_ptr<JsonContainer> foundedContainer = findByPath(rootNode, parentPath);

    if(foundedContainer == nullptr)
    {
        throw JsonException("Cannot find node by path: " + parentPath + " in addArrayMethod",
											JsonException::BAD_PATH);
    }

    addChild(foundedContainer, *(newContainerPtr.get()));
}

void JsonObject::addOrUpdateArray(const std::string &keyNode, const std::vector<std::string> &values, const std::string &arrayPath)
{
    try
    {
        updateArray(keyNode, values, arrayPath);
    }
    catch(const JsonException &ex)
    {
        std::string stringParentPath = StringManager::cropFromEnd(arrayPath, SLASH);
        addArray(keyNode, values, stringParentPath);
    }
}

void JsonObject::updateArray(const std::string &keyNode, const std::vector<std::string> &values, const std::string &arrayPath)
{
    std::shared_ptr<JsonContainer> foundedArrayNode = findByPath(rootNode, arrayPath);

    if(foundedArrayNode == nullptr)
    {
        throw  JsonException("Cannot find node by path: " + arrayPath + " in updateArray method",
			JsonException::BAD_PATH);
    }

    if(foundedArrayNode->childNode != nullptr)
    {
        foundedArrayNode->recursiveClearElements(foundedArrayNode->childNode);
    }

    foundedArrayNode->keyValue.first = keyNode;

    for(size_t i(0); i < values.size(); i++)
    {
        JsonContainer newArrayElementContainer;
        newArrayElementContainer.keyValue.first = values[i];
        newArrayElementContainer.typeNode = ARRAY_ELEMENT;
        this->addChild(foundedArrayNode, newArrayElementContainer);
    }
}

void JsonObject::addEmptyNode(const std::string &keyNode, const std::string &parentPath)
{
    JsonContainer newContainer;
    newContainer.keyValue.first = keyNode;
    newContainer.typeNode = OBJECT;

    std::shared_ptr<JsonContainer> foundedContainer = findByPath(rootNode, parentPath);

    if(foundedContainer == nullptr)
    {
        throw JsonException("Cannot find node by path: " + parentPath + " in addEmptyNode method",
			JsonException::BAD_PATH);
    }

    addChild(foundedContainer, newContainer);
}

void JsonObject::updateNodeName(const std::string &keyNode, const std::string &nodePath)
{
    std::shared_ptr<JsonContainer> foundedNode = findByPath(rootNode, nodePath);

    if(foundedNode == nullptr)
    {
        throw JsonException("Cannot find node by path: " + nodePath + " in updateNodeName method",
			JsonException::BAD_PATH);
    }

    foundedNode->keyValue.first = keyNode;
}

void JsonObject::addOrUpdateNode(const std::string &keyNode, const std::string &nodePath)
{
    try
    {
        updateNodeName(keyNode, nodePath);
    }
    catch(const JsonException &ex)
    {
        std::string stringParentPath = StringManager::cropFromEnd(nodePath, SLASH);
        addEmptyNode(keyNode, stringParentPath);
    }
}

void JsonObject::addString(const std::pair<std::string, std::string> &keyValue, const std::string &parentPath)
{
    JsonContainer newContainer;
    newContainer.keyValue = keyValue;
    newContainer.typeNode = STRING;

    std::shared_ptr<JsonContainer> foundedContainer = findByPath(rootNode, parentPath);

    if(foundedContainer == nullptr)
    {
        throw JsonException("Cannot find node by path: " + parentPath + " in addString method",
			JsonException::BAD_PATH);
    }

    addChild(foundedContainer, newContainer);
}

void JsonObject::updateString(const std::pair<std::string, std::string> &keyValue, const std::string &stringPath)
{
    std::shared_ptr<JsonContainer> foundedStringNode = findByPath(rootNode, stringPath);

    if(foundedStringNode == nullptr)
    {
        throw JsonException("Cannot find node by path: " + stringPath + " in updateString method",
			JsonException::BAD_PATH);
    }

    foundedStringNode->keyValue = keyValue;
}

void JsonObject::addOrUpdateString(const std::pair<std::string, std::string> &keyValue, const std::string &stringPath)
{
    try
    {
        updateString(keyValue, stringPath);
    }
    catch(const JsonException &ex)
    {
        std::string stringParenPath = StringManager::cropFromEnd(stringPath, SLASH);
        addString(keyValue, stringParenPath);
    }
}

void JsonObject::clearJson()
{
    rootNode->typeNode = NONE;
    if(rootNode->childNode)
        rootNode->childNode.reset();
    if(rootNode->nextNode)
        rootNode->nextNode.reset();
}

void JsonObject::getJson(std::istream &in)
{
    rootNode = JsonStreamParser::getInstance().getFromStream(in, rootNode);
    rootNode->parentNode = nullptr;
    rootNode->nextNode = nullptr;
    clearEmptyJsonNodes(rootNode);
    rootNode->keyValue.first = "root";
}

void JsonObject::setJson(std::ostream &out, bool formatOut) const
{
    if(!formatOut)
        JsonStreamParser::getInstance().putToStream(out, rootNode->childNode);
    else
        JsonStreamParser::getInstance().putToStreamFormat(out, rootNode->childNode);
}

std::shared_ptr<JsonContainer> JsonObject::findElementByName(const std::string &keyName) const
{
    std::shared_ptr<JsonContainer> foundedNodePtr = findByName(rootNode, keyName);
    return foundedNodePtr;
}

std::vector<std::shared_ptr<JsonContainer>> JsonObject::findElementsByName(const std::string &keyName) const
{
    std::vector<std::shared_ptr<JsonContainer>> foundedVecPtr = findsByName(rootNode, keyName);
    return foundedVecPtr;
}

std::shared_ptr<JsonContainer> JsonObject::findElementByTemplate(const std::string &templateString)
{
	return nullptr;
}

std::vector<std::shared_ptr<JsonContainer>> JsonObject::findElementsByTemplate(const std::string &temlateString)
{
	return std::vector<std::shared_ptr<JsonContainer>>();
}

std::shared_ptr<JsonContainer> JsonObject::findElementByPath(const std::string &path)
{
    std::shared_ptr<JsonContainer> foundedNodePtr = findByPath(rootNode, path);
    return foundedNodePtr;
}

std::shared_ptr<JsonContainer> JsonObject::findNearElementByName(const std::string &keyName,
            size_t elementNumber) const
{
    std::shared_ptr<JsonContainer> foundedNodePtr = findNearByName(rootNode, keyName, elementNumber);
    return foundedNodePtr;
}

/*---------------------------------------------------------------------------------------------*/
/*------------------------------------JSON OBJECT(PRIVATE)-------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
std::shared_ptr<JsonContainer> JsonObject::findByName(std::shared_ptr<JsonContainer> node, 
    const std::string &keyName) const
{
    for(std::shared_ptr<JsonContainer> itPtr = node; itPtr != nullptr; itPtr = itPtr->nextNode)
    {
        std::string nodeKey = itPtr->keyValue.first;

        if(nodeKey == keyName)
            return itPtr;

        if((itPtr->childNode != nullptr))
        {
            std::shared_ptr<JsonContainer> foundedJsonPtr = findByName(itPtr->childNode, 
                keyName);
            if(foundedJsonPtr != nullptr)
                return foundedJsonPtr;
        }
    }

    return nullptr;
}

std::vector<std::shared_ptr<JsonContainer>> JsonObject::findsByName(std::shared_ptr<JsonContainer> node, 
    const std::string &keyName) const
{
    std::vector<std::shared_ptr<JsonContainer>> foundedVec;

    for(std::shared_ptr<JsonContainer> itPtr = node; itPtr != nullptr; itPtr = itPtr->nextNode)
    {
        std::string nodeKey = itPtr->keyValue.first;

        if(nodeKey == keyName)
            foundedVec.push_back(itPtr);

        if(itPtr->childNode != nullptr)
        {
            std::vector<std::shared_ptr<JsonContainer>> returnedVec = findsByName(itPtr->childNode, 
                keyName);
            std::copy(returnedVec.begin(), returnedVec.end(), std::back_inserter(foundedVec));
        }
    }

    return foundedVec;
}

std::shared_ptr<JsonContainer> JsonObject::findNearByName(std::shared_ptr<JsonContainer> node,
            const std::string &keyName, size_t elementNumber) const
{
    if(!(node->childNode))
    {
        throw JsonException("Empty container passed",
            JsonException::EMPTY_CONTAINER);
    }
    node = node->childNode; // Move down from root node

    for(std::shared_ptr<JsonContainer> itNode = node; 
        itNode != nullptr; itNode = itNode->nextNode)
    {
        std::string name = itNode->keyValue.first;
        if(name == keyName)
        {
            if(elementNumber > 0)
            {
                elementNumber--;
            }
            return itNode;
        }
    }

    return nullptr;
}

std::shared_ptr<JsonContainer> JsonObject::findByPath(std::shared_ptr<JsonContainer> node, 
    const std::string &path)
{
    std::shared_ptr<JsonContainer> foundedNodePtr = node;
    std::vector<std::string> pathNodesVec = StringManager::parseByDelimiter(path, "/");

    for(std::vector<std::string>::iterator it = pathNodesVec.begin(); it != pathNodesVec.end(); it++)
    {
        std::string nodeName = *it;
        foundedNodePtr = findByName(foundedNodePtr, nodeName);
        if(foundedNodePtr == nullptr)
        {
            return nullptr;
        }
    }

    return foundedNodePtr;
}

void JsonObject::addChild(std::shared_ptr<JsonContainer> node, JsonContainer &childNode)
{
    if(node->childNode == nullptr)
    {
        JsonContainer addingContainer(childNode);
        node->childNode = std::make_shared<JsonContainer>(addingContainer);
        node->childNode->parentNode = node;
    }
    else
    {
        std::shared_ptr<JsonContainer> nodePtr = node->childNode;
        for(; nodePtr->nextNode != nullptr; nodePtr = nodePtr->nextNode);

        addNeighbor(nodePtr, childNode);
    }
}

void JsonObject::addNeighbor(std::shared_ptr<JsonContainer> node, JsonContainer &neighborNode)
{
    JsonContainer addingContainer(neighborNode);

    node->nextNode = std::make_shared<JsonContainer>(addingContainer);
    node->nextNode->prevNode = node;
    node->nextNode->parentNode = node->parentNode;
}

void JsonObject::clearEmptyJsonNodes(std::shared_ptr<JsonContainer> node)
{
    if(node->childNode != nullptr)
    {
        size_t lengthNodeFirstString = node->childNode->keyValue.first.length();
        if((node->childNode->childNode == nullptr) &&
                (node->childNode->nextNode == nullptr) &&
                (lengthNodeFirstString == 0))
        {
            node->childNode.reset();
        }
        else
        {
            clearEmptyJsonNodes(node->childNode);
        }
    }

    if(node->nextNode != nullptr)
    {
        size_t lengthNodeFirstString = node->nextNode->keyValue.first.length();
        if((node->nextNode->childNode == nullptr) &&
                (node->nextNode->nextNode == nullptr) &&
                (lengthNodeFirstString == 0))
        {
            node->nextNode.reset();
        }
        else
        {
            clearEmptyJsonNodes(node->nextNode);
        }
    }
}

/*--------------------------------------------------------------------------------------*/
/*------------------------------------JSON STREAM PARSER--------------------------------*/
/*--------------------------------------------------------------------------------------*/
JsonStreamParser::JsonStreamParser()
{

}

JsonStreamParser::~JsonStreamParser()
{

}

JsonStreamParser& JsonStreamParser::getInstance()
{
    static JsonStreamParser obj;
    return obj;
}

std::shared_ptr<JsonContainer> JsonStreamParser::getFromStream(std::istream &in, std::shared_ptr<JsonContainer> parentNode)
{
    return getNode(in, parentNode);
}

void JsonStreamParser::putToStream(std::ostream &out, std::shared_ptr<JsonContainer> jsonNode) const
{
    out.write("{", 1);
    putNode(out, jsonNode);
    out.write("}", 1);
}

void JsonStreamParser::putToStreamFormat(std::ostream &out, std::shared_ptr<JsonContainer> jsonNode, 
    size_t offset) const
{
    out.write("{\n", 2);
    putNodeFormat(out, jsonNode, offset + 4);
    out.write("}", 1);
}

/*--------------------------------------------------------------------------------------*/
/*----------------------------------JSON STREAM PARSER(PRIVATE)-------------------------*/
/*--------------------------------------------------------------------------------------*/
std::string JsonStreamParser::getUntilSymbol(std::istream &in, symbolType delimeterSymbol)
{
    std::string getString;
    char symbol = 0;

    while(in.get(symbol))
    {
        if(symbol == delimeterSymbol)
            break;

        getString.push_back(symbol);
    }

    return getString;
}

std::shared_ptr<JsonContainer> JsonStreamParser::getNode(std::istream &in, std::shared_ptr<JsonContainer> parentNode)
{
    std::shared_ptr<JsonContainer> newContainerPtr
        = std::make_shared<JsonContainer>(JsonContainer());

    char symbol = 0;
    std::string firstStringNodeVar;
    std::string secondStringNodeVar;

    while(in.get(symbol))
    {
        if(symbol == END_OBJECT)
            break;

        if(symbol == DQUOTE)
        {
            if(firstStringNodeVar.length() == 0)
            {
                firstStringNodeVar = getUntilSymbol(in, DQUOTE);
                continue;
            }
            else
            {
                secondStringNodeVar = getUntilSymbol(in, DQUOTE);
                newContainerPtr->keyValue =
                        std::pair<std::string, std::string>(firstStringNodeVar, secondStringNodeVar);

                newContainerPtr->typeNode = STRING;

                newContainerPtr->nextNode = getNode(in, parentNode);
                newContainerPtr->nextNode->prevNode = newContainerPtr;
                newContainerPtr->parentNode = parentNode;
                break;
            }
        }

        if(symbol == OBJECT)
        {
            newContainerPtr->keyValue.first = firstStringNodeVar;
            newContainerPtr->typeNode = OBJECT;

            newContainerPtr->childNode = getNode(in, newContainerPtr);
            newContainerPtr->parentNode = parentNode;
            newContainerPtr->nextNode = getNode(in, parentNode);
            newContainerPtr->nextNode->prevNode = newContainerPtr;
            break;
        }

        if(symbol == ARRAY)
        {
            newContainerPtr->keyValue.first = firstStringNodeVar;
            newContainerPtr->typeNode = ARRAY;

            newContainerPtr->childNode = getArrayElement(in, newContainerPtr);
            newContainerPtr->childNode->parentNode = parentNode;
            newContainerPtr->nextNode = getNode(in, parentNode);
            newContainerPtr->nextNode->prevNode = newContainerPtr;
            break;
        }
    }

    return newContainerPtr;
}

std::shared_ptr<JsonContainer> JsonStreamParser::getArrayElement(std::istream &in, std::shared_ptr<JsonContainer> parentNode)
{
    std::shared_ptr<JsonContainer> arrayElementContainerPtr
        = std::make_shared<JsonContainer>(JsonContainer());

    arrayElementContainerPtr->typeNode = ARRAY_ELEMENT;

    std::string arrayElementString;
    char symbol = 0;
    while(in.get(symbol))
    {
        if(symbol == END_ARRAY)
        {
            if(arrayElementString.length() > 0)
                arrayElementContainerPtr->keyValue.first = arrayElementString;

            break;
        }

        if((symbol == SPACE) || (symbol == NEW_LINE) || (symbol == DQUOTE) || (symbol == TAB))
            continue;

        if(symbol == COMMA)
        {
            arrayElementContainerPtr->keyValue.first = arrayElementString;

            arrayElementContainerPtr->nextNode = getArrayElement(in, parentNode);
            arrayElementContainerPtr->nextNode->prevNode = arrayElementContainerPtr;
            arrayElementContainerPtr->parentNode = parentNode;
            break;
        }

        arrayElementString.push_back(symbol);
    }

    return arrayElementContainerPtr;
}

void JsonStreamParser::putNode(std::ostream &out, std::shared_ptr<JsonContainer> node) const
{
    for(std::shared_ptr<JsonContainer> nodeIter = node; nodeIter != nullptr; 
        nodeIter = nodeIter->nextNode)
    {
        if(nodeIter.get()->typeNode == STRING)
        {
            std::string key = nodeIter.get()->keyValue.first;
            std::string value = nodeIter.get()->keyValue.second;

            std::string writeStr =  "\"" + key + "\":\"" + value + "\"";
            out.write(writeStr.c_str(), static_cast<long>(writeStr.length()));

            if(nodeIter->nextNode)
                out.write(",", 1);

            continue;
        }

        if(nodeIter.get()->typeNode == OBJECT)
        {
            std::string key = nodeIter.get()->keyValue.first;
            key = "\"" + key + "\"";
            out.write(key.c_str(), static_cast<long>(key.length()));
            out.write(":{", 2);
            putNode(out, nodeIter->childNode);
            out.write("}", 1);

            if(nodeIter->nextNode)
                out.write(",", 1);

            continue;
        }

        if(nodeIter.get()->typeNode == ARRAY)
        {
            std::string key = nodeIter.get()->keyValue.first;
            key = "\"" + key + "\"";
            out.write(key.c_str(), static_cast<long>(key.length()));
            out.write(":[", 2);
            putArrayElement(out, nodeIter->childNode);
            out.write("]", 1);

            if(nodeIter->nextNode)
                out.write(",", 1);
        }
    }
}

void JsonStreamParser::putArrayElement(std::ostream &out, std::shared_ptr<JsonContainer> node) const
{
    for(std::shared_ptr<JsonContainer> nodeIter = node; nodeIter != nullptr; 
        nodeIter = nodeIter->nextNode)
    {
        std::string arrayElement = nodeIter.get()->keyValue.first;
        arrayElement = "\"" + arrayElement + "\"";
        out.write(arrayElement.c_str(), static_cast<long>(arrayElement.length()));

        if(nodeIter->nextNode)
            out.write(",", 1);
    }
}

void JsonStreamParser::putNodeFormat(std::ostream &out, std::shared_ptr<JsonContainer> node, 
    size_t offset) const
{
    std::string offsetStr(" ", offset);

    for(std::shared_ptr<JsonContainer> nodeIter = node; nodeIter != nullptr; 
        nodeIter = nodeIter->nextNode)
    {
        if(nodeIter.get()->typeNode == STRING)
        {
            std::string key = nodeIter.get()->keyValue.first;
            std::string value = nodeIter.get()->keyValue.second;

            std::string writeStr = offsetStr + key + ":" + value;
            out.write(writeStr.c_str(), static_cast<long>(writeStr.length()));

            if(nodeIter->nextNode)
                out.write(",", 1);

            out.write("\n", 1);

            continue;
        }

        if(nodeIter.get()->typeNode == OBJECT)
        {
            std::string key = nodeIter.get()->keyValue.first;

            std::string writeStr = offsetStr + key + ":{\n";
            out.write(writeStr.c_str(), static_cast<long>(writeStr.length()));

            putNodeFormat(out, nodeIter->childNode, offset + 4);
            out.write(std::string(offsetStr + "}\n").c_str(), static_cast<long>(offset + 2));

            if(nodeIter->nextNode)
                out.write(",", 1);

            out.write("\n", 1);

            continue;
        }

        if(nodeIter.get()->typeNode == ARRAY)
        {
            std::string key = nodeIter.get()->keyValue.first;

            std::string writeStr = offsetStr + key + ":[\n";
            out.write(writeStr.c_str(), static_cast<long>(writeStr.length()));

            putArrayElementFormat(out, nodeIter->childNode, offset + 4);
            out.write(std::string(offsetStr + "]\n").c_str(), static_cast<long>(offset + 2));

            if(nodeIter->nextNode)
                out.write(",", 1);

            out.write("\n", 1);
        }
    }
}

void JsonStreamParser::putArrayElementFormat(std::ostream &out, std::shared_ptr<JsonContainer> node, 
    size_t offset) const
{
    std::string offsetStr(" ", offset);
    for(std::shared_ptr<JsonContainer> nodeIter = node; nodeIter != nullptr; nodeIter = nodeIter->nextNode)
    {
        std::string arrayElement = nodeIter.get()->keyValue.first;
        std::string writeStr = offsetStr + arrayElement;

        out.write(writeStr.c_str(), static_cast<long>(writeStr.length()));

        if(nodeIter->nextNode)
            out.write(",", 1);
        out.write("\n", 1);
    }
}

/*-------------------------------------------------------------------------------------------------*/
/*------------------------------------TYPE JSON NODE RESOLVER--------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/
JSONNodeTypeResolver::JSONNodeTypeResolver()
{

}

JSONNodeTypeResolver& JSONNodeTypeResolver::getInstance()
{
    static JSONNodeTypeResolver instance;
    return instance;
}

typeNodeJSON JSONNodeTypeResolver::getNodeType(const std::string &nodeName)
{
    if(nodeName == "object")
        return OBJECT;
    else if(nodeName == "array")
        return ARRAY;
    else if(nodeName == "string")
        return STRING;
    else
        return NONE;
}

std::string JSONNodeTypeResolver::getNodeName(const typeNodeJSON &typeNode)
{
    if(typeNode == OBJECT)
        return OBJECT_NODE;
    else if(typeNode == ARRAY)
        return ARRAY_NODE;
    else if(typeNode == STRING)
        return STRING_NODE;
    else
        return NONE_NODE;
}

/*-------------------------------------------------------------------------------*/
/*---------------------------------PUBLIC FUNCTIONS------------------------------*/
/*-------------------------------------------------------------------------------*/
std::shared_ptr<JsonContainer> utility_space::guaranteeGetPtrByName(JsonObject const &obj, std::string nameStr)
{
	std::shared_ptr<JsonContainer> foundedPtr = obj.findElementByName(nameStr);

	if(!foundedPtr)
	{
		throw JsonException("Node name: " + nameStr + " doesn't exists", 
			JsonException::BAD_NODE);
	}

	return foundedPtr;
}

JsonObject utility_space::getJsonData(std::string jsonFilename)
{
    std::ifstream fin;
    fin.open(jsonFilename, std::ios_base::in);

    if(fin.is_open() == false)
    {
        throw SIEMException("Cannot open file: " + jsonFilename, SIEMException::BAD_FILE);
    }

    JsonObject readJsonObj;
    readJsonObj.getJson(fin);

    fin.close();
    return readJsonObj;
}

JsonObject utility_space::getJsonData(std::stringstream &stream)
{
    JsonObject jsonObj;
    jsonObj.getJson(stream);

    return jsonObj;
}

JsonObject utility_space::getJsonData(std::fstream &stream)
{
    JsonObject readJsonObj;
    readJsonObj.getJson(stream);

    return readJsonObj;
}

void utility_space::setJsonData(std::fstream &stream, JsonObject const &jsonObj)
{
    std::shared_ptr<JsonContainer> rootNode = jsonObj.findElementByName("root");

    JsonStreamParser::getInstance().putToStream(stream, rootNode);
}
