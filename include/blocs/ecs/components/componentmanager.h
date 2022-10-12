#pragma once

#include <blocs/common.h>
#include <blocs/ecs/entities/entity.h>
#include <blocs/ecs/entities/entitymanager.h>
#include <blocs/ecs/components/component.h>
#include <blocs/ecs/components/componentarray.h>

namespace blocs
{
    namespace ecs
    {
        /**
         * @brief Manages the registration and lifetime of all
         * components, and the validation of component signatures.
         */
        class ComponentManager
        {
        private:
            IComponentArray* m_componentArrays[ecs::MAX_COMPONENTS]{nullptr};
            u8               m_nextComponentType = 0;

            template<typename T>
            inline u8 get_type_id()
            {
                static u8 s_component = ++m_nextComponentType;
                return s_component;
            }

            /*
             * Creates new ComponentArray of type T
             * and inserts into map using typename as key.
             *
             * @tparam T the type of component to register.
             */
            template<typename T>
            void register_component()
            {
                u8 type = get_type_id<T>();
                assert(
                    m_componentArrays[type] == nullptr &&
                    "ERROR: already registered component"
                );
                m_componentArrays[type] = new ComponentArray<T>();
            }

        public:
            /**
             * @brief Creates a new component of type T (with passed in
             * arguments or default constructor) and associates with an entity.
             *
             * @tparam T type of component to add.
             * @param entity Entity the component will be added to.
             * @param args Arguments to pass to the constructor of component
             * type T.
             *
             * @return Reference to the new component.
             */
            template<typename T, typename... Args>
            T& add(Entity entity, Args&&... args)
            {
                return get_components<T>()->insert(entity, T(args...));
            }

            /**
             * @brief Inserts a premade component of type T into a component
             * array and associates with an entity.
             *
             * @tparam T type of component being added.
             * @param entity Entity the component will be added to.
             * @param component Component being inserted.
             */
            template<typename T>
            T& insert(Entity entity, T component)
            {
                return get_components<T>()->insert(entity, component);
            }

            /**
             * @brief Removes a component of type T from an entity.
             *
             * @tparam T type of component to remove from entity.
             * @param entity Entity the component will be removed from.
             */
            template<typename T>
            void remove(Entity entity)
            {
                get_components<T>()->remove(entity);
            }

            /**
             * @brief Removes all components belonging to an entity.
             *
             * @param entity Entity to remove all components from.
             */
            void remove(Entity entity)
            {
                for (i32 i = 0; i < ecs::MAX_COMPONENTS; i++)
                {
                    if (m_componentArrays[i] != nullptr)
                    {
                        m_componentArrays[i]->remove(entity);
                    }
                }
            }

            /**
             * @brief Returns ComponentArray for type T.
             *
             * @tparam T the type of requested ComponentArray.
             */
            template<typename T>
            ComponentArray<T>* get_components()
            {
                u8 type = get_type_id<T>();
                if (m_componentArrays[type] != nullptr)
                    return (ComponentArray<T>*)m_componentArrays[type];
                else
                {
                    register_component<T>();
                    return get_components<T>();
                }
            }

            IComponentArray* get_components(u8 type)
            {
                return m_componentArrays[type];
            }

            /**
             * @tparam T type of component to retrieve.
             * @param entity Entity to get component of type T from.
             *
             * @return Reference to component belonging to entity.
             */
            template<typename T>
            T& get(Entity entity)
            {
                return get_components<T>()->get(entity);
            }

            /**
             * @tparam T type of component to check for.
             * @param Entity to check for component of type T.
             *
             * @return Whether an entity has a component of type T.
             */
            template<typename... Types>
            bool has(Entity entity)
            {
                return ((get_components<Types>()->set.has(entity)) && ...);
            }

            /**
             * @brief Calls `reset` on all Component Arrays which reinitializes
             * all components to their default values.
             */
            void reset()
            {
                for (i32 i = 0; i < ecs::MAX_COMPONENTS; i++)
                {
                    if (m_componentArrays[i] != nullptr)
                        m_componentArrays[i]->reset();
                }
            }
        };
    }
}
