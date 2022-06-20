#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API LifeSpanComponent {
        float currentLifetime;
        float totalLifetime;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, LifeSpanComponent &life) {
        inStream >> life.currentLifetime >> life.totalLifetime;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const LifeSpanComponent &life) {
        outStream << life.currentLifetime << " " << life.totalLifetime;
        
        return outStream;
    }
}