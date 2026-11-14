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

    bool SceneManager::DeleteScene(const std::string& sceneName)
    {
        if (sceneName.empty())
        {
            OTTERUS_ERROR("Failed to delete scene - scene name is empty.");
            return false;
        }

        auto sceneItr = m_mapScenes.find(sceneName);
        if (sceneItr == m_mapScenes.end())
        {
            OTTERUS_ERROR("Failed to delete scene object - [{}] does not exist.", sceneName);
            return false;
        }

        m_mapScenes.erase(sceneItr);

        if (m_CurrentScene == sceneName)
        {
            m_CurrentScene.clear();
        }

        return true;
    }

    bool SceneManager::RenameScene(const std::string& oldName, const std::string& newName)
    {
        if (oldName.empty() || newName.empty())
        {
            OTTERUS_ERROR("Failed to rename scene - old or new name is empty.");
            return false;
        }

        if (oldName == newName)
            return true;

        auto sceneItr = m_mapScenes.find(oldName);
        if (sceneItr == m_mapScenes.end())
        {
            OTTERUS_ERROR("Failed to rename scene object - [{}] does not exist.", oldName);
            return false;
        }

        if (m_mapScenes.contains(newName))
        {
            OTTERUS_ERROR("Failed to rename scene object - [{}] already exists.", newName);
            return false;
        }

        auto scene = sceneItr->second;
        if (scene)
        {
            scene->SetName(newName);
        }

        if (!otterus_utils::KeyChange(m_mapScenes, oldName, newName))
        {
            OTTERUS_ERROR("Failed to change key for scene [{}] to [{}].", oldName, newName);
            return false;
        }

        if (m_CurrentScene == oldName)
        {
            m_CurrentScene = newName;
        }

        return true;
    }

    bool SceneManager::HasScene(const std::string& sceneName) const
    {
        return m_mapScenes.contains(sceneName);
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
