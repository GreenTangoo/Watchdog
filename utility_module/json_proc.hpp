#ifndef JSON_PROC_HPP
#define JSON_PROC_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "filesystem_siem.hpp"
#include "string_manager.hpp"

namespace utility_space
{
    class IJsonContainer;
    typedef std::shared_ptr<IJsonContainer> IJsonContainerPtr;

    class IJsonContainer
    {
    public:
        enum class JsContainerType {
            jsObject, jsArray, jsString
        };
    public:
        virtual IJsonContainerPtr operator[](std::string const &key) = 0;
        virtual IJsonContainerPtr FindByName(std::string const &name) = 0;
    };


    class JsonContainer : public IJsonContainer
    {
    public:
        explicit JsonContainer() = default;
        explicit JsonContainer(JsContainerType type);
        explicit JsonContainer(JsonContainer const &other);
        explicit JsonContainer(JsonContainer &&other);
        virtual ~JsonContainer() = default;
        JsonContainer const& operator=(JsonContainer const &other);
        JsonContainer const& operator=(JsonContainer &&other);
        virtual IJsonContainerPtr operator[](std::string const &key) = 0;
        virtual IJsonContainerPtr FindByName(std::string const &name) override;
        JsContainerType GetType() const;
    private:
        JsContainerType m_JsType;
    };
    typedef std::shared_ptr<JsonContainer> JsonContainerPtr;


    class JsonObject : public JsonContainer
    {
    public:
        explicit JsonObject();
        explicit JsonObject(std::map<std::string, IJsonContainerPtr> const &values);
        explicit JsonObject(JsonObject const &other);
        explicit JsonObject(JsonObject &&other);
        virtual ~JsonObject() = default;
        JsonObject const& operator=(JsonObject const &other);
        JsonObject const& operator=(JsonObject &&other);
        virtual IJsonContainerPtr operator[](std::string const &key) override;
        explicit operator std::map<std::string, IJsonContainerPtr>() const;
    private:
        std::map<std::string, IJsonContainerPtr> m_MapValues;
    };
    typedef std::shared_ptr<JsonObject> JsonObjectPtr;


    class JsonArray : public JsonContainer
    {
    public:
        explicit JsonArray();
        explicit JsonArray(std::vector<IJsonContainerPtr> const &values);
        explicit JsonArray(JsonArray const &other);
        explicit JsonArray(JsonArray &&other);
        virtual ~JsonArray() = default;
        JsonArray const& operator=(JsonArray const &other);
        JsonArray const& operator=(JsonArray &&other);
        virtual IJsonContainerPtr operator[](std::string const &key) override;
        explicit operator std::vector<IJsonContainerPtr>() const;
    private:
        std::vector<IJsonContainerPtr> m_VecValues;
    };
    typedef std::shared_ptr<JsonArray> JsonArrayPtr;


    class JsonString : public JsonContainer
    {
    public:
        explicit JsonString();
        explicit JsonString(std::string const value);
        explicit JsonString(JsonString const &other);
        explicit JsonString(JsonString &&other);
        virtual ~JsonString() = default;
        JsonString const& operator=(JsonString const &other);
        JsonString const& operator=(JsonString &&other);
        virtual IJsonContainerPtr operator[](std::string const &key) override;
        explicit operator std::string() const;
    private:
        std::string m_Value;
    };
    typedef std::shared_ptr<JsonString> JsonStringPtr;

    IJsonContainerPtr CreateContainer(std::string const &value);
    IJsonContainerPtr CreateContainer(std::vector<IJsonContainerPtr> const &values);
    IJsonContainerPtr CreateContainer(std::map<std::string, IJsonContainerPtr> const &values);

    class IJsonSerializer
    {
    public:
        explicit IJsonSerializer(IJsonContainerPtr json);
        explicit IJsonSerializer(IJsonSerializer const &other);
        explicit IJsonSerializer(IJsonSerializer &&other);
        virtual void Write() = 0;
    protected:
        IJsonContainerPtr m_pJson;
    };

    class JsonFileSerializer : public IJsonSerializer
    {
    public:
        explicit JsonFileSerializer(IJsonContainerPtr json);
        explicit JsonFileSerializer(IJsonContainerPtr json, std::string const &filename);
        explicit JsonFileSerializer(JsonFileSerializer const &other);
        explicit JsonFileSerializer(JsonFileSerializer &&other);
        virtual ~JsonFileSerializer() = default;
        JsonFileSerializer const& operator=(JsonFileSerializer const &other);
        JsonFileSerializer const& operator=(JsonFileSerializer &&other);
        void SetFile(std::string const &filename);
        virtual void Write() override;
    private:
        static std::string AddQuaters(const std::string &value);
        static bool IsNeedSkipWriteComma(const size_t commaCount, const size_t containerSize);
        static void WriteSymbol(SymbolType const symbol, FileManipulator &writer);
        static void WriteJsonObject(IJsonContainerPtr pJson, FileManipulator &writer);
        static void WriteJsonArray(IJsonContainerPtr pJson, FileManipulator &writer);
        static void WriteJsonString(IJsonContainerPtr pJson, FileManipulator &writer);
    private:
        class JsonFileWriterResolver
        {
        public:
            static JsonFileWriterResolver& GetInstance();
            void RecursiveResolveWrite(IJsonContainerPtr pJson, FileManipulator &writer);
        };
    private:
        std::string m_Filename;
    };

    class IJsonDeserializer
    {
    public:
        virtual IJsonContainerPtr Read() = 0;
    };

    class JsonFileDeserializer : public IJsonDeserializer
    {
    public:
        explicit JsonFileDeserializer();
        explicit JsonFileDeserializer(std::string const &filename);
        explicit JsonFileDeserializer(JsonFileDeserializer const &other);
        explicit JsonFileDeserializer(JsonFileDeserializer &&other);
        virtual ~JsonFileDeserializer() = default;
        JsonFileDeserializer const& operator=(JsonFileDeserializer const &other);
        JsonFileDeserializer const& operator=(JsonFileDeserializer &&other);
        void SetFile(std::string const &filename);
        virtual IJsonContainerPtr Read() override;
    public:
        class JsonFileDeserializerException : public SIEMException
        {
        private:
            int m_Errno;
            char m_ReadedSymbol;
        public:
            enum JsonFileDeserializerErrCode
            {
                JSON_FILE_SYNTAX_ERROR = 1
            };
        public:
            JsonFileDeserializerException(std::string const &exMsg, int errCode, char symbol = 0);
            JsonFileDeserializerException(std::string &&exMsg, int errCode, char symbol = 0);
            ~JsonFileDeserializerException() = default;
            inline char GetLastSymbol();
        };
    private:
        static std::string ReadUntilSymbol(FileManipulator &reader, char const symbol);
        static void SkipStreamIncludeSymbol(FileManipulator &reader, char const symbol);
        static void SkipUselessSymbols(FileManipulator &reader);
        static std::string GetKey(FileManipulator &reader);
        static IJsonContainerPtr ReadObject(FileManipulator &reader);
        static IJsonContainerPtr ReadArray(FileManipulator &reader);
        static IJsonContainerPtr ReadString(FileManipulator &reader);
    private:
        class JsonFileReaderResolver
        {
        public:
            static JsonFileReaderResolver& GetInstance();
            IJsonContainerPtr RecursiveResolveRead(FileManipulator &reader);
        };
    private:
        std::string m_Filename;
    };
}

#endif // JSON_PROC_HPP
