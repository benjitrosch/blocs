#pragma once

#include <unordered_map>

#include <blocs/common.h>
#include <blocs/ecs/resources/resource.h>

namespace blocs
{
    namespace ecs
    {
        /**
         * @brief Manages the registration and retrieval of
         * global unique data containers.
         */
        class ResourceManager
        {
        private:
            std::unordered_map<u8, Resource> m_resources;
            u8                               m_nextResourceType = 0;

            template<typename T>
            inline u8 get_type_id()
            {
                static u8 s_resource = ++m_nextResourceType;
                return s_resource;
            }

        public:
            /**
             * @tparam T type of resource to add (automatically inferred from
             * resource parameter).
             * @param resource resouce to add.
             */
            template<typename T>
            T* add(T* resource)
            {
                u8 type = get_type_id<T>();
                assert(
                    m_resources.find(type) == m_resources.end() &&
                    "ERROR: already registered resource"
                );
                m_resources.insert({type, resource});
                return resource;
            }

            /**
             * @tparam T type of resource to add.
             * @tparam Args types of resource arguments (inferred from members
             * of T).
             *
             * @param args resource arguments for list initialization.
             */
            template<typename T, typename... Args>
            T* add(Args&&... args)
            {
                return add(new T(args...));
            }

            template<typename T>
            T* replace(T* resource)
            {
                u8 type = get_type_id<T>();
                assert(
                    m_resources.find(type) != m_resources.end() &&
                    "ERROR: could not find requested resource"
                );
                auto prev         = (T*)m_resources[type];
                m_resources[type] = resource;
                delete prev;
                return (T*)m_resources[type];
            }

            /**
             * @tparam T type of resource to access.
             *
             * @return Reference to global resource.
             */
            template<typename T>
            T* get()
            {
                u8 type = get_type_id<T>();
                assert(
                    m_resources.find(type) != m_resources.end() &&
                    "ERROR: could not find requested resource"
                );
                return (T*)m_resources[type];
            }

            template<typename T>
            bool has() const
            {
                u8 type = get_type_id<T>();
                return m_resources.find(type) != m_resources.end();
            }

            /**
             * @brief Resets all registered resources to their default values.
             * Does not remove or unregister resources.
             */
            void reset()
            {
                for (auto [type, resource] : m_resources)
                {
                    resource = {};
                }
            }
        };
    }
}
