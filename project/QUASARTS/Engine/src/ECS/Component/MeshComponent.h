#pragma once

// Library includes:
#include <string>

// Local includes:
#include "ECS/ECS-Common.h"
#include "ResourceManager/FileModule.h"

namespace Engine {
    struct QS_API MeshComponent {
        float one;
        float two;
        float three;

        int resId = -1;
        std::string path;

    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, MeshComponent &mesh) {
        inStream >> mesh.resId >> mesh.path;

        if (mesh.path == "None")
        {
            mesh.path = "";
        }
        else
        {
            auto path = FileModule::Instance()->get_internal_assets_path();
            mesh.path = path + mesh.path;
        }

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const MeshComponent &mesh) {
        outStream << mesh.resId << " ";
        if (mesh.path.empty())
        {
            outStream << "None";
        }
        else
        {
            auto path = FileModule::Instance()->get_internal_assets_path();
            std::size_t pathLength = path.size();
            std::string temp = (mesh.path).substr(pathLength);
            outStream << temp;
        }
        
        return outStream;
    }
}