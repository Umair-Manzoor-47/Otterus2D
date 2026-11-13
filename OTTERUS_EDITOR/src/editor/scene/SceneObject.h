#pragma once
#include "Core/ECS/Entity.h"
#include "editor/utilities/EditorUtilities.h"

namespace otterus_editor
{

    class SceneObject
    {
    private:
        otterus_core::ECS::Registry m_Registry, m_RuntimeRegistry;
        std::string m_SceneName;
        Canvas m_Canvas;

    public:
        SceneObject(const std::string& sceneName);
        ~SceneObject() = default;

        void CopySceneToRuntime();
        void ClearRuntime();

        inline Canvas& GetCanvas(){ return m_Canvas; }
        inline const std::string& GetName() { return m_SceneName; }
        inline otterus_core::ECS::Registry& GetRegistry() { return m_Registry; }
        inline otterus_core::ECS::Registry& GetRuntimeRegistry() { return m_RuntimeRegistry; }

    };

}
