#include "StateStack.h"
#include <Logger/Logger.h>

namespace otterus_core {
	void StateStack::Push(State& state)
	{
		auto hasState = std::find_if(m_States.begin(), m_States.end(), 
			[&](const auto& s) {return (state.name == s.name);}
		);
		if (hasState == m_States.end())
		{
			state.addState = true;
			m_StateHolder = std::make_unique<State>(state);
			return;
		}
		OTTERUS_ASSERT(false && "Trying to add state which is already in stack.");

	}
	void StateStack::Pop()
	{
		if (m_States.empty())
		{
			OTTERUS_ERROR("Trying to pop a stack which is empty.");
			return;
		}
		auto& top = m_States.back();
		top.killState = true;
	}
	void StateStack::ChangeState(State& state)
	{
		if (!m_States.empty())
			Pop();
		Push(state);
	}
	void StateStack::Update(const float dt)
	{
	}
	void StateStack::Render()
	{
	}
	State& StateStack::Top()
	{
		OTTERUS_ASSERT(!m_States.empty() && "Cannot get the top of an empty stack");

		if (m_States.empty())
			throw std::runtime_error("State stack is empty!");

		return m_States.back();
	}
	void StateStack::CreateLuaStateStackBind(sol::state& lua)
	{
	}
}