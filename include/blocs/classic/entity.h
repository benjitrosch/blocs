#pragma once

#include <blocs/classic/component.h>

namespace blocs
{
    namespace classic
    {
        struct World;

        struct Entity
        {
            bool active = true;

            f32 x;
            f32 y;

            World* world = nullptr;
            Entity* next = nullptr;
            Entity* prev = nullptr;
            std::vector<Component*> components;

            template<typename T>
            T* add(T&& component = T());

            template<typename T>
            T* get()
            {
                for (auto& c : components)
                    if (c->type == Component::Types::id<T>())
                        return (T*)c;
                return nullptr;
            }
        };
    }
}
