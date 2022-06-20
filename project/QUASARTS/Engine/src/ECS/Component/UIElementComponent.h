#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API UIElementComponent {
        std::string uiTexPath = "Texture/UI/Crosshair.png";
        Texture2D *uiElementTexture = nullptr;
        bool uiTransparency = true;
        int priority = 0; // Value from 0 to 5 - 5 being the highest so will be rendered last.

        ~UIElementComponent()
        {
            if (uiElementTexture) { delete uiElementTexture; }
        }
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, UIElementComponent &uiElement) {
        inStream >> uiElement.uiTexPath;

        auto path = FileModule::Instance()->get_internal_assets_path();

        if (uiElement.uiTexPath == "None")
        {
            uiElement.uiTexPath = path + "Texture/UI/Crosshair.png";
            uiElement.uiElementTexture = new Texture2D(uiElement.uiTexPath);
        }
        else
        {
            uiElement.uiTexPath = path + uiElement.uiTexPath;
            uiElement.uiElementTexture = new Texture2D(uiElement.uiTexPath);
        }

        int transparency;
        inStream >> transparency;
        if (transparency == 1) { uiElement.uiTransparency = true; }
        else { uiElement.uiTransparency = false; }

        inStream >> uiElement.priority;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const UIElementComponent &uiElement) {
        if (uiElement.uiTexPath.empty())
        {
            outStream << "None ";
        }
        else
        {
            auto path = FileModule::Instance()->get_internal_assets_path();
            std::size_t pathLength = path.size();
            std::string temp = (uiElement.uiTexPath).substr(pathLength);
            outStream << temp << " ";
        }

        if (uiElement.uiTransparency) { outStream << 1 << " "; }
        else { outStream << 0 << " "; }

        outStream << uiElement.priority;

        return outStream;
    }
}