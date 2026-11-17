#include "ContactListenerBind.h"
#include "../ECS/MetaUtilities.h"
#include <Physics/UserData.h>
#include <Logger/Logger.h>

namespace otterus_core::Scripting {
    std::tuple<sol::object, sol::object> ContactListenerBinder::GetUserData(otterus_physics::ContactListener& contactListener, sol::this_state s)
    {
        auto userDataA = contactListener.GetUserDataA();
        auto userDataB = contactListener.GetUserDataB();

        if (!userDataA || !userDataB)
        {
            return std::make_tuple (sol::lua_nil_t{}, sol::lua_nil_t{} );
        }
        
        OTTERUS_ASSERT( userDataA->typeId !=0 && userDataB->typeId != 0 && "UserData typeId must be set." );

        using namespace entt::literals;

        const auto maybe_any_a = otterus_core::Utils::InvokeMetaFunction(
            static_cast<entt::id_type>(userDataA->typeId),
            "get_user_data"_hs,
            *userDataA, s
        );

        const auto maybe_any_b = otterus_core::Utils::InvokeMetaFunction(
            static_cast<entt::id_type>(userDataB->typeId),
            "get_user_data"_hs,
            *userDataB, s
        );

        if (!maybe_any_a || !maybe_any_b)
        {
            return std::make_tuple(sol::lua_nil_t{}, sol::lua_nil_t{});
        }

        return std::make_tuple(
            maybe_any_a.cast<sol::reference>(),
            maybe_any_b.cast<sol::reference>()
        );
    }

    void ContactListenerBinder::CreateLuaContactListener(sol::state& lua, entt::registry& registry)
    {
        auto& contactListener = registry.ctx().get<std::shared_ptr<otterus_physics::ContactListener>>();
        if (!contactListener)
        {
            OTTERUS_ERROR("Failed to  create the contact listener binder, Contact listener in not in Registry.");
            return;
        }

        lua.new_usertype<otterus_physics::ContactListener>(
            "ContactListener",
            sol::no_constructor,
            "get_user_data", [&](sol::this_state state) {
                return GetUserData(*contactListener, state);
            }
        );
    }
}