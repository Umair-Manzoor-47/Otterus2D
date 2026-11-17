#pragma once
#include <Logger/Logger.h>
#include "State.h"
#include <map>


namespace otterus_core
{
    class StateMachine
    {
    private:
        std::map<std::string, std::unique_ptr<State>> m_MapStates;
        std::string m_CurrentState;
        std::optional<sol::table> m_StateTable;

    public:
        StateMachine();
        StateMachine(const sol::table& stateFuncs);

        void ChangeState(const std::string& stateName, bool removeState = false,
            const sol::object& enterParams = sol::lua_nil_t{});
        void AddState(const State& newState);
        void RemoveState(const std::string& name);


        void Update(const float dt);
        void Render();
        void ExitState();
        void DestroyStates();

        const std::string& CurrentState() const { return m_CurrentState; };

        static void CreateLuaStateMachine(sol::state& lua);
    };
}
