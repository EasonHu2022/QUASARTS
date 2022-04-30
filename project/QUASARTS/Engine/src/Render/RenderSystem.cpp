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
		
		update_projection();
		update_light();
	}

	void RenderSystem::update()
	{
		update_projection();
		update_light();
		// Get the manager:
		ECSManager* active_manager = get_manager();
		// Get the entity ID mask:
		quasarts_entity_ID_mask* entitiesRenderable = get_entity_ID_mask(0);
		

		//pass data to renderQueue

		TransformComponent *transform;
		MeshComponent *mesh;
		MaterialComponent *material;
		
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

					ResourceManager::Instance()->load_resource(mesh->path, &resId))
				{
					auto model = ResourceManager::Instance()->get_resource<ModelResource>(resId);
					model->render(p);
				}
				p->set_model(transform->position, transform->rotation, transform->scale);
				//get mat resource from material component

				if (material->material == NULL)
				{
					if (size_t resId;

						ResourceManager::Instance()->load_resource(material->path, &resId))
					{
						auto mat = ResourceManager::Instance()->get_resource<Material>(resId);

						material->material = mat.get();
						p->shader_program = mat->shader;
					}
				}
				else
				{

					p->shader_program = material->material->shader;
				}
				//push p into renderQueue
				Renderer::Instance()->renderQueue->push(p);
			}
		}

	


	}

	void RenderSystem::update_projection()
	{
		auto cameraID = Engine::ECSManager::Instance()->get_camera();		if (cameraID == TOO_MANY_ENTITIES)
			return;
		// Get the manager:
		ECSManager* active_manager = get_manager();

		TransformComponent *transform;
		CameraComponent *camera;
	
		camera = active_manager->get_component
			<CameraComponent>(cameraID, COMPONENT_CAMERA);
		transform = active_manager->get_component
			<TransformComponent>(cameraID, COMPONENT_TRANSFORM);

		Renderer::Instance()->cameraContext->set_view(transform->position, transform->rotation);
		Renderer::Instance()->cameraContext->set_projection(camera->fov, camera->ratio, camera->nearClip, camera->farClip);
		
	}

	void RenderSystem::update_light()
	{
		// Get the manager:
		ECSManager* active_manager = get_manager();

		TransformComponent *transform;
		LightComponent *light;
		quasarts_entity_ID_mask* lightingSources = get_entity_ID_mask(1);

		//later test if can release

		auto info = Renderer::Instance()->lightingContext;
		int ind = 0;
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
				
		
				info->lights[ind].type = (float) light->type;
				info->lights[ind].ambient = glm::vec4(light->ambient,1.0f);
				info->lights[ind].diffuse = glm::vec4(light->diffuse,1.0f);
				info->lights[ind].specular = glm::vec4(light->specular,1.0f);
				info->lights[ind].positon = glm::vec4(transform->position,1.0f);

				/******************calculate light matrix*****************************/
				auto lightPos = transform->position;
				glm::mat4 lightProjection;
				glm::mat4 lightView;
				//temp
				//these param should be in the light component
				if (light->type == LightType::parallel)//parallel light
					lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
				else
					lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
					//lightProjection = glm::perspective(glm::radians(85.0f), 1.0f, 0.1f, 100.0f);
				lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				info->lights[ind].lightSpaceMatrix = lightProjection * lightView;
				/******************calculate light matrix*****************************/



				ind++;
				info->countLight = ind;
			}
		}
		
	}


};
