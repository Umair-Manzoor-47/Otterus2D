#pragma once
#include <vector>
#include <memory>

namespace otterus_editor {
	struct IDisplay
	{
		virtual ~IDisplay() = default;
		virtual void Draw() = 0;
		virtual void Update() {};
	};

	struct DisplayHolder
	{
		std::vector<std::unique_ptr<IDisplay>> displays;
	};

}