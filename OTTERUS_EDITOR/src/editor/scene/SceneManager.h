#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>


#define SCENE_MANAGER() otterus_editor::SceneManager::GetInstance()

namespace otterus_editor
{
    class SceneObject;
    class ToolManager;

    class SceneManager
    {
    private:
        std::map<std::string, std::shared_ptr<otterus_editor::SceneObject>> m_mapScenes;
        std::string m_CurrentScene{ "" }, m_CurrentTileset{ "" };

        std::unique_ptr<ToolManager> m_ToolManager{ nullptr };
    private:
        SceneManager() = default;
        ~SceneManager() = default;
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;

    public:
        static SceneManager& GetInstance();

        bool AddScene( const std::string& sceneName );
        bool DeleteScene( const std::string& sceneName );
        bool RenameScene( const std::string& oldName, const std::string& newName );
        bool HasScene( const std::string& sceneName ) const;

        std::shared_ptr<SceneObject> GetScene( const std::string& sceneName );
        std::shared_ptr<SceneObject> GetCurrentScene();
        std::vector<std::string> GetSceneNames() const;
        ToolManager& GetToolManager ();

        void SetCurrentTileset ( const std::string& tilesetID );
        inline void SetCurrentScene( const std::string& sceneName ) { m_CurrentScene = sceneName; }
        inline const std::string& GetCurrentSceneName() const { return m_CurrentScene; }
        inline const std::string& GetCurrentTileset() const { return m_CurrentTileset; }
    };

}
