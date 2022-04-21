#pragma once
#include "Core/Core.h"
#include "IResource.h"
#include <vector>
#include "Core/Vertex.h"
#include <string>
#include "glad/glad.h"
#include "Render/RenderQueue.h"
namespace Engine
{
	class QS_API Mesh : public IResource
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices);
		~Mesh();
		static void generate_normals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		static void generate_tangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void set_name(std::string _name);
	public:
		static glm::vec3 generateTangent(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec2& ta, const glm::vec2& tb, const glm::vec2& tc);
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::string name;

	public:
		void render(pack* p);
		unsigned int VAO, VBO, EBO;
		void set_up_buffers();

	public:
		void reload() override {}; // From IResource
	};
}

