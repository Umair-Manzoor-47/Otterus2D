#include "SceneHierarchyDisplay.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Entity.h"
#include "Core/ECS/Components/AllComponents.h"
#include "Core/ECS/MetaUtilities.h"
#include "editor/utilities/ImGuiUtils.h"
#include "editor/utilities/EditorTheme.h"

using namespace otterus_core::ECS;
using namespace entt::literals;

namespace otterus_editor{

    auto create_entity = [](SceneObject& currentScene) {
        Entity newEntity{ currentScene.GetRegistry(), "GameObject", "" };
        newEntity.AddComponent<TransformComponent>();
    };

    bool SceneHierarchyDisplay::OpenTreeNode(Entity &entity) {

        auto& name = entity.GetName();
        const char* displayName = name.empty() ? "GameObject" : name.c_str();

        ImGui::PushID(static_cast<int32_t>(entity.GetEntity()));

        ImGuiTreeNodeFlags treenNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding |
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (m_SelectedEntity && m_SelectedEntity->GetEntity() == entity.GetEntity())
            treenNodeFlags |= ImGuiTreeNodeFlags_Selected;
        bool treeNodeOpen { false };

        treeNodeOpen = ImGui::TreeNodeEx(displayName, treenNodeFlags);

        if (ImGui::IsItemClicked()) {
            m_SelectedEntity = std::make_shared<Entity>( SCENE_MANAGER().GetCurrentScene()->GetRegistry(), entity.GetEntity() );
        }

        ImGui::PopID();
        return treeNodeOpen;

    }

    bool SceneHierarchyDisplay::AddComponent(Entity &entity, bool *addComponent) {

        if (!addComponent)
            return false;

        if (addComponent && *addComponent)
            ImGui::OpenPopup("Add Component");

        if (ImGui::BeginPopupModal("Add Component"))
        {
            auto& registry = entity.GetRegistry();

            std::map<entt::id_type, std::string> componentsMap;

            for (auto&& [id, type] : entt::resolve())
            {
                const auto& info = type.info();
                auto pos = info.name().find_last_of(':')+1;
                auto name = info.name().substr(pos);
                componentsMap[id] = std::string{ name };
            }

            static std::string componentStr{ "" };
            static std::string componentStrPrev{ "" };
            static entt::id_type id_type{0};
            static bool hasError{false};

            if (ImGui::BeginCombo("Choose Component", componentStr.c_str()))
            {
                for (auto& [id, name] : componentsMap)
                {
                    if (ImGui::Selectable(name.c_str(), name == componentStr))
                    {
                        componentStr = name;
                        id_type = id;
                    }
                }
                ImGui::EndCombo();
            }

            if (componentStr != componentStrPrev)
            {
                componentStrPrev = componentStr;
                hasError = false;
            }

            if (hasError)
            {
                ImGui::TextColored(EditorTheme::GetTokens().errorColor, "%s",
                    fmt::format("Game Object already has [{}].", componentStr).c_str());
            }

            if (ImGui::Button("Add"))
            {
                if (!componentStr.empty())
                {
                    for (auto&& [id, storage] : registry.storage())
                    {
                        if (id != id_type)
                            continue;
                        if (storage.contains(entity.GetEntity()))
                        {
                            hasError = true;
                            OTTERUS_ERROR("Entity already has the component [{}].", componentStr);
                            break;
                        }
                        // No need to throw since component is not associated with entity yet
                        break;
                    }
                }

                if (!hasError)
                {
                    auto&& storage = registry.storage(id_type);
                    if (!storage)
                    {
                        const auto addComponentDefault =
                            otterus_core::Utils::InvokeMetaFunction(id_type, "add_component_default"_hs, entity);
                        if (addComponentDefault)
                        {
                            *addComponent = false;
                            ImGui::CloseCurrentPopup();
                        }
                        else
                        {
                            OTTERUS_ASSERT(addComponentDefault && "Failed to add component");
                            hasError = true;

                        }
                    }
                    else
                    {
                        storage->push(entity.GetEntity());
                        *addComponent = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                hasError = false;
                *addComponent = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

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
        ImGui::PushID(static_cast<int32_t>(m_SelectedEntity->GetEntity()));

        constexpr entt::id_type idType = entt::type_hash<Identification>::value();
        constexpr entt::id_type transformType = entt::type_hash<TransformComponent>::value();
        constexpr entt::id_type tileType = entt::type_hash<TileComponent>::value();

        // Always draw Identification and Transform first (non-removable)
        for (const entt::id_type fixedId : { idType, transformType })
        {
            if (auto* storage = registry.storage(fixedId); storage && storage->contains(m_SelectedEntity->GetEntity()))
            {
                otterus_core::Utils::InvokeMetaFunction(fixedId, "DrawEntityComponentInfo"_hs, *m_SelectedEntity);
                ImGui::Spacing();
                ImGui::Separator();
            }
        }

        for (const auto&& [id, storage] : registry.storage()) {
            if (!storage.contains(m_SelectedEntity->GetEntity()))
                continue;

            if (id == tileType || id == idType || id == transformType)
                continue;

            const auto drawInfo = otterus_core::Utils::InvokeMetaFunction(id, "DrawEntityComponentInfo"_hs, *m_SelectedEntity);

            if (drawInfo)
            {
                ImGui::PushID(id);
                const float buttonWidth = ImGui::CalcTextSize("Remove Component").x + ImGui::GetStyle().FramePadding.x * 2.0f;
                const float avail = ImGui::GetContentRegionAvail().x;
                if (avail > buttonWidth)
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + avail - buttonWidth);
                if (ImGui::SmallButton("Remove Component"))
                {
                    storage.remove(m_SelectedEntity->GetEntity());
                }
                ImGui::PopID();

                ImGui::Spacing();
                ImGui::Separator();
            }
        }

        ImGui::PopID();
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
