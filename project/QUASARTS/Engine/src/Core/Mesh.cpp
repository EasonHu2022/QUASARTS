#include "Mesh.h"
#include "Render/Renderer.h"
#pragma once
namespace Engine {






	Mesh::Mesh(const std::vector<uint32_t>& _indices, const std::vector<Vertex>& _vertices)
	{
		vertices = _vertices;
		indices = _indices;

		set_up_buffers();
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::generate_normals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		std::vector<glm::vec3> normals(vertices.size());

		if (!indices.empty())
		{
			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				const auto a = indices[i];
				const auto b = indices[i + 1];
				const auto c = indices[i + 2];
				const auto normal = glm::cross((vertices[b].pos - vertices[a].pos), (vertices[c].pos - vertices[a].pos));
				normals[a] += normal;
				normals[b] += normal;
				normals[c] += normal;
			}
		}
		else
		{
			for (uint32_t i = 0; i < vertices.size(); i += 3)
			{
				auto& a = vertices[i];
				auto& b = vertices[i + 1];
				auto& c = vertices[i + 2];
				const auto normal = glm::cross(b.pos - a.pos, c.pos - a.pos);
				normals[i] = normal;
				normals[i + 1] = normal;
				normals[i + 2] = normal;
			}
		}
		for (uint32_t i = 0; i < normals.size(); ++i)
		{
			vertices[i].normal = glm::normalize(normals[i]);
		}
	}

	void Mesh::generate_tangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		std::vector<glm::vec3> tangents(vertices.size());

		if (!indices.empty())
		{
			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				int a = indices[i];
				int b = indices[i + 1];
				int c = indices[i + 2];

				const auto tangent =
					generateTangent(vertices[a].pos, vertices[b].pos, vertices[c].pos, vertices[a].texCoord, vertices[b].texCoord, vertices[c].texCoord);

				tangents[a] += tangent;
				tangents[b] += tangent;
				tangents[c] += tangent;
			}
		}
		else
		{
			for (uint32_t i = 0; i < vertices.size(); i += 3)
			{
				const auto tangent =
					generateTangent(vertices[i].pos, vertices[i + 1].pos, vertices[i + 2].pos, vertices[i].texCoord, vertices[i + 1].texCoord, vertices[i + 2].texCoord);

				tangents[i] += tangent;
				tangents[i + 1] += tangent;
				tangents[i + 2] += tangent;
			}
		}
		for (uint32_t i = 0; i < vertices.size(); ++i)
		{
			vertices[i].tangent = glm::normalize(tangents[i]);
		}
	}

	auto Mesh::generateTangent(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec2& ta, const glm::vec2& tb, const glm::vec2& tc) -> glm::vec3
	{
		const glm::vec2 coord1 = tb - ta;
		const glm::vec2 coord2 = tc - ta;
		const glm::vec3 vertex1 = b - a;
		const glm::vec3 vertex2 = c - a;
		const glm::vec3 axis = (vertex1 * coord2.y - vertex2 * coord1.y);
		const float     factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);
		return axis * factor;
	}

	void Mesh::set_name(std::string _name)
	{
	}


	void Mesh::render(pack* p)
	{
		p->_VAO = VAO;
		p->size = indices.size();
	}

	void Mesh::set_up_buffers()
	{
		// Generate buffer objects and store handles.
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Bind vertex array and vertex buffer.
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Copy vertex data to the vertex buffer.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// Bind element buffer and copy vertex indices to it.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

		// Enable and copy vertex attributes:
		// "pos"
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// "color"
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// "texCoord"
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

		// "normal"
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// "tangent"
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));



		

		// Unbind.
		glBindVertexArray(0);
	}
}