#include "ModelResource.h"

namespace Engine
{
	ModelResource::ModelResource()
	{
	}
	ModelResource::~ModelResource()
	{
	}
	void ModelResource::render(pack* p)
	{
		for (auto m : meshes)
		{
			m.second->render(p);
		}
	}
};