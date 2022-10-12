#pragma once

#include <functional>

#include <blocs/platform/platform.h>
#include <blocs/ecs/entities/entity.h>

namespace blocs
{
    struct Time;
    namespace ecs
    {
        struct World;

        template<typename T, typename... Types>
        using Query =
            typename std::common_type<std::function<void(T&, Types&...)>>::type;
        template<typename T, typename... Types>
        using QueryWithEntity = typename std::common_type<
            std::function<void(Entity, T&, Types&...)>>::type;

        using SetupSystem    = std::function<void(World& world)>;
        using System         = std::function<void(World& world)>;
        using EventSystem    = std::function<void(World& world, Event event)>;
        using ShutdownSystem = std::function<void(World& world)>;
    }
}
