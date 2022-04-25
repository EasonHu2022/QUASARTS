#pragma once
#include "glfw3.h"
#include "glad.h"
#include "Core/Core.h"
#include "Render/RenderQueue.h"


//#include "GL.h"

namespace Engine
{
	class CameraContext
	{
	private:
		//view
		glm::mat4 view;
		//projection
		//later change a place because rarely update
		glm::mat4 projection;
		
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;

	public:
		glm::vec3 pos = { 0.0f,0.0f,0.0f };
		CameraContext() {
			view = glm::mat4(1.0f);
			projection = glm::mat4(1.0f);
			up = { 0.0f,1.0f,0.0f };
			front = { 0.0f,0.0f,-1.0f };
			worldUp = up;
		};
	

		inline void set_view(glm::vec3 offset, glm::vec3 rotation)
		{
			view = glm::mat4(1.0f);
			pos = offset;
			auto pitch = rotation.x;
			auto yaw =  rotation.y;
			//world up
			auto up = glm::vec3(0.0f, 1.0f, 0.0f);

			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			front = glm::normalize(front);
			// also re-calculate the Right and Up vector
			right = glm::normalize(glm::cross(front, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			up = glm::normalize(glm::cross(right, front));
			
			view = glm::lookAt(pos,pos+front,up);

		}

		inline void set_projection(float fov, float ratio, float nearClip, float farClip)
		{
			projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(fov), ratio, nearClip, farClip);

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
		CameraContext* context;
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

