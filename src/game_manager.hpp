#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <typeindex>
#include <unordered_map>

#include "camera_system.hpp"

class GameManager
{
  public:
    GameManager();
    ~GameManager();

  private:
    std::unordered_map<std::size_t, void *> systems_ = {};

    template <typename T> inline T *system()
    {
        const std::size_t typeId = std::type_index(typeid(T)).hash_code();

        return static_cast<T *>(systems_[typeId]);
    }
};

#endif // GAME_MANAGER_H_