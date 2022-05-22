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
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emission;
        float emissiveRange;
        float shininess;

        // Don't bother with the pointer, just the path:
        std::string texturePath;
        inStream >> material.path
            >> ambient.r >> ambient.g >> ambient.b
            >> diffuse.r >> diffuse.g >> diffuse.b
            >> specular.r >> specular.g >> specular.b
            >> emission.r >> emission.g >> emission.b
            >> emissiveRange
            >> shininess >> texturePath;

        // Get default engine assets path:
        auto path = FileModule::Instance()->get_internal_assets_path();

        if (material.path == "None")
        {
            material.path = "";
        }
        else
        {
            material.path = path + material.path;
        }

        if (texturePath == "None")
        {
            texturePath = path + "Texture/white.png";
        }
        else
        {
            texturePath = path + texturePath;
        }

        std::string vshPath = path + "Shader/DefaultShader.vsh";
        std::string fshPath = path + "Shader/DefaultShader.fsh";
        std::string gshPth = "";
        material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);

        material.material->ambient = ambient;
        material.material->diffuse = diffuse;
        material.material->specular = specular;
        material.material->emission = emission;
        material.material->emissiveRange = emissiveRange;
        material.material->shininess = shininess;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const MaterialComponent &material) {
        // Don't bother with the pointer, just the path:
        if (material.path.empty())
        {
            outStream << "None";
        }
        else
        {
            auto path = FileModule::Instance()->get_internal_assets_path();
            std::size_t pathLength = path.size();
            std::string temp = (material.path).substr(pathLength);
            outStream << temp;
        }

        outStream << " "
            << material.material->ambient.r << " " << material.material->ambient.g << " " << material.material->ambient.b << " "
            << material.material->diffuse.r << " " << material.material->diffuse.g << " " << material.material->diffuse.b << " "
            << material.material->specular.r << " " << material.material->specular.g << " " << material.material->specular.b << " "
            << material.material->emission.r << " " << material.material->emission.g << " " << material.material->emission.b << " "
            << material.material->emissiveRange << " "
            << material.material->shininess << " ";

        if (material.material->texturePath.empty())
        {
            outStream << "None";
        }
        else
        {
            auto path = FileModule::Instance()->get_internal_assets_path();
            std::size_t pathLength = path.size();
            std::string temp = (material.material->texturePath).substr(pathLength);
            outStream << temp;
        }

        return outStream;
    }
}