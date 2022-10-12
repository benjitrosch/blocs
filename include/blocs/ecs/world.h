#pragma once

namespace blocs
{
    namespace ecs
    {
        constexpr u16 MAX_ENTITIES   = 65535;
        constexpr u8  MAX_COMPONENTS = 255;
    }
}

#include <blocs/time.h>
#include <blocs/ecs/entities/entitymanager.h>
#include <blocs/ecs/components/componentmanager.h>
#include <blocs/ecs/systems/systemmanager.h>
#include <blocs/ecs/resources/resourcemanager.h>

namespace blocs
{
    namespace ecs
    {
        /**
         * @brief Maintains the Entity, Component, System, and Resource managers
         * and provides duplicate API's for common manager methods to allow
         * quick access.
         */
        struct World
        {
            EntityManager    entities;
            ComponentManager components;
            SystemManager    systems;
            ResourceManager  resources;

            /**
             * @brief Creates a new entity from the available pool of id's and
             * adds to game. Will become available in System component queries
             * next frame.
             *
             * @return Reference to entity.
             */
            Entity spawn_entity(const str& name)
            {
                return entities.create(name);
            }

            Entity spawn_entity() { return entities.create(); }

            template<typename T, typename... Args>
            T& add_component(Entity entity, Args&&... args)
            {
                return components.add<T>(entity, T(args...));
            }

            template<typename T, typename... Args>
            void remove_component(Entity entity, Args&&... args)
            {
                components.remove<T>(entity);
            }

            void destroy_entity(Entity entity)
            {
                entities.remove(entity);
                components.remove(entity);
            }

            /**
             * @brief Returns number of active components for the queried
             * component.
             *
             * @tparam T component to count.
             *
             * @return Number of active components of type T.
             */
            template<typename T>
            size query_count()
            {
                auto* component_array = components.get_components<T>();
                return component_array->set.len;
            }

            template<typename T>
            size query_count(std::function<bool(T&)> predicate)
            {
                size  n               = 0;
                auto* component_array = components.get_components<T>();
                for (auto e = component_array->set.end();
                     e-- != component_array->set.begin();)
                {
                    auto entity    = *e;
                    auto component = component_array->get(entity);
                    if (predicate(component))
                    {
                        n++;
                    }
                }

                return n;
            }

            /**
             * @brief Runs a lambda expression on every entity with components
             * specified in the type parameters. The first type parameter
             * specifies the "primary" component being queried whose component
             * array is iterated over.
             *
             * @tparam Types the components to query for and use as params in
             * the lambda.
             * @param func Logic called on every entity that matches the query.
             */
            template<typename T, typename... Types>
            constexpr void query(Query<T, Types...> func)
            {
                auto* component_array = components.get_components<T>();
                for (auto e = component_array->set.end();
                     e-- != component_array->set.begin();)
                {
                    auto  entity    = *e;
                    auto& component = component_array->get(entity);

                    constexpr i64 size = sizeof...(Types);
                    if (size == 0 || components.has<Types...>(entity))
                    {
                        func(component, components.get<Types>(entity)...);
                    }
                }
            }

            template<typename T, typename... Types>
            constexpr void query(QueryWithEntity<T, Types...> func)
            {
                auto* component_array = components.get_components<T>();
                for (auto e = component_array->set.end();
                     e-- != component_array->set.begin();)
                {
                    auto  entity    = *e;
                    auto& component = component_array->get(entity);

                    constexpr i64 size = sizeof...(Types);
                    if (size == 0 || components.has<Types...>(entity))
                    {
                        func(
                            entity, component, components.get<Types>(entity)...
                        );
                    }
                }
            }

            template<typename T>
            constexpr T& query_singleton()
            {
                ComponentArray<T>* componentArray =
                    components.get_components<T>();
                return componentArray->get_singleton();
            }

            template<typename T, typename... Types>
            struct QueryResult
            {
                using item = std::tuple<Entity, T&, Types&...>;
                World* world;

                struct Iterator
                {
                    World*             world;
                    ComponentArray<T>* component_array;

                    u16 index;

                    item operator*()
                    {
                        auto  entity = *(component_array->set.begin() + index);
                        auto& component = component_array->get(entity);

                        return {
                            entity, component,
                            world->components.get<Types>(entity)...};
                    }

                    Iterator& operator++()
                    {
                        ++index;

                        if constexpr (sizeof...(Types) > 0)
                        {
                            auto entity =
                                *(component_array->set.begin() + index);
                            while (index < component_array->set.len &&
                                   !world->components.has<Types...>(entity))
                            {
                                ++index;
                            }
                        }

                        return *this;
                    }

                    Iterator operator++(i32)
                    {
                        Iterator temp = *this;
                        ++*this;
                        return temp;
                    }

                    Iterator& operator--()
                    {
                        --index;

                        if constexpr (sizeof...(Types) > 0)
                        {
                            auto entity =
                                *(component_array->set.begin() + index);
                            while (index > 0 &&
                                   !world->components.has<Types...>(entity))
                            {
                                --index;
                            }
                        }

                        return *this;
                    }

                    Iterator operator--(i32)
                    {
                        Iterator temp = *this;
                        --*this;
                        return temp;
                    }

                    Iterator operator+(const i32 val) const
                    {
                        return {world, component_array, index + val};
                    }

                    Iterator operator-(const i32 val) const
                    {
                        return {world, component_array, index - val};
                    }

                    bool operator==(const Iterator& i) const
                    {
                        return index == i.index;
                    }

                    bool operator!=(const Iterator& i) const
                    {
                        return index != i.index;
                    }
                };

                Iterator begin() const
                {
                    return {world, world->components.get_components<T>(), 0U};
                }

                Iterator end() const
                {
                    return {
                        world, world->components.get_components<T>(),
                        (u16)world->components.get_components<T>()->set.len};
                }

                size size() const
                {
                    return world->components.get_components<T>()->set.len;
                }

                item operator[](i32 index)
                {
                    auto entity =
                        *(world->components.get_components<T>()->set.begin() +
                          index);
                    auto& component =
                        world->components.get_components<T>()->get(entity);

                    return {
                        entity, component,
                        world->components.get<Types>(entity)...};
                }
            };

            template<typename T, typename... Types>
            constexpr QueryResult<T, Types...> iter()
            {
                return {this};
            }

            void start()
            {
                for (auto system : systems.setup_systems) system(*this);
            }

            /**
             * @brief Calls `reset` on the Entity and Component manager.
             */
            void reset()
            {
                entities.reset();
                components.reset();
            }

            void events(Event& event)
            {
                for (auto system : systems.event_systems) system(*this, event);
            }

            void update()
            {
                for (auto system : systems.systems[Stage::EARLY_UPDATE])
                    system(*this);
                for (auto system : systems.systems[Stage::UPDATE])
                    system(*this);
                for (auto system : systems.systems[Stage::LATE_UPDATE])
                    system(*this);
            }

            void render()
            {
                for (auto system : systems.systems[Stage::DRAW]) system(*this);

                for (auto system : systems.systems[Stage::UI]) system(*this);
            }
        };
    }
}
