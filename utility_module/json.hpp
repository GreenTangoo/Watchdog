
#ifndef JSON_H
#define JSON_H

#include <fstream>
#include <vector>
#include <memory>
#include <list>

#include "string_manager.hpp"
#include "../exception_module/exceptions.hpp"

namespace utility_space
{
    enum typeNodeJSON {OBJECT = '{', ARRAY = '[', END_OBJECT = '}', END_ARRAY = ']', STRING, ARRAY_ELEMENT, NONE};

    struct JsonContainer
    {
        std::pair<std::string, std::string> keyValue;
        typeNodeJSON typeNode;

        std::shared_ptr<JsonContainer> nextNode;
        std::shared_ptr<JsonContainer> prevNode;
        std::shared_ptr<JsonContainer> parentNode;
        std::shared_ptr<JsonContainer> childNode;

        JsonContainer();
        JsonContainer(const JsonContainer &other);
        JsonContainer(JsonContainer &&other);
        JsonContainer& operator=(const JsonContainer &other);

        void recursiveCopyElements(JsonContainer &destObj, const JsonContainer &srcObj);
        void recursiveClearElements(std::shared_ptr<JsonContainer> parentNode);
        void moveElements(JsonContainer &destObj, JsonContainer &srcObj);

    };

    class JsonObject
    {
    private:
        std::shared_ptr<JsonContainer> rootNode;
    private:
        std::shared_ptr<JsonContainer> findByName(std::shared_ptr<JsonContainer> node, 
            const std::string &keyName) const;
        std::vector<std::shared_ptr<JsonContainer>> findsByName(std::shared_ptr<JsonContainer> node, 
            const std::string &keyName) const;
        std::shared_ptr<JsonContainer> findByPath(std::shared_ptr<JsonContainer> node, 
            const std::string &path);
        void addChild(std::shared_ptr<JsonContainer>node, JsonContainer &childNode);
        void addNeighbor(std::shared_ptr<JsonContainer> node, JsonContainer &neighborNode);
        void clearEmptyJsonNodes(std::shared_ptr<JsonContainer> node);
    public:
        JsonObject();
        JsonObject(const JsonContainer &otherContainer);
        JsonObject(const JsonObject &other);
        JsonObject(JsonObject &&other);
        ~JsonObject();
        JsonObject& operator=(const JsonObject &other);
        void setContainer(const JsonContainer &otherContainer);
        void addArray(const std::string &keyNode, const std::vector<std::string> &values, 
            const std::string &parentpath);
        void addOrUpdateArray(const std::string &keyNode, const std::vector<std::string> &values, 
            const std::string &arrayPath);
        void updateArray(const std::string &keyNode, const std::vector<std::string> &values, 
            const std::string &arrayPath);
        void addArrayElement(const std::string &value, const std::string &parentPath);
        void addEmptyNode(const std::string &keyNode, const std::string &parentpath);
        void addOrUpdateNode(const std::string &keyNode, const std::string &nodePath);
        void updateNodeName(const std::string &keyNode, const std::string &nodePath);
        void addString(const std::pair<std::string, std::string> &keyValue, 
            const std::string &parentpath);
        void updateString(const std::pair<std::string, std::string> &keyValue, 
            const std::string &stringPath);
        void addOrUpdateString(const std::pair<std::string, std::string> &keyValue, 
            const std::string &stringPath);
        void clearJson();
        void getJson(std::istream &in);
        void setJson(std::ostream &out, bool formatOut = false) const;
        std::shared_ptr<JsonContainer> findElementByName(const std::string &keyName) const;
        std::vector<std::shared_ptr<JsonContainer>> findElementsByName(const std::string &keyName) const;
        std::shared_ptr<JsonContainer> findElementByPath(const std::string &path);
        std::shared_ptr<JsonContainer> findElementByTemplate(const std::string &templateString);
        std::vector<std::shared_ptr<JsonContainer>> findElementsByTemplate(const std::string &templateString);
    };

    class JsonStreamParser // SINGLETON
    {
    private:
        JsonStreamParser();
        JsonStreamParser(const JsonStreamParser &other) = delete;
        JsonStreamParser(JsonStreamParser &&other) = delete;
        JsonStreamParser& operator=(const JsonStreamParser &other) = delete;
        JsonStreamParser& operator=(JsonStreamParser &&other) = delete;
        ~JsonStreamParser();
        std::string getUntilSymbol(std::istream &in, symbolType delimeterSymbol);
        void fillContainer(JsonContainer &node, const std::string &first, const std::string &second, 
            const typeNodeJSON nodeType);
        std::shared_ptr<JsonContainer> getNode(std::istream &in, 
            std::shared_ptr<JsonContainer> parentNode);
        std::shared_ptr<JsonContainer> getArrayElement(std::istream &in, 
            std::shared_ptr<JsonContainer> parentNode);
        void putNode(std::ostream &out, std::shared_ptr<JsonContainer> node) const;
        void putArrayElement(std::ostream &out, std::shared_ptr<JsonContainer> node) const;
        void putNodeFormat(std::ostream &out, std::shared_ptr<JsonContainer> node, 
            size_t offset) const;
        void putArrayElementFormat(std::ostream &out, std::shared_ptr<JsonContainer> node, 
            size_t offset) const;
    public:
        static JsonStreamParser& getInstance();
        std::shared_ptr<JsonContainer> getFromStream(std::istream &in, 
            std::shared_ptr<JsonContainer> parentNode);
        void putToStream(std::ostream &out, std::shared_ptr<JsonContainer> jsonNode) const;
        void putToStreamFormat(std::ostream &out, std::shared_ptr<JsonContainer> jsonNode, 
            size_t offset = 0) const;
    };

    class JSONNodeTypeResolver // SINGLETON
    {
    public:
        JSONNodeTypeResolver();
        JSONNodeTypeResolver(const JSONNodeTypeResolver &other) = delete;
        JSONNodeTypeResolver(JSONNodeTypeResolver &&other) = delete;
        JSONNodeTypeResolver operator=(const JSONNodeTypeResolver &other) = delete;
        static JSONNodeTypeResolver& getInstance();
        typeNodeJSON getNodeType(const std::string &nodeName);
        std::string getNodeName(const typeNodeJSON &typeNode);
    };
}

utility_space::JsonObject getJsonData(std::string jsonFilename);

#endif