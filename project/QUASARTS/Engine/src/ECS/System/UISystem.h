#pragma once
#include "Core/Core.h"
#include "Core/Application.h"
#include "Render/Shader.h"
#include "ECS/ECSManager.h"
#include "System.h"

namespace Engine
{
	class QS_API UISystem : public System
	{
        private:
        // Singleton:
        static UISystem* instance;
        UISystem();

        public:
        static UISystem* Instance();
        ~UISystem();

        int init(RenderContext *aRenderContext);
        int render();

        void init() {}
        int start() { return 0; }
        void update() {}
        int stop() { return 0; }
        void release() {}
        void initialize_components() override {}

        private:
        std::string vshPath;
        std::string fshPath;
        Shader* uiElementShader = NULL;

        RenderContext *renderContext;
	};
};
