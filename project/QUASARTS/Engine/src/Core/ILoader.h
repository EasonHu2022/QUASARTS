#pragma once

#include "Core/Core.h"
#include "Core/IResource.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace Engine
{
	class QS_API ILoader
	{
	public:
		virtual bool load(const std::string& filepath, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap) = 0;
	};
}