#pragma once

// Library includes:

#include <string>

// Local includes:
#include "ECS/ECS-Common.h"
#include "Render/Material.h"
#include "ResourceManager/FileModule.h"

namespace Engine {
    struct QS_API MaterialComponent {
       
        std::string path = "";
        //for created in memory
        Material* material = NULL;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, MaterialComponent &material) {
        // Don't bother with the pointer, just the path:
        inStream >> material.path;

        // Get default engine assets path:
        auto path = FileModule::Instance()->get_internal_assets_path();
        std::string vshPath = path + "Shader/DefaultShader.vsh";
        std::string fshPath = path + "Shader/DefaultShader.fsh";
        std::string gshPth = "";
        std::string texturePath = path + "Texture/floor.jpg";
        material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const MaterialComponent &material) {
        // Don't bother with the pointer, just the path:
        outStream << material.path;
        
        return outStream;
    }
}