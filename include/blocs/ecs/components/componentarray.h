#pragma once

#include <unordered_map>

#include <blocs/common.h>
#include <blocs/ecs/sparse.h>
#include <blocs/ecs/entities/entity.h>

namespace blocs
{
    namespace ecs
    {
        /** @brief Interface for component array virtual inheritance. */
        struct IComponentArray
        {
            virtual ~IComponentArray() = default;

            virtual void remove(Entity entity) = 0;
            virtual void reset()               = 0;
        };

        /**
         * @brief Container for component data and maintains
         * relationship between components and entities.
         *
         * @tparam T Type of component.
         */
        template<typename T>
        struct ComponentArray : public IComponentArray
        {
            /**
             * @brief Packed array of components contiguous in memory
             * with one index for each entity.
             */
            T                  components[ecs::MAX_ENTITIES];
            sparse_set<Entity> set{ecs::MAX_ENTITIES};

            /**
             * @brief Adds a component to the next available index in the packed
             * array. Then creates two maps between entity and array index.
             *
             * @param entity 	Entity the component belongs to.
             * @param component Component being added to packed array.
             *
             * @return Reference to the added component.
             */
            T& insert(Entity entity, T component)
            {
                set.add(entity);
                components[set.value(entity)] = component;
                return components[set.value(entity)];
            }

            /**
             * @brief Removes an entity and updates the maps to keep array data
             * packed.
             *
             * @param entity Entity to remove.
             */
            void remove(Entity entity) override { set.remove(entity); }

            /** @brief Resets all component member values to default. */
            void reset() override
            {
                memset(components, 0, sizeof(components));
                set.clear();
            }

            /**
             * @param entity Entity to get the component of.
             *
             * @return Reference to the component belonging to the specified
             * entity.
             */
            T& get(Entity entity) { return components[set.value(entity)]; }

            /**
             * @param entity Which entity to check for the component.
             *
             * @return Whether an entity has a component of the specified type.
             */
            bool has(Entity entity) const { return set.has(entity); }

            size size() const { return set.len; }

            T& get_singleton() { return components[set.dense[0]]; }
        };
    }
}
