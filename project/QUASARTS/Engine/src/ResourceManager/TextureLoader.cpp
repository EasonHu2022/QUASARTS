#include "TextureLoader.h"
#include "Logger/LogModule.h"

#include "stb_image.h"
namespace Engine
{
	bool TextureLoader::load(const std::string& filepath, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap)
	{
		

        auto textureResource = std::make_shared<TextureResource>();
		textureResource->data = stbi_load(filepath.data(), &textureResource->width, &textureResource->height, &textureResource->nrChannels, 0);
		resMap.emplace(handle, textureResource);
		return true;
	}
};