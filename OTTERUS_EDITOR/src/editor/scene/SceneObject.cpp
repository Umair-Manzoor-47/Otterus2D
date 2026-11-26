#include "SceneObject.h"
#include <fmt/format.h>
#include <Core/ECS/Components/AllComponents.h>
#include <Core/ECS/MetaUtilities.h>
#include "OtterusUtilities/HelperUtilities.h"

using namespace otterus_core::ECS;
using namespace entt::literals;

namespace otterus_editor
{
    SceneObject::SceneObject(const std::string& sceneName)
        : m_Registry{}
        , m_RuntimeRegistry{}
        , m_SceneName{sceneName}
        , m_Canvas{}
        , m_Lighting{}
    {}

    SceneObject::~SceneObject()
    {}

    void SceneObject::CopySceneToRuntime()
    {
        auto& registryToCopy = m_Registry.GetRegistry();

        for (auto entityToCopy : registryToCopy.view<entt::entity>(entt::exclude<ScriptComponent>))
        {
            entt::entity newEntity = m_RuntimeRegistry.CreateEntity();

            for (auto&& [id_type, storage] : registryToCopy.storage())
            {
                if (!storage.contains(entityToCopy))
                    continue;
                otterus_core::Utils::InvokeMetaFunction(id_type, "copy_component"_hs,
                    Entity{m_Registry, entityToCopy}, Entity{m_RuntimeRegistry, newEntity});

            }
        }

    }

    void SceneObject::ClearRuntime()
    {
        m_RuntimeRegistry.ClearRegistry();
    }

    void SceneObject::AddNewLayer()
    {
        static int number{ 0 };
        m_LayerParams.emplace_back(otterus_utils::SpriteLayerParams{ .layerName = fmt::format("NewLayer_{}", number++) });

    }
}
