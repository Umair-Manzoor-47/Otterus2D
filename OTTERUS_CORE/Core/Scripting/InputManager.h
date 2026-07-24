#pragma once
#include <memory>
#include <sol/sol.hpp>
#include <Windowing/Inputs/Keyboard.h>
#include <Windowing/Inputs/Mouse.h>

using namespace otterus_windowing::Inputs;

namespace otterus_core {
	class InputManager
	{
	private:
		std::unique_ptr<Keyboard> m_Keyboard;
		std::unique_ptr<Mouse> m_Mouse;

	private:
		InputManager();
		~InputManager() = default;

		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

	private:
		static void RegisterLuaKeyNames(sol::state& lua);
		static void RegisterLuaBtnNames(sol::state& lua);

	public:
		static InputManager& GetInstance();
		static void CreateLuaBindings(sol::state& lua);

		inline Keyboard& GetKeyboard() { return *m_Keyboard; }
		inline Mouse& GetMouse() { return *m_Mouse; }
	};

}