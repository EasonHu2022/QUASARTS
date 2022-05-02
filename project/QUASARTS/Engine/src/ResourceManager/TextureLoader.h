#pragma once
#include "Core/Core.h"
#include "Core/ILoader.h"
#include "FileResource.h"
#include "TextureResource.h"
namespace Engine
{
	class QS_API TextureLoader : public ILoader
	{
	public:
		static constexpr const char* EXTENSIONS[] = {"jpg","png","tga","bmp","psd","gif","hdr","pic"};
		bool load(const std::string& pth, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap) override;

	};
}