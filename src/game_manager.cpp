#include "game_manager.hpp"

static void register_system(std::unordered_map<std::size_t, void *> &systems, void *system)
{
    const std::size_t typeId = std::type_index(typeid(system)).hash_code();
    systems[typeId] = system;
}

GameManager::GameManager()
{
    register_system(systems_, new CameraSystem());
}

GameManager::~GameManager()
{
    for (auto &[_, system] : systems_)
        delete system;
}
