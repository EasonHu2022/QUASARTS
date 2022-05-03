#pragma once
#include "Core/IResource.h"
namespace Engine
{
	class TextureResource :public IResource
	{
	public:
		TextureResource();
		~TextureResource();
		void reload() override {}
	public:
		int width;
		int height;
		int nrChannels;
		unsigned char* data;
	};

};
