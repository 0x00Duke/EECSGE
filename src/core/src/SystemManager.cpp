#include "core/include/SystemManager.hpp"

template <typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

    std::shared_ptr<T> system = std::make_shared<T>();
    mSystems.insert({typeName, system});
    return system;
}

template <typename T>
void SystemManager::SetSignature(Signature signature)
{
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

    mSignatures.insert({typeName, signature});
}

void SystemManager::EntityDestroyed(Entity entity)
{
    for (auto const &pair : mSystems)
    {
        auto const &system = pair.second;

        system->mEntities.erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
    for (auto const &pair : mSystems)
    {
        auto const &type = pair.first;
        auto const &system = pair.second;
        auto const &systemSignature = mSignatures[type];

        if ((entitySignature & systemSignature) == systemSignature)
        {
            system->mEntities.insert(entity);
        }
        else
        {
            system->mEntities.erase(entity);
        }
    }
}