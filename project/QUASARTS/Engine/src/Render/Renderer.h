#pragma once
#include "glfw3.h"
#include "glad.h"
#include "Core/Core.h"
#include "Render/RenderQueue.h"


//#include "GL.h"

namespace Engine
{
	class RenderContext
	{
	public:
		RenderContext() {};
		//view
		glm::mat4 view = glm::mat4(1.0f);
		//projection
		//later change a place because rarely update
		glm::mat4 projection = glm::mat4(1.0f);

		inline void set_view(glm::vec3 offset, glm::vec3 rotation)
		{

			view = glm::translate(view, offset);
			//x
			view = glm::rotate(view, rotation.x, glm::vec3(1, 0, 0));
			//y
			view = glm::rotate(view, rotation.y, glm::vec3(0, 1, 0));
			//z
			view = glm::rotate(view, rotation.z, glm::vec3(0, 0, 1));
		}

		inline void set_projection(float fov, float ratio, float nearClip, float farClip)
		{
			projection = glm::perspective(fov, ratio, nearClip, farClip);

		}

		inline auto get_projection_data()
		{
			auto data = glm::value_ptr(projection);
			return data;
		}

		inline auto get_view_data()
		{
			auto data = glm::value_ptr(view);
			return data;
		}

	};

	class QS_API Renderer
	{



	private:
		static Renderer* instance;

	public:
		static Renderer* Instance();
	public:
		unsigned int tbo;
		unsigned int fbo;
	public:
		RenderContext* context;
		RenderQueue* renderQueue;

	public:
		/// <summary>
		/// init the memoryModule
		/// </summary>
		//int init(int width, int height, char* title);
		int init();

		/// <summary>
		/// update
		/// </summary>
		int render();

		/// <summary>
		/// stop the Memory module
		/// </summary>
		/// <returns></returns>
		int stop();

		/// <summary>
		/// release the memory module
		/// </summary>
		void release() {};

	


		unsigned int get_rt();


	private:

		void Shader();
	};

};

