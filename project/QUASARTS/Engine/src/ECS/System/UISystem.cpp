#include "UISystem.h"

namespace Engine
{
    // Singleton:
    UISystem* UISystem::instance = nullptr;

    // Create the instance of the CombatSystem:
    UISystem* UISystem::Instance() {
        if (nullptr == instance)
            instance = new UISystem();
        return instance;
    }

	UISystem::UISystem()
	{
        // Set a Component mask for UI elements:
        quasarts_component_mask mask;
        mask.mask = 0;
        mask.mask += (uint64_t)1 << COMPONENT_UI_ELEMENT;
        add_component_mask(mask);
	}

	int UISystem::init(RenderContext *aRenderContext)
	{
		renderContext = aRenderContext;

		// Get internal engine assets path:
		std::string path = FileModule::Instance()->get_internal_assets_path();

        // Define default paths:
		vshPath = path + "Shader/UIElement.vsh";
		fshPath = path + "Shader/UIElement.fsh";

		uiElementShader = new Shader(vshPath.data(), fshPath.data());

		return 0;
	}

	int UISystem::render()
	{
        if (uiElementShader == NULL) { return 0; }

		glBindFramebuffer(GL_FRAMEBUFFER, renderContext->finalBufferObject);
		glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

		uiElementShader->use();

		ECSManager *active_manager = get_manager();
		quasarts_entity_ID_mask *uiMask = get_entity_ID_mask(0);

		UIElementComponent *uiElement;

        // Draw UI elements in order:
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < MAX_ENTITIES; j++)
			{
				if (uiMask->mask[j] == 1)
				{
					uiElement = active_manager->get_component<UIElementComponent>(j, COMPONENT_UI_ELEMENT);
					if (uiElement->priority != i) { continue; }

					if (uiElement->uiTransparency)
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					}
					else
					{
						glDisable(GL_BLEND);
					}

					GLuint emptyVAO;
					glGenVertexArrays(1, &emptyVAO);
					glBindVertexArray(emptyVAO);

					uiElement->uiElementTexture->use(GL_TEXTURE0);
					GLint iTextureUniform = glGetUniformLocation(uiElementShader->ID, "uiElementTexture");
					glUniform1i(iTextureUniform, 0);

					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return 0;
	}
};