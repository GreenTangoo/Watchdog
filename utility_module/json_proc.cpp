#include "json_proc.hpp"

using namespace utility_space;

namespace
{
    IJsonContainerPtr FindContainerByName(const JsonObject* pJsonObject, std::string const &name)
    {
        const std::map<std::string, IJsonContainerPtr> values =
                static_cast<const std::map<std::string, IJsonContainerPtr>>(*pJsonObject);

        const auto itFound = values.find(name);
        if(itFound != values.end())
            return itFound->second;

        for(const auto& it : values)
        {
            IJsonContainerPtr pFounded = it.second->FindByName(name);

            if(pFounded)
                return pFounded;
        }

        return nullptr;
    }

    IJsonContainerPtr FindContainerByName(const JsonArray* pJsonArray, std::string const &name)
    {
        const std::vector<IJsonContainerPtr> values =
                static_cast<const std::vector<IJsonContainerPtr>>(*pJsonArray);

        for(const auto& pContainer : values)
        {
            IJsonContainerPtr pFounded = pContainer->FindByName(name);

            if(pFounded)
                return pFounded;
        }

        return nullptr;
    }
}

// //////////////////////////////////////////////////////////////////////
// JsonContainer
JsonContainer::JsonContainer(JsContainerType type) :
    m_JsType(type)
{

}

JsonContainer::JsonContainer(JsonContainer const &other) :
    m_JsType(other.m_JsType)
{

}

JsonContainer::JsonContainer(JsonContainer &&other) :
    m_JsType(std::move(other.m_JsType))
{

}

JsonContainer const& JsonContainer::operator=(JsonContainer const &other)
{
    if(this != &other)
    {
        this->m_JsType = other.m_JsType;
    }

    return *this;
}

JsonContainer const& JsonContainer::operator=(JsonContainer &&other)
{
    if(this != &other)
    {
        this->m_JsType = std::move(other.m_JsType);
    }

    return *this;
}

IJsonContainerPtr JsonContainer::FindByName(std::string const &name)
{
    switch(m_JsType)
    {
    case JsContainerType::jsObject:
        return FindContainerByName(static_cast<const JsonObject*>(this), name);
        break;
    case JsContainerType::jsArray:
        return FindContainerByName(static_cast<const JsonArray*>(this), name);
        break;
    default:
        return nullptr;
        break;
    }
}

inline IJsonContainer::JsContainerType JsonContainer::GetType() const
{
    return m_JsType;
}


// //////////////////////////////////////////////////////////////////////
// JsonObject
JsonObject::JsonObject() :
    JsonContainer(JsContainerType::jsObject)
{

}

JsonObject::JsonObject(std::map<std::string, IJsonContainerPtr> const &values):
    JsonContainer(JsContainerType::jsObject), m_MapValues(values)
{

}

JsonObject::JsonObject(JsonObject const &other) :
    JsonContainer(other), m_MapValues(other.m_MapValues)
{

}

JsonObject::JsonObject(JsonObject &&other) :
    JsonContainer(std::move(other)), m_MapValues(std::move(other.m_MapValues))
{

}

JsonObject const& JsonObject::operator=(JsonObject const &other)
{
    if(this != &other)
    {
        JsonContainer::operator=(other);
        this->m_MapValues = other.m_MapValues;
    }

    return *this;
}

JsonObject const& JsonObject::operator=(JsonObject &&other)
{
    if(this != &other)
    {
        JsonContainer::operator=(std::move(other));
        this->m_MapValues = std::move(other.m_MapValues);
    }

    return *this;
}

IJsonContainerPtr JsonObject::operator[](std::string const &key)
{
    return m_MapValues[key];
}

JsonObject::operator std::map<std::string, IJsonContainerPtr>() const
{
    return m_MapValues;
}


// //////////////////////////////////////////////////////////////////////out
// JsonArray
JsonArray::JsonArray() :
    JsonContainer(JsContainerType::jsArray)
{

}

JsonArray::JsonArray(std::vector<IJsonContainerPtr> const &values) :
    JsonContainer(JsContainerType::jsArray), m_VecValues(values)
{

}

JsonArray::JsonArray(JsonArray const &other) :
    JsonContainer(other), m_VecValues(other.m_VecValues)
{

}

JsonArray::JsonArray(JsonArray &&other) :
    JsonContainer(std::move(other)), m_VecValues(std::move(other.m_VecValues))
{

}

JsonArray const& JsonArray::operator=(JsonArray const &other)
{
    if(this != &other)
    {
        JsonContainer::operator=(other);
        this->m_VecValues = other.m_VecValues;
    }

    return *this;
}

JsonArray const& JsonArray::operator=(JsonArray &&other)
{
    if(this != &other)
    {
        JsonContainer::operator=(std::move(other));
        this->m_VecValues = std::move(other.m_VecValues);
    }

    return *this;
}

IJsonContainerPtr JsonArray::operator[](std::string const &key)
{
    size_t index = static_cast<size_t>(std::atoi(key.c_str()));
    return m_VecValues[index];
}

JsonArray::operator std::vector<IJsonContainerPtr>() const
{
    return m_VecValues;
}


// //////////////////////////////////////////////////////////////////////
// JsonString
JsonString::JsonString() :
    JsonContainer(JsContainerType::jsString)
{

}

JsonString::JsonString(std::string const value) :
    JsonContainer(JsContainerType::jsString), m_Value(value)
{

}

JsonString::JsonString(JsonString const &other) :
    JsonContainer(other) ,m_Value(other.m_Value)
{

}

JsonString::JsonString(JsonString &&other) :
    JsonContainer(std::move(other)), m_Value(std::move(other.m_Value))
{

}

JsonString const& JsonString::operator=(JsonString const &other)
{
    if(this != &other)
    {
        JsonContainer::operator=(other);
        this->m_Value = other.m_Value;
    }

    return *this;
}

JsonString const& JsonString::operator=(JsonString &&other)
{
    if(this != &other)
    {
        JsonContainer::operator=(std::move(other));
        this->m_Value = std::move(other.m_Value);
    }

    return *this;
}

IJsonContainerPtr JsonString::operator [](std::string const &key)
{
    return std::make_shared<JsonString>(*this);
}

JsonString::operator std::string() const
{
    return m_Value;
}


// //////////////////////////////////////////////////////////////////////
// Public functions
IJsonContainerPtr utility_space::CreateContainer(std::string const &value)
{
    return std::make_shared<JsonString>(value);
}

IJsonContainerPtr utility_space::CreateContainer(std::vector<IJsonContainerPtr> const &values)
{
    return std::make_shared<JsonArray>(values);
}

IJsonContainerPtr utility_space::CreateContainer(std::map<std::string, IJsonContainerPtr> const &values)
{
    return std::make_shared<JsonObject>(values);
}


// //////////////////////////////////////////////////////////////////////
// JsonFileSerializer
IJsonSerializer::IJsonSerializer(IJsonContainerPtr json) :
    m_pJson(json)
{

}

IJsonSerializer::IJsonSerializer(IJsonSerializer const &other) :
    m_pJson(other.m_pJson)
{

}

IJsonSerializer::IJsonSerializer(IJsonSerializer &&other) :
    m_pJson(std::move(other.m_pJson))
{

}


// //////////////////////////////////////////////////////////////////////
// JsonFileSerializer
JsonFileSerializer::JsonFileSerializer(IJsonContainerPtr json) :
    IJsonSerializer(json)
{
}

JsonFileSerializer::JsonFileSerializer(IJsonContainerPtr json, std::string const &filename) :
    IJsonSerializer(json), m_Filename(filename)
{
}

JsonFileSerializer::JsonFileSerializer(JsonFileSerializer const &other) :
    IJsonSerializer(other), m_Filename(other.m_Filename)
{
}

JsonFileSerializer::JsonFileSerializer(JsonFileSerializer &&other) :
    IJsonSerializer(std::move(other)), m_Filename(std::move(other.m_Filename))
{
}

JsonFileSerializer const& JsonFileSerializer::operator=(JsonFileSerializer const &other)
{
    if(this != &other)
    {
        m_pJson = other.m_pJson;
        m_Filename = other.m_Filename;
    }

    return *this;
}

JsonFileSerializer const& JsonFileSerializer::operator=(JsonFileSerializer &&other)
{
    if(this != &other)
    {
        m_pJson = std::move(other.m_pJson);
        m_Filename = std::move(other.m_Filename);
    }

    return *this;
}

void JsonFileSerializer::SetFile(std::string const &filename)
{
    m_Filename = filename;
}

void JsonFileSerializer::Write()
{
    FileManipulator writer;
    writer.OpenManipulator(m_Filename,
                           FileManipulator::CREATE     |
                           FileManipulator::WRITEONLY  |
                           FileManipulator::TRUNCATE);

    JsonFileWriterResolver::GetInstance().RecursiveResolveWrite(m_pJson, writer);
}

std::string JsonFileSerializer::AddQuaters(const std::string &value)
{
    std::string newValue(1, static_cast<char>(SymbolType::DQUOTE));
    newValue += value;
    newValue += static_cast<char>(SymbolType::DQUOTE);

    return newValue;
}

bool JsonFileSerializer::IsNeedSkipWriteComma(const size_t commaCount, const size_t containerSize)
{
    return commaCount == containerSize;
}

void JsonFileSerializer::WriteSymbol(SymbolType const symbol, FileManipulator &writer)
{
    writer->WriteSymbol(static_cast<char>(symbol));
}

void JsonFileSerializer::WriteJsonObject(IJsonContainerPtr pJson, FileManipulator &writer)
{
    auto ConstructKeyForWrite = [](std::string const &key)
    {
        return AddQuaters(key) + static_cast<char>(SymbolType::DOUBLE_DOTS);
    };

    JsonObjectPtr pJsonObj = std::dynamic_pointer_cast<JsonObject>(pJson);

    std::map<std::string, IJsonContainerPtr> values =
            static_cast<std::map<std::string, IJsonContainerPtr>>(*pJsonObj.get());

    WriteSymbol(SymbolType::L_BRACKET, writer);

    size_t commaCounter = 0;
    const size_t containerSize = values.size();

    for(std::map<std::string, IJsonContainerPtr>::iterator it = values.begin(); it != values.end(); it++)
    {
        const std::string key = ConstructKeyForWrite(it->first);
        writer->Write(key.c_str(), key.length());

        JsonFileWriterResolver::GetInstance().RecursiveResolveWrite(it->second, writer);

        commaCounter++;
        if(IsNeedSkipWriteComma(commaCounter, containerSize))
        {
            break;
        }

        WriteSymbol(SymbolType::COMMA, writer);
    }

    WriteSymbol(SymbolType::R_BRACKET, writer);
}

void JsonFileSerializer::WriteJsonArray(IJsonContainerPtr pJson, FileManipulator &writer)
{
    JsonArrayPtr pJsonArray = std::dynamic_pointer_cast<JsonArray>(pJson);

    std::vector<IJsonContainerPtr> values =
            static_cast<std::vector<IJsonContainerPtr>>(*pJsonArray.get());

    WriteSymbol(SymbolType::L_SQ_BRACKET, writer);

    size_t commaCounter = 0;
    const size_t containerSize = values.size();

    for(std::vector<IJsonContainerPtr>::iterator it = values.begin(); it != values.end(); it++)
    {
        JsonFileWriterResolver::GetInstance().RecursiveResolveWrite(*it, writer);

        commaCounter++;
        if(IsNeedSkipWriteComma(commaCounter, containerSize))
        {
            break;
        }

        WriteSymbol(SymbolType::COMMA, writer);
    }

    WriteSymbol(SymbolType::R_SQ_BRACKET, writer);
}

void JsonFileSerializer::WriteJsonString(IJsonContainerPtr pJson, FileManipulator &writer)
{
    JsonStringPtr pJsonStr = std::dynamic_pointer_cast<JsonString>(pJson);

    std::string valueStr = static_cast<std::string>(*pJsonStr.get());
    valueStr = AddQuaters(valueStr);

    writer->Write(valueStr.c_str(), valueStr.length());
}


// //////////////////////////////////////////////////////////////////////
// JsonFileSerializer::JsonFileWriterResolver
JsonFileSerializer::JsonFileWriterResolver& JsonFileSerializer::JsonFileWriterResolver::GetInstance()
{
    static JsonFileWriterResolver resolver;
    return resolver;
}

void JsonFileSerializer::JsonFileWriterResolver::RecursiveResolveWrite(IJsonContainerPtr pJson, FileManipulator &writer)
{
    const JsonContainerPtr pJsonBase = std::dynamic_pointer_cast<JsonContainer>(pJson);
    const IJsonContainer::JsContainerType jsType = pJsonBase->GetType();

    switch(jsType)
    {
    case IJsonContainer::JsContainerType::jsObject:
        JsonFileSerializer::WriteJsonObject(pJson, writer);
        break;
    case IJsonContainer::JsContainerType::jsArray:
        JsonFileSerializer::WriteJsonArray(pJson, writer);
        break;
    case IJsonContainer::JsContainerType::jsString:
        JsonFileSerializer::WriteJsonString(pJson, writer);
        break;
    }
}


// //////////////////////////////////////////////////////////////////////
// JsonFileDeserializer
JsonFileDeserializer::JsonFileDeserializer()
{

}

JsonFileDeserializer::JsonFileDeserializer(std::string const &filename) :
    m_Filename(filename)
{

}

JsonFileDeserializer::JsonFileDeserializer(JsonFileDeserializer const &other) :
    m_Filename(other.m_Filename)
{

}

JsonFileDeserializer::JsonFileDeserializer(JsonFileDeserializer &&other) :
    m_Filename(std::move(other.m_Filename))
{

}

JsonFileDeserializer const& JsonFileDeserializer::operator=(JsonFileDeserializer const &other)
{
    if(this != &other)
    {
        m_Filename = other.m_Filename;
    }

    return *this;
}

JsonFileDeserializer const& JsonFileDeserializer::operator=(JsonFileDeserializer &&other)
{
    if(this != &other)
    {
        m_Filename = std::move(other.m_Filename);
    }

    return *this;
}

void JsonFileDeserializer::SetFile(std::string const &filename)
{
    m_Filename = filename;
}

IJsonContainerPtr JsonFileDeserializer::Read()
{
    FileManipulator reader;
    reader.OpenManipulator(m_Filename, O_RDONLY | O_LARGEFILE);

    return JsonFileReaderResolver::GetInstance().RecursiveResolveRead(reader);
}

std::string JsonFileDeserializer::ReadUntilSymbol(FileManipulator &reader, char const symbol)
{
    std::string readStr;
    char ch = 0;
    while(reader->ReadSymbol(ch) != std::ios_base::iostate::_S_eofbit)
    {
        if(ch == symbol)
            break;

        readStr.push_back(ch);
    }

    return readStr;
}

void JsonFileDeserializer::SkipStreamIncludeSymbol(FileManipulator &reader, char const symbol)
{
    char ch = 0;
    while(reader->ReadSymbol(ch) != std::ios_base::iostate::_S_eofbit)
    {
        if(ch == symbol)
            break;
    }
}

void JsonFileDeserializer::SkipUselessSymbols(FileManipulator &reader)
{
    // SKip ' ', '\n', '\r' symbols
    char ch = 0;
    while(reader->ReadSymbol(ch) != std::ios_base::iostate::_S_eofbit)
    {
        if((ch != ' ')  &&
           (ch != '\n') &&
           (ch != '\r'))
        {
            reader->PutbackSymbol(ch);
            break;
        }
    }
}

std::string JsonFileDeserializer::GetKey(FileManipulator &reader)
{
    char ch = 0;
    std::string key = JsonFileDeserializer::ReadUntilSymbol(reader, static_cast<char>(SymbolType::DQUOTE));

    SkipUselessSymbols(reader);

    reader->ReadSymbol(ch);
    if(ch != static_cast<char>(SymbolType::DOUBLE_DOTS))
    {
        throw JsonFileDeserializerException("Error occured due json file reading",
                                            JsonFileDeserializerException::JSON_FILE_SYNTAX_ERROR, ch);
    }

    return key;
}

IJsonContainerPtr JsonFileDeserializer::ReadObject(FileManipulator &reader)
{
    std::map<std::string, IJsonContainerPtr> values;
    std::string key;
    char ch = 0;

    SkipUselessSymbols(reader);

    while(reader->ReadSymbol(ch) != std::ios_base::iostate::_S_eofbit)
    {
        if(ch == static_cast<char>(SymbolType::R_BRACKET))
            break;

        if(ch == static_cast<char>(SymbolType::COMMA))
            continue;

        if(ch == static_cast<char>(SymbolType::DQUOTE))
        {
            key = GetKey(reader);
        }

        IJsonContainerPtr value = JsonFileReaderResolver::GetInstance().RecursiveResolveRead(reader);
        if(value)
        {
            values.insert({key, value});
        }

        key.clear();
    }

    return CreateContainer(values);
}

IJsonContainerPtr JsonFileDeserializer::ReadArray(FileManipulator &reader)
{
    std::vector<IJsonContainerPtr> values;
    char ch = 0;

    SkipUselessSymbols(reader);

    while(reader->ReadSymbol(ch) != std::ios_base::iostate::_S_eofbit)
    {
        if(ch == static_cast<char>(SymbolType::R_SQ_BRACKET))
            break;

        if(ch == static_cast<char>(SymbolType::COMMA))
            continue;

        reader->PutbackSymbol(ch);

        SkipUselessSymbols(reader);

        IJsonContainerPtr pContainer = JsonFileReaderResolver::GetInstance().RecursiveResolveRead(reader);
        values.push_back(pContainer);
    }

    return CreateContainer(values);
}

IJsonContainerPtr JsonFileDeserializer::ReadString(FileManipulator &reader)
{
    std::string value = JsonFileDeserializer::ReadUntilSymbol(reader, static_cast<char>(SymbolType::DQUOTE));

    return CreateContainer(value);
}


// //////////////////////////////////////////////////////////////////////
// JsonFileDeserializer::JsonFileReaderResolver
JsonFileDeserializer::JsonFileReaderResolver& JsonFileDeserializer::JsonFileReaderResolver::GetInstance()
{
    static JsonFileReaderResolver reader;
    return reader;
}

IJsonContainerPtr JsonFileDeserializer::JsonFileReaderResolver::RecursiveResolveRead(FileManipulator &reader)
{
    char ch = 0;
    operationState state = reader->ReadSymbol(ch);

    if(state == std::ios_base::iostate::_S_eofbit)
        return nullptr;

    switch(static_cast<SymbolType>(ch))
    {
    case SymbolType::L_BRACKET:
        return JsonFileDeserializer::ReadObject(reader);
        break;
    case SymbolType::L_SQ_BRACKET:
        return JsonFileDeserializer::ReadArray(reader);
        break;
    case SymbolType::DQUOTE:
        return JsonFileDeserializer::ReadString(reader);
        break;
    default:
        throw JsonFileDeserializerException("Error occured due json file reading",
                                            JsonFileDeserializerException::JSON_FILE_SYNTAX_ERROR, ch);
    }
}


// //////////////////////////////////////////////////////////////////////
// JsonFileDeserializer::JsonFileDeserializerException
JsonFileDeserializer::JsonFileDeserializerException::JsonFileDeserializerException(std::string const &exMsg, int errCode, char symbol) :
    SIEMException(exMsg, errCode), m_ReadedSymbol(symbol)
{

}

JsonFileDeserializer::JsonFileDeserializerException::JsonFileDeserializerException(std::string &&exMsg, int errCode, char symbol) :
    SIEMException(std::move(exMsg), errCode), m_ReadedSymbol(symbol)
{

}

char JsonFileDeserializer::JsonFileDeserializerException::GetLastSymbol()
{
    return m_ReadedSymbol;
}
