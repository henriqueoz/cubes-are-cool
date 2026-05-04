#ifndef SYSTEM_MANAGER_H_
#define SYSTEM_MANAGER_H_

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "isystem.hpp"

using SystemMap = std::unordered_map<std::size_t, std::unique_ptr<ISystem>>;

class SystemManager
{
  public:
    SystemManager() = default;
    ~SystemManager() = default;

    inline void add(ISystem *system)
    {
        const std::size_t typeId = std::type_index(typeid(system)).hash_code();

        if (systems_.find(typeId) != systems_.end())
        {
            return;
        }

        systems_[typeId] = std::unique_ptr<ISystem>(system);
    }

    template <typename T> inline T *system()
    {
        const std::size_t typeId = std::type_index(typeid(T)).hash_code();
        const auto it = systems_.find(typeId);

        if (it == systems_.end())
            return nullptr;

        return static_cast<T *>(it->second);
    }

  private:
    SystemMap systems_;
};

#endif // SYSTEM_MANAGER_H_