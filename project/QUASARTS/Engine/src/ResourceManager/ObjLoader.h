#pragma once
#include "Core/Core.h"
#include "Core/ILoader.h"
#include "ModelResource.h"
namespace Engine
{

	class ObjLoader : public ILoader
	{
	public:
		static constexpr char* EXTENSIONS[] = { "obj" };
		bool load(const std::string& filepath, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap) override;

	};

}



