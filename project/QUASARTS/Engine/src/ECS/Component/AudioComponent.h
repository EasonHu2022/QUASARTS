#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "Audio/ClipSource.h"

#include <string>

namespace Engine {

    struct QS_API AudioComponent
    {
        ALuint audio_src;
        std::string sound_path;
    };

    // Input stream operator:
    inline std::istream& operator >> (std::istream& inStream, AudioComponent& audio) {
        // Use the name and path, nothing else:
        inStream >> audio.sound_path;

        return inStream;
    }

    // Output stream operator:
    inline std::ostream& operator << (std::ostream& outStream, const AudioComponent& audio) {
        // Use the name and path, nothing else:
        outStream << audio.sound_path;

        return outStream;
    }
}