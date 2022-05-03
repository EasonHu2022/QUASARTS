#pragma once
#include "glfw3.h"
#include "glad.h"
#include "Core/Core.h"
#include "Render/UniformBufferObject.h"
#include "Render/RenderQueue.h"
#define MAX_LIGHTING_COUNT 5

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
			auto yaw = rotation.y;

			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			front = glm::normalize(front);
			// also re-calculate the Right and Up vector
			right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			up = glm::normalize(glm::cross(right, front));

			view = glm::lookAt(pos, pos + front, up);

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

		inline glm::mat4 get_projection_matrix()
		{
			return projection;
		}

		inline glm::mat4 get_view_matrix()
		{
			return view;
		}

	};

	struct alignas(16) lightBuffer
	{
		glm::vec4 ambient = { 0.2f,0.2f,0.2f,1.0f };
		glm::vec4 diffuse = { 0.2f,0.2f,0.2f,1.0f };
		glm::vec4 specular = { 0.2f,0.2f,0.2f,1.0f };
		glm::vec4 positon = { 0.0f,0.0f,0.0f ,1.0f };
		glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);
		float type = 0.0f;
	};


	struct alignas(16) LightingContext// aligned with std140
	{
		/*count of all of the lights in the scene, max = 10*/

		lightBuffer lights[MAX_LIGHTING_COUNT];
		float countLight = 0.0f;
	};

	class QS_API RenderContext
	{
	public :
		RenderContext();
		~RenderContext();

	public:
		void init();
		//push camera data to gpu
		void commit_camera_data();
		//push lighting data to gpu
		void commit_lighting_data();

	public:
		/*****************************commonData********************************/
		UniformBufferObject* matricesBuffer;
		UniformBufferObject* lightBuffer;
		CameraContext* cameraContext;
		LightingContext* lightingContext;
		RenderQueue* renderQueue;
		/*****************************commonData********************************/


		/***************************** for scene ********************************/
		//texture object for scene window
		unsigned int textureBufferObject;
		unsigned int get_renderTexture()
		{
			return textureBufferObject;
		}
		//frameBuffer object of scene window
		unsigned int frameBufferObject;

		//depth and stencil buffer of scene window
		unsigned int renderBufferObject;

		//rendertexture resolution
		const GLuint RT_WIDTH = 1024;
		const GLuint RT_HEIGHT = 1024;
		/***************************** for scene ********************************/


		/***************************** for shadow ********************************/
		//shadow depth resolution
		const GLuint SHADOW_WIDTH = 1024;
		const GLuint SHADOW_HEIGHT = 1024;
		

		//depth map buffer for shadow mapping
		unsigned int depthMapFBO[MAX_LIGHTING_COUNT];
		unsigned int depthTextureArray;
		/***************************** for shadow ********************************/
	};

};

