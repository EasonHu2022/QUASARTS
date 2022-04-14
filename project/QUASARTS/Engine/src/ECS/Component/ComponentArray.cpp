#include "ECS/Component/ComponentArray.h"

namespace Engine {
    void ComponentArray<TransformComponent>::print_component(unsigned int entityID) {
        TransformComponent transform = get_data(entityID);
        std::cout << "Component type: Transform" << std::endl;
        std::cout << "x: " << transform.x << std::endl;
        std::cout << "y: " << transform.y << std::endl;
        std::cout << "z: " << transform.z << std::endl;
        std::cout << "rotation: " << transform.rotation << std::endl;
        std::cout << "scale: " << transform.scale << std::endl;
    }

    void ComponentArray<MeshComponent>::print_component(unsigned int entityID) {
        MeshComponent mesh = get_data(entityID);
        std::cout << "Component type: Mesh" << std::endl;
        std::cout << "one: " << mesh.one << std::endl;
        std::cout << "two: " << mesh.two << std::endl;
        std::cout << "three: " << mesh.three << std::endl;
    }
}