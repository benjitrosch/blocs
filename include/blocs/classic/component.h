#pragma once

#include <blocs/time.h>

namespace blocs
{
    namespace classic
    {
        struct World;
        struct Entity;
        
        struct Component
        {
            class Types
            {
            private:
                static inline u8 s_componentType = 0;

            public:
                static uint8_t count() { return s_componentType; }

                template<class T>
                static uint8_t id()
                {
                    static const uint8_t value = Types::s_componentType++;
                    return value;
                }
            };
            
            uint8_t type    = 0;
            World* world    = nullptr;
            Entity* entity  = nullptr;
            Component* prev = nullptr;
            Component* next = nullptr;

            bool active = true;;

            template<class T>
		    T* get();

            virtual void start() {}
            virtual void update(Time time) {}
            virtual void render(Time time) {}
            virtual void destroy() {}
        };
    }
}