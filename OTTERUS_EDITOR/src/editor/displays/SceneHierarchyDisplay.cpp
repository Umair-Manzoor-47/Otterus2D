#include "SceneHierarchyDisplay.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Entity.h"
#include "Core/ECS/Components/AllComponents.h"
#include "Core/ECS/MetaUtilities.h"
#include "editor/utilities/ImGuiUtils.h"

using namespace otterus_core::ECS;
using namespace entt::literals;

namespace otterus_editor{

    auto create_entity = [](SceneObject& currentScene) {
        Entity newEntity{ currentScene.GetRegistry(), "GameObject", "" };
        newEntity.AddComponent<TransformComponent>();
    };

    bool SceneHierarchyDisplay::OpenTreeNode(Entity &entity) {

        auto& name = entity.GetName();

        ImGui::PushID(static_cast<int32_t>(entity.GetEntity()));

        ImGuiTreeNodeFlags treenNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding |
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (m_SelectedEntity && m_SelectedEntity->GetEntity() == entity.GetEntity())
            treenNodeFlags |= ImGuiTreeNodeFlags_Selected;
        bool treeNodeOpen { false };

        treeNodeOpen = ImGui::TreeNodeEx(name.c_str(), treenNodeFlags);

        if (ImGui::IsItemClicked()) {
            m_SelectedEntity = std::make_shared<Entity>( SCENE_MANAGER().GetCurrentScene()->GetRegistry(), entity.GetEntity() );
        }

        ImGui::PopID();
        return treeNodeOpen;

    }

    bool SceneHierarchyDisplay::AddComponent(otterus_core::ECS::Entity &entity, bool *addComponent) {
        return false;
    }

    void SceneHierarchyDisplay::DrawGameObjectDetails() {
        if (!ImGui::Begin("Inspector")) {
            ImGui::End();
            return;
        }

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Add Component"))
                m_AddComponent = true;
            ImGui::EndPopup();
        }

        if (m_SelectedEntity && m_AddComponent)
            AddComponent(*m_SelectedEntity, &m_AddComponent);


        if (m_SelectedEntity)
            DrawEntityComponents();

        ImGui::End();
    }

    void SceneHierarchyDisplay::DrawEntityComponents() {
        if (!m_SelectedEntity)
            return;

        auto& registry = m_SelectedEntity->GetRegistry();

        for (const auto&& [id, storage] : registry.storage() ) {
            if (!storage.contains(m_SelectedEntity->GetEntity()))
                continue;

            if (id == entt::type_hash<TileComponent>::value())
                continue;

            const auto drawInfo = otterus_core::Utils::InvokeMetaFunction(id, "DrawEntityComponentInfo"_hs, *m_SelectedEntity);

            if ( drawInfo )
            {
                ImGui::Spacing();
                ImGui::PushID( id );
                if ( ImGui::Button( "remove" ) )
                {
                    storage.remove( m_SelectedEntity->GetEntity() );
                }
                ImGui::PopID();
            }

            ImGui::Spacing();
            ImGui::Separator();

        }
    }

    SceneHierarchyDisplay::SceneHierarchyDisplay() {
    }

    SceneHierarchyDisplay::~SceneHierarchyDisplay() {
    }

    void SceneHierarchyDisplay::Update() {
        IDisplay::Update();
    }

    void SceneHierarchyDisplay::Draw() {

        auto currentScene = SCENE_MANAGER().GetCurrentScene();
        if (!ImGui::Begin("Scene Hierarchy") || !currentScene) {
            ImGui::End();
            return;
        }


        if (ImGui::BeginPopupContextWindow()) {

            if (ImGui::Selectable("Add New Game Object"))
                create_entity(*currentScene);

            ImGui::EndPopup();
        }


        auto& registery = currentScene->GetRegistry();
        auto sceneEntities = registery.GetRegistry().view<entt::entity>(entt::exclude<ScriptComponent, TileComponent>);

        for (auto entity : sceneEntities) {

            Entity ent{registery, entity };
            if (OpenTreeNode(ent))
                ImGui::TreePop();
        }

        ImGui::End();

        DrawGameObjectDetails();
    }
}
