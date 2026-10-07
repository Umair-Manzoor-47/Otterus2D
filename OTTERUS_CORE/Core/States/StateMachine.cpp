#include "StateMachine.h"

namespace otterus_core
{
    StateMachine::StateMachine()
        : StateMachine(sol::lua_nil_t{})
    {}

    StateMachine::StateMachine(const sol::table& stateFuncs)
        : m_MapStates{}
        , m_CurrentState{ "" }
        , m_StateTable{stateFuncs}
    {}

    void StateMachine::ChangeState(const std::string& stateName, bool removeState, const sol::object& enterParams)
    {
        /*if (m_StateTable) {
            ChangeStateTable(newState, removeState, enterParams);
            return;
        } */
        auto stateItr = m_MapStates.find(stateName);
        if (stateItr == m_MapStates.end())
        {
            OTTERUS_ERROR("Failed to change state, [{}] could not be found.", stateName);
            return;
        }
        auto& newState = stateItr->second;

        if (m_CurrentState.empty())
        {
            m_CurrentState = stateName;
        }
        else
        {
            auto& oldState = m_MapStates.at(m_CurrentState);
            if (oldState->on_exit.valid())
            {
                try
                {
                    auto result = oldState->on_exit();
                    if (!result.valid())
                    {
                        sol::error err = result;
                        throw err;
                    }
                }
                catch (const sol::error& e)
                {
                    OTTERUS_ERROR("Failed to exit_state: {}", e.what());
                    return;
                }
            }
            if (removeState)
                oldState->killState = true;

            m_CurrentState = stateName;
        }
        if (newState->on_enter.valid())
        {
            try
            {
                auto result = newState->on_enter();
                if (!result.valid())
                {
                    sol::error err = result;
                    throw err;
                }
            }
            catch (const sol::error& e)
            {
                OTTERUS_ERROR("Failed to enter_state: {}", e.what());
                return;
            }
        }

    }

    void StateMachine::AddState(const State& newState)
    {
        if (m_MapStates.contains(newState.name))
        {
            OTTERUS_ERROR("Failed to add state [{}], state already exists.", newState.name);
            return;
        }

        m_MapStates.emplace(newState.name, std::make_unique<State>(newState));
    }

    void StateMachine::RemoveState(const std::string& name)
    {


    }

    void StateMachine::Update(const float dt)
    {
        try
        {   auto stateItr = m_MapStates.find(m_CurrentState);
            if (stateItr == m_MapStates.end())
                return;

            auto& newState = stateItr->second;

            if (newState->on_update.valid())
            {

                auto result = newState->on_update(dt);
                if (!result.valid())
                {
                    sol::error err = result;
                    throw err;
                }
            }
            std::erase_if(m_MapStates, [](auto& state){ return state.second->killState; });
        }
        catch (const sol::error& e)
        {
            OTTERUS_ERROR("Failed to update state: {}", e.what());
            return;
        }
        catch (...)
        {
            OTTERUS_ERROR("Failed to update state: Error Unknown.");
        }

    }

    void StateMachine::Render()
    {
        try
        {   auto stateItr = m_MapStates.find(m_CurrentState);
            if (stateItr == m_MapStates.end())
                return;

            auto& newState = stateItr->second;

            if (newState->on_render.valid())
            {

                auto result = newState->on_render();
                if (!result.valid())
                {
                    sol::error err = result;
                    throw err;
                }
            }
        }
        catch (const sol::error& e)
        {
            OTTERUS_ERROR("Failed to invoke on_render state: {}", e.what());
            return;
        }
        catch (...)
        {
            OTTERUS_ERROR("Failed to invoke on_render state: Error Unknown.");
        }
    }

    void StateMachine::ExitState()
    {
        auto stateItr = m_MapStates.find(m_CurrentState);
        if (stateItr == m_MapStates.end())
        {
            OTTERUS_ERROR("Failed to remove state [{}], state does not exist.", m_CurrentState);
            return;
        }

        stateItr->second->on_exit();
        stateItr->second->killState = true;
        m_CurrentState.clear();
    }

    void StateMachine::DestroyStates()
    {
        for (auto& [name, state] : m_MapStates)
        {
            state->on_exit();
        }
        m_MapStates.clear();
    }

    void StateMachine::CreateLuaStateMachine(sol::state& lua)
    {
        lua.new_usertype<StateMachine>(
            "StateMachine",
            sol::call_constructor,
            sol::constructors<StateMachine(), StateMachine( const sol::table& )>(),
            "change_state",
            sol::overload(
                []( StateMachine& sm, const std::string& state, bool remove, const sol::object& enterParams ) {
                    sm.ChangeState( state, remove, enterParams );
                },
                []( StateMachine& sm, const std::string& state, bool remove ) { sm.ChangeState( state, remove ); },
                []( StateMachine& sm, const std::string& state ) { sm.ChangeState( state ); } ),
            "update",
            &StateMachine::Update,
            "render",
            &StateMachine::Render,
            "current_state",
            &StateMachine::CurrentState,
            "add_state",
            &StateMachine::AddState,
            "exit_state",
            &StateMachine::ExitState,
            "destroy",
            &StateMachine::DestroyStates
        );
    }
}
