#pragma once
#include "Core/Core.h"
#include "Core/ILoader.h"
#include "FileResource.h"
namespace Engine
{
	class QS_API FileLoader : public ILoader
	{
	public:
		static constexpr const char* EXTENSIONS[] = { "vsh", "fsh", "gsh", "Lua" };
		bool load(const std::string& filepath, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap) override;

	};
}