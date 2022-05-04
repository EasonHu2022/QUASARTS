#include "AttributeVector.h"
namespace Engine
{
	AttributeVector::AttributeVector()
	{
		glm::vec3 to_add;
		glm::vec3 to_add2;
		attributes.push_back(to_add);
		attributes.push_back(to_add2);
	}
	AttributeVector::~AttributeVector()
	{
	}
};