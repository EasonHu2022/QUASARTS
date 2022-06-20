#include "RenderContext.h"
namespace Engine
{
	RenderContext::RenderContext()
	{
		renderQueue = new RenderQueue();
		cameraContext = new CameraContext();
		lightingContext = new LightingContext();

		//initialize databuffers
		matricesBuffer = new UniformBufferObject(UniformBlockIndex::matrices, 2 * sizeof(glm::mat4));
		//carefully allocate memory for light buffer
		lightBuffer = new UniformBufferObject(UniformBlockIndex::light, sizeof(LightingContext));
	}
	RenderContext::~RenderContext()
	{
		delete renderQueue;
		delete cameraContext;
		delete lightingContext;
		delete matricesBuffer;
		delete lightBuffer;
	}
	void RenderContext::init()
	{
		matricesBuffer->init();
		lightBuffer->init();

		/*
		
		gen and bind buffers
		
		*/
		/*****************RenderTexture************************/
		glGenFramebuffers(1, &finalBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, finalBufferObject);

		glGenTextures(1, &textureBufferObject);
		glBindTexture(GL_TEXTURE_2D, textureBufferObject);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RT_WIDTH, RT_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBufferObject, 0);

		glGenRenderbuffers(1, &renderBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RT_WIDTH, RT_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject); // now actually attach it

		/**********for gaussian blur************/
		glGenFramebuffers(2, postBufferObject);
		glGenTextures(2, screenTexture);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, postBufferObject[i]);
			glBindTexture(GL_TEXTURE_2D, screenTexture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RT_WIDTH, RT_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture[i], 0);
		}


		glGenFramebuffers(1, &frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RT_WIDTH, RT_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

		glGenTextures(1, &emissionTeture);
		glBindTexture(GL_TEXTURE_2D, emissionTeture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RT_WIDTH, RT_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, emissionTeture, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject); // now actually attach it
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/*****************RenderTexture************************/

		/*****************ShadowMapping************************/
		//for many lights
		glEnable(GL_TEXTURE_2D_ARRAY);
		glGenTextures(1, &depthTextureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, depthTextureArray);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, MAX_LIGHTING_COUNT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		for (int i = 0; i < MAX_LIGHTING_COUNT; i++)
		{
			glGenFramebuffers(1, &depthMapFBO[i]);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureArray, 0, i);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	}
	void RenderContext::commit_camera_data()
	{
		matricesBuffer->set_data(0, sizeof(glm::mat4), cameraContext->get_projection_data());
		matricesBuffer->set_data(sizeof(glm::mat4), sizeof(glm::mat4), cameraContext->get_view_data());
	}
	void RenderContext::commit_lighting_data()
	{
		lightBuffer->set_data(0, sizeof(LightingContext), lightingContext);
	}
};