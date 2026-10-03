#pragma once
#include <vector>
#include "State.h"

namespace otterus_core {
	class StateStack
	{
	private:
		std::vector<State> m_States{};
		std::unique_ptr<State> m_StateHolder{ nullptr };
 
	public:
		StateStack() = default;
		~StateStack()= default;

		void Push(State& state);
		void Pop();
		void ChangeState(State& state);

		void Update(const float dt);
		void Render();

		State& Top();

		static void CreateLuaStateStackBind(sol::state& lua);

	};

}