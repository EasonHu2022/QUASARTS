#include "ModelResource.h"
namespace Engine
{
	ModelResource::ModelResource()
	{
	}
	ModelResource::~ModelResource()
	{
	}
	void ModelResource::render()
	{
		for (auto m : meshes)
		{
			m.second->render();
		}
	}
};