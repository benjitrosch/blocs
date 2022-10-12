#pragma once

#include <queue>
#include <unordered_map>

#include <blocs/ecs/sparse.h>
#include <blocs/ecs/entities/entity.h>
#include <blocs/ecs/components/component.h>

namespace blocs
{
    namespace ecs
    {
        /**
         * @brief Manages entity identifier lifecycle methods and naming.
         */
        struct EntityManager
        {
            sparse_set<Entity> entities{ecs::MAX_ENTITIES};

            /** Queue of ascending integers representing unused entity id's. */
            std::queue<Entity> available_entities{};
            size               num_active_entities{};

            /** Map of entity index to entity name. */
            std::unordered_map<Entity, str> entity_to_name{};
            /** Map of entity name to entity index. */
            std::unordered_map<str, Entity> name_to_entity{};

            EntityManager()
            {
                // Initialize the available entities queue with all integers
                // from 0 to the max # of entities.
                for (Entity entity = 0; entity < ecs::MAX_ENTITIES; ++entity)
                {
                    available_entities.push(entity);
                }
            }

            /**
             * @brief Adds available entity id to active entities vector
             * and appears in System component queries next frame.
             *
             * @return New `entity` represented by uint32.
             */
            Entity create(str name)
            {
                assert(
                    entities.len + 1 < ecs::MAX_ENTITIES &&
                    "ERROR: max entity limit reached"
                );
                Entity id = available_entities.front();
                available_entities.pop();
                num_active_entities++;

                assert(!entities.has(id) && "ERROR: entity already exists");
                entities.add(id);

                name += std::to_string(id);
                assert(
                    name_to_entity.find(name) == name_to_entity.end() &&
                    "ERROR: cannot have duplicate entity names"
                );
                entity_to_name.insert({id, name});
                name_to_entity.insert({name, id});

                return id;
            }

            Entity create()
            {
                assert(
                    entities.len + 1 < ecs::MAX_ENTITIES &&
                    "ERROR: max entity limit reached"
                );
                Entity id = available_entities.front();
                available_entities.pop();
                num_active_entities++;

                assert(!entities.has(id) && "ERROR: entity already exists");
                entities.add(id);

                str name = std::to_string(id);
                assert(
                    name_to_entity.find(name) == name_to_entity.end() &&
                    "ERROR: cannot have duplicate entity names"
                );
                entity_to_name.insert({id, name});
                name_to_entity.insert({name, id});

                return id;
            }

            /**
             * @param entity Entity to retrieve name for.
             *
             * @return Entity name.
             */
            str get_name(Entity entity) { return entity_to_name[entity]; }

            /**
             * @param name Name of entity to lookup by name.
             *
             * @return Entity with matching name.
             */
            Entity get_by_name(const str& name) { return name_to_entity[name]; }

            /**
             * @param entity Entity to name.
             * @param name New name to associate with Entity.
             */
            void set_name(Entity entity, const str& name)
            {
                entity_to_name[entity] = name;
                name_to_entity[name]   = entity;
            }

            /**
             * @return Key value pairs of all active entities and their names.
             */
            std::unordered_map<Entity, str>& get_all_active_names()
            {
                return entity_to_name;
            }

            /**
             * @brief Finds and removes an from entity among the active entities
             * vector and returns the id to the availabe entities queue.
             *
             * @param entity Entity to be removed.
             */
            void remove(Entity entity)
            {
                assert(
                    entity < ecs::MAX_ENTITIES && "ERROR: entity out of range"
                );
                assert(entities.has(entity) && "ERROR: entity does not exist");

                entities.remove(entity);

                name_to_entity.erase(entity_to_name[entity]);
                entity_to_name.erase(entity);

                // Return the entity id to back of available queue to be reused
                // later.
                available_entities.push(entity);
                num_active_entities--;
            }

            /** @brief Clears all entities and resets availableEntities queue.
             */
            void reset()
            {
                available_entities = {};
                for (Entity entity = 0; entity < ecs::MAX_ENTITIES; ++entity)
                    available_entities.push(entity);

                entities.clear();
                name_to_entity.clear();
                entity_to_name.clear();
                num_active_entities = 0;
            }
        };
    }
}
