#include "RenderSystem.h"
#include "Core/Application.h"
#include "Render/Renderer.h"
#include "ResourceManager/ResourceManager.h"
#include "ECS/ECSManager.h"	
namespace Engine
{
	RenderSystem::RenderSystem()
	{
		// Set the Component mask 0 :
		quasarts_component_mask mask;
		mask.mask = 0;
		mask.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
		mask.mask += (uint64_t)1 << COMPONENT_MESH;
		mask.mask += (uint64_t)1 << COMPONENT_MATERIAL;
		// Add the Renderable mask to the System:
		add_component_mask(mask);

		quasarts_component_mask mask1;
		// Set the Component mask 1 :
		mask1.mask = 0;
		mask1.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
		mask1.mask += (uint64_t)1 << COMPONENT_LIGHTING;
		// Add the Renderable mask to the System:
		add_component_mask(mask1);

		quasarts_component_mask mask2;
		// Set the Component mask 2 :
		mask2.mask = 0;
		mask2.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
		mask2.mask += (uint64_t)1 << COMPONENT_CAMERA;
		// Add the Renderable mask to the System:
		add_component_mask(mask2);
	
	}


	void RenderSystem::init()
	{
		//initialize databuffers
		matricesBuffer = new UniformBufferObject(UniformBlockIndex::matrices, 2 * sizeof(glm::mat4));
		//carefully allocate memory for light buffer

		lightBuffer = new UniformBufferObject(UniformBlockIndex::light, sizeof(Lightinfo));

		matricesBuffer->init();
		lightBuffer->init();
		update_camera();
		update_light();
	}

	void RenderSystem::update()
	{
		
		// Get the manager:
		ECSManager* active_manager = get_manager();
		// Get the entity ID mask:
		quasarts_entity_ID_mask* entitiesRenderable = get_entity_ID_mask(0);
		

		//pass data to renderQueue
		TransformComponent transform;
		MeshComponent mesh;
		MaterialComponent material;
		
		

		

		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (entitiesRenderable->mask[i] == 1)
			{
				transform = active_manager->get_component
					<TransformComponent>(i, COMPONENT_TRANSFORM);
				mesh = active_manager->get_component
					<MeshComponent>(i, COMPONENT_MESH);
				material = active_manager->get_component
					 <MaterialComponent>(i, COMPONENT_MATERIAL);


				pack* p = new pack();
				//get mesh resource from component
				/*************temp****************/
				if (size_t resId;
					ResourceManager::Instance()->load_resource(mesh.path, &resId))
				{
					auto model = ResourceManager::Instance()->get_resource<ModelResource>(resId);
					model->render(p);
				}

				p->set_model(transform.position, transform.rotation, transform.scale);

				

				//get mat resource from material component(todo)
				if (material.material == NULL)
				{
					if (size_t resId;
						ResourceManager::Instance()->load_resource(material.path, &resId))
					{
						auto mat = ResourceManager::Instance()->get_resource<Material>(resId);
						p->shader_program = mat->shader;
					}
				}
				else
				{
					p->shader_program = material.material->shader;
				}
				
				

				//push p into renderQueue
				Renderer::Instance()->renderQueue->push(p);
			}
		}

	


	}

	void RenderSystem::update_camera()
	{
		// Get the manager:
		ECSManager* active_manager = get_manager();
		TransformComponent transform;
		CameraComponent camera;
		
		quasarts_entity_ID_mask* cameraEnt = get_entity_ID_mask(2);
		//set camera
		int cameras = 0;
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (cameras != 0)
			{
				cameras++;
				break;
			}			
			if (cameraEnt->mask[i] == 1)
			{
				camera = active_manager->get_component
					<CameraComponent>(i, COMPONENT_CAMERA);
				transform = active_manager->get_component
					<TransformComponent>(i, COMPONENT_TRANSFORM);
				Renderer::Instance()->context->set_view(transform.position, transform.rotation);
				Renderer::Instance()->context->set_projection(camera.fov, camera.ratio, camera.nearClip, camera.farClip);
				matricesBuffer->set_data(0,sizeof(glm::mat4), Renderer::Instance()->context->get_projection_data());
				matricesBuffer->set_data(sizeof(glm::mat4), sizeof(glm::mat4), Renderer::Instance()->context->get_view_data());

				cameras++;
			}
		}
		if (cameras != 1)
		{
			QERROR("can't support no camera or multi camera, use the first camera");
		}
	}

	void RenderSystem::update_light()
	{
		// Get the manager:
		ECSManager* active_manager = get_manager();
		TransformComponent transform;
		LightComponent light;
		quasarts_entity_ID_mask* lightingSources = get_entity_ID_mask(1);

		//later test if can release
		Lightinfo* info = new Lightinfo();
		//set the light resource of this frame
		for (int i = 0; i < MAX_ENTITIES; i++)
		{

			if (lightingSources->mask[i] == 1)
			{
				//set light resource to renderer
				transform = active_manager->get_component
					<TransformComponent>(i, COMPONENT_TRANSFORM);
				light = active_manager->get_component
					<LightComponent>(i, COMPONENT_LIGHTING);
				
				info->lights[info->countLight].type = light.type;
				info->lights[info->countLight].ambient = light.ambient;
				info->lights[info->countLight].diffuse = light.diffuse;
				info->lights[info->countLight].specular = light.specular;
				info->countLight++;				
			}
		}
		lightBuffer->set_data(0,sizeof(Lightinfo),info);
	}


};
