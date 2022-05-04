#include "ObjLoader.h"
#include "tiny_obj_loader.h"
#include "Logger/LogModule.h"
#define TINYOBJLOADER_IMPLEMENTATION
namespace Engine
{
	bool ObjLoader::load(const std::string& filepath, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap)
	{
		std::string path = filepath;
		std::string directory = path.substr(0, path.find_last_of("/\\"));
		std::string name = path.substr(path.find_last_of("/\\") + 1);
		std::string modelName = name.substr(0, name.find_last_of("."));

		tinyobj::attrib_t                attrib;
		std::vector<tinyobj::shape_t>    shapes;
		std::vector<tinyobj::material_t> materials;
		std::string                      warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), directory.c_str()))
		{
			QWARN(warn);
			QERROR(err);
			QERROR("LoadObj params: path: {0}, directory: {1}", path.c_str(), directory.c_str());
			return false;
		}

		auto model = std::make_shared<ModelResource>();
		model->name = modelName;
		model->path = filepath;
		model->refs = 1;

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
			model->meshes.emplace(shape.name, _mesh);
		}

		resMap.emplace(handle, model);
		return true;
	}

}

