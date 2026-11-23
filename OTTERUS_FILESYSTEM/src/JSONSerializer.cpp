#include "OtterusFileSystem/Serializers/JSONSerializer.h"
#include "Logger.h"

constexpr int MAX_DECIMAL_PLACES = 3;

namespace otterus_filesystem
{
    JSONSerializer::JSONSerializer(const std::string& filename)
    : m_Filestream{}
    , m_StringBuffer{}
    , m_Writer{ std::make_unique<rapidjson::PrettyWriter<rapidjson::StringBuffer>>( m_StringBuffer ) }
    , m_NumObjectsStarted{ 0 }
    , m_NumArraysStarted{ 0 }
    {
        m_Filestream.open( filename, std::ios::out | std::ios::trunc );
        OTTERUS_ASSERT( m_Filestream.is_open() && "Failed to open file!" );

        if ( !m_Filestream.is_open() )
            throw std::runtime_error( fmt::format( "JSONSerializer failed to open file [{}]", filename ) );

        m_Writer->SetMaxDecimalPlaces( MAX_DECIMAL_PLACES );
    }

    JSONSerializer::~JSONSerializer()
    {
        if (m_Filestream.is_open())
            m_Filestream.close();
    }

    bool JSONSerializer::StartDocument()
    {
        OTTERUS_ASSERT( m_NumObjectsStarted == 0 && "Document has already been started. Please Reset the serializer." );

        if (m_NumObjectsStarted != 0)
        {
            OTTERUS_ERROR( "Document has already been started. Please Reset the serializer." );
            return false;
        }

        ++m_NumObjectsStarted;

        return m_Writer->StartObject();
    }

    bool JSONSerializer::EndDocument()
    {
        OTTERUS_ASSERT( m_NumObjectsStarted == 1 && "There should only be one object open." );
        OTTERUS_ASSERT( m_NumArraysStarted == 0 && "Too many arrays started. Did you forget to call EndArray()?" );

        if ( m_NumObjectsStarted != 1 )
        {
            OTTERUS_ERROR( "Failed to end document correctly. There should only be one object open. Did you forget to call EndObject()?" );
            return false;
        }

        if ( m_NumArraysStarted != 0 )
        {
            OTTERUS_ERROR( "Failed to end document correctly. Too many arrays started. Did you forget to call "
                         "EndArray()?" );
            return false;
        }

        m_Writer->EndObject();
        --m_NumObjectsStarted;

        m_Filestream << m_StringBuffer.GetString();
        m_Filestream.flush();

        return true;
    }

    JSONSerializer& JSONSerializer::StartNewObject(const std::string& key)
    {
        ++m_NumObjectsStarted;

        if (!key.empty())
            m_Writer->Key(key.c_str());

        m_Writer->StartObject();
        return *this;
    }

    JSONSerializer& JSONSerializer::EndObject()
    {
        OTTERUS_ASSERT( m_NumObjectsStarted > 1 && "EndObject() called too many times!" );
        --m_NumObjectsStarted;
        m_Writer->EndObject();
        return *this;
    }

    JSONSerializer& JSONSerializer::StartNewArray(const std::string& key)
    {
        ++m_NumArraysStarted;
        m_Writer->Key( key.c_str() );

        m_Writer->StartArray();
        return *this;
    }

    JSONSerializer& JSONSerializer::EndArray()
    {
        OTTERUS_ASSERT( m_NumArraysStarted > 0 && "EndArray() called too many times!" );
        --m_NumArraysStarted;
        m_Writer->EndArray();
        return *this;
    }

    bool JSONSerializer::Reset( const std::string& filename )
    {
        OTTERUS_ASSERT( m_NumObjectsStarted == 0 && "Cannot Reset, Document has not been finished." );
        OTTERUS_ASSERT( m_NumArraysStarted == 0 && "Cannot reset, Too many arrays open" );

        if ( m_NumObjectsStarted != 0 )
        {
            OTTERUS_ERROR( "Cannot Reset, Document has not been finished." );
            return false;
        }

        if ( m_NumArraysStarted != 0 )
        {
            OTTERUS_ERROR( "Cannot reset, Too many arrays open" );
            return false;
        }

        if ( m_Filestream.is_open() )
            m_Filestream.close();

        m_Filestream.open( filename, std::ios::out | std::ios::trunc );
        OTTERUS_ASSERT( m_Filestream.is_open() && "Failed to open file!" );

        if ( !m_Filestream.is_open() )
            throw std::runtime_error( fmt::format( "JSONSerializer failed to open file [{}]", filename ) );

        return true;
    }
}
