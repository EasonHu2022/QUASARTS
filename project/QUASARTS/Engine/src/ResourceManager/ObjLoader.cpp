#include "ObjLoader.h"
#include "tiny_obj_loader.h"
#include "Logger/LogModule.h"
#define TINYOBJLOADER_IMPLEMENTATION
namespace Engine
{
	void ObjLoader::load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>& meshes)
	{
		std::string path = obj;
		auto directory = path.substr(0, path.find_last_of("\\"));
		std::string name = directory.substr(directory.find_last_of("\\") + 1);

		tinyobj::attrib_t                attrib;
		std::vector<tinyobj::shape_t>    shapes;
		std::vector<tinyobj::material_t> materials;
		std::string                      warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj.c_str(), directory.c_str()))
		{
			QWARN(warn);
			QERROR(err);
			return;
		}

		for (const auto& shape : shapes)
		{
			std::vector<Vertex>                  vertices;
			std::vector<uint32_t>                indices;
			std::unordered_map<Vertex, uint32_t> uniqueVertices{};

			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2] };
				
				if (index.normal_index >= 0)
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2] };

				if (index.texcoord_index >= 0)
					vertex.texCoord = glm::vec2{
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };

				vertex.color = { 1.0f, 1.0f, 1.0f, 1.f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.emplace_back(uniqueVertices[vertex]);

			}
			// if no normals, re-gen it
			if (attrib.normals.empty())
				Mesh::generate_normals(vertices, indices);

			Mesh::generate_tangents(vertices, indices);
			auto _mesh = std::make_shared<Mesh>(indices, vertices);
			_mesh->set_name(shape.name);
			meshes.emplace(shape.name, _mesh);
		}
	}

}

