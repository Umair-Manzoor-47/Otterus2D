#include "SceneObject.h"
#include <fmt/format.h>
#include "OtterusUtilities/HelperUtilities.h"

namespace otterus_editor
{
    SceneObject::SceneObject(const std::string& sceneName)
        : m_Registry{}
        , m_RuntimeRegistry{}
        , m_SceneName{sceneName}
        , m_Canvas{}
    {}

    SceneObject::~SceneObject()
    {}

    void SceneObject::CopySceneToRuntime()
    {
    }

    void SceneObject::ClearRuntime()
    {
    }

    void SceneObject::AddNewLayer()
    {
        static int number{ 0 };
        m_LayerParams.emplace_back(otterus_utils::SpriteLayerParams{ .layerName = fmt::format("NewLayer_{}", number++) });

    }
}
