#pragma once
#include "Core/IModule.h"
#include "Core/Core.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "Core/Mesh.h"
namespace Engine
{

	class QS_API MeshLoader
	{
	public:
		virtual void load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>&) = 0;

	};

	class QS_API MeshLoaderFactory
	{
	public:
		MeshLoaderFactory();

		template<typename T>
		void add_loader();

		void load(const std::string& obj, std::unordered_map<std::string, std::shared_ptr<Mesh>>& meshes);

		inline auto& get_support_extension() const
		{
			return supportExtensions;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<MeshLoader>> loaders;

		std::unordered_set<std::string> supportExtensions;

	};

	template<typename T>
	inline void MeshLoaderFactory::add_loader()
	{
		auto loader = std::make_shared<T>();
		for (auto ext : T::EXTENSIONS)
		{
			loaders.emplace(ext, loader);
			supportExtensions.emplace(ext);
		}
	}



}
