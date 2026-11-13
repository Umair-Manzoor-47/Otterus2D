#include "SceneManager.h"
#include "SceneObject.h"
#include "OtterusUtilities/OtterusUtilities.h"
#include "Logger.h"

namespace otterus_editor
{
    SceneManager& SceneManager::GetInstance()
    {
        static SceneManager instance{};
        return instance;
    }

    bool SceneManager::AddScene(const std::string& sceneName)
    {
        if (m_mapScenes.contains(sceneName))
        {
            OTTERUS_ERROR( "Failed to add new scene object - [{}] already exists.", sceneName );
            return false;
        }

        auto [itr, success] = m_mapScenes.emplace( sceneName, std::move( std::make_shared<otterus_editor::SceneObject>( sceneName ) ) );
        return success;
    }

    std::shared_ptr<otterus_editor::SceneObject> SceneManager::GetScene(const std::string& sceneName)
    {
        auto sceneItr = m_mapScenes.find( sceneName );
        if (sceneItr == m_mapScenes.end())
        {
            OTTERUS_ERROR( "Failed to get scene object - [{}] does not exist.", sceneName );
            return nullptr;
        }

        return sceneItr->second;
    }

    std::shared_ptr<otterus_editor::SceneObject> SceneManager::GetCurrentScene()
    {
        if ( m_CurrentScene.empty() )
            return nullptr;

        auto sceneItr = m_mapScenes.find( m_CurrentScene );
        if ( sceneItr == m_mapScenes.end() )
        {
            OTTERUS_ERROR( "Failed to get scene object - [{}] does not exist.", m_CurrentScene );
            return nullptr;
        }

        return sceneItr->second;
    }

    std::vector<std::string> SceneManager::GetSceneNames() const
    {
        return otterus_utils::GetKeys( m_mapScenes );
    }
}
