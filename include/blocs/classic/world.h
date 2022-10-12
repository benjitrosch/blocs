#pragma once

namespace blocs
{
	namespace classic
	{
		constexpr u16 MAX_ENTITIES 		= 65535;
		constexpr u8  MAX_COMPONENTS 	= 255;
	}
}

#include <blocs/time.h>
#include <blocs/classic/component.h>
#include <blocs/classic/entity.h>

namespace blocs
{
    namespace classic
    {
        struct World
        {
        private:
            template<typename T>
            struct Pool
            {
                T* first = nullptr;
                T* last = nullptr;

                void insert(T* instance)
                {
                    if (last)
                    {
                        last->next = instance;
                        instance->prev = last;
                        instance->next = nullptr;
                        last = instance;
                    }
                    else
                    {
                        first = last = instance;
                        instance->prev = instance->next = nullptr;
                    }
                }

                void remove(T* instance)
                {
                    if (instance->prev)
                        instance->prev->next = instance->next;
                    if (instance->next)
                        instance->next->prev = instance->prev;

                    if (first == instance)
                        first = instance->next;
                    if (last == instance)
                        last = instance->prev;

                    instance->next = nullptr;
                    instance->prev = nullptr;
                }
            };

            Pool<Entity> m_cache;
            Pool<Entity> m_alive;
            Pool<Component> m_components_cache[MAX_COMPONENTS];
            Pool<Component> m_components_alive[MAX_COMPONENTS];
            std::vector<Component*> m_visible;

        public:
            Entity* spawn_entity(f32 x, f32 y)
            {
                Entity* entity = new Entity { true, x, y };
                m_alive.insert(entity);
                return entity;
            }

            void destroy_entity() {}

            template<typename T>
            T* add_component(Entity* entity, T&& component = T())
            {
                u8 type = Component::Types::id<T>();
		        auto& cache = m_components_cache[type];
		        auto& alive = m_components_alive[type];

                T* instance;
                if (cache.first)
                {
                    instance = (T*)cache.first;
                    cache.remove(instance);
                }
                else
                {
                    instance = new T();
                }

                *instance = component;
                instance->type = type;
                instance->entity = entity;

                alive.insert(instance);

                entity->components.push_back(instance);

                instance->start();
		        return instance;
            }

            template<typename T>
            void remove_component(T* component) {}

            void update(Time time) {}
            void render(Time time) {}
        };

        template<typename T>
        T* Entity::add(T&& component)
        {
            return world->add_component(this, std::forward<T>(component));
        }

        template<typename T>
        T* Component::get()
        {
            return entity->get<T>();
        }
    }
}
