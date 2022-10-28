#ifndef DATA_STORAGE_HPP
#define DATA_STORAGE_HPP

#include <memory>

namespace utility_space
{
    class IDataStorage
    {
    };
    typedef std::shared_ptr<IDataStorage> IDataStoragePtr;

    class ISerializer
    {
    public:
        virtual void Write() = 0;
    };

    class IDeserializer
    {
    };
}

#endif // DATA_STORAGE_HPP
