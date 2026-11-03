#pragma once
#include "Rendering/Buffers/Framebuffer.h"
#include <map>

namespace otterus_editor
{
	enum class FramebufferType
	{
		TILEMAP, SCENE, NO_TYPE
	};

	struct EditorFramebuffers
	{
		std::map<FramebufferType, std::shared_ptr<otterus_rendering::Framebuffer>> mapFramebuffers;
	};
}