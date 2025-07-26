#include "ECS.hpp"
#include "../Logger/Logger.hpp"

int IComponent::nextId = 0;

int Entity::GetId() const { return id; }

void System::AddEntityToSystem(Entity entity) { entities.push_back(entity); }

void System::RemoveEntityFromSystem(Entity entity) {
    std::erase_if(entities, [&entity](Entity other) {
        return entity == other;
    });
}

std::vector<Entity> System::GetSystemEntities() const { return entities; }

const Signature& System::GetComponentSignature() const { return componentSignature; }

Entity Registry::CreateEntity() {
    int entityId = numEntities++;
    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with ID = " + std::to_string(entityId));
    return entity;
}

void Registry::AddEntityToSystem(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    for (auto &system: systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
        if (isInterested) system.second->AddEntityToSystem(entity);
    }
}

void Registry::Update() {

}
