#pragma once

#include <fstream>
#include <rapidjson/prettywriter.h>

namespace otterus_filesystem
{
    class JSONSerializer
    {
    private:
        std::fstream m_Filestream;
        rapidjson::StringBuffer m_StringBuffer;
        std::unique_ptr<rapidjson::PrettyWriter<rapidjson::StringBuffer>> m_Writer;
        int m_NumObjectsStarted, m_NumArraysStarted;

    public:
        JSONSerializer(const std::string& filename);
        ~JSONSerializer();

        bool StartDocument();
        bool EndDocument();

        JSONSerializer& StartNewObject(const std::string& key = "");
        JSONSerializer& EndObject();

        JSONSerializer& StartNewArray(const std::string& key);
        JSONSerializer& EndArray();

        bool Reset(const std::string& filename);

        template <typename TValue>
        JSONSerializer& AddKeyValuePair(const std::string& key, const TValue& value);

        template <>
        JSONSerializer& AddKeyValuePair(const std::string& key, const bool& value);

    };

}
#include "JSONSerializer.inl"