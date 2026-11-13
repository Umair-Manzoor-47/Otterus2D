#include "SceneObject.h"

namespace otterus_editor
{
    SceneObject::SceneObject(const std::string& sceneName)
        : m_Registry{}
        , m_RuntimeRegistry{}
        , m_SceneName{sceneName}
        , m_Canvas{}
    {}

    void SceneObject::CopySceneToRuntime()
    {
    }

    void SceneObject::ClearRuntime()
    {
    }
}
