#pragma once

#include <unordered_map>
#include <vector>

#include <blocs/common.h>
#include <blocs/ecs/systems/system.h>

namespace blocs
{
    namespace ecs
    {
        /** Specifies system update schedule */
        enum class Stage : uchar
        {
            START,
            EARLY_UPDATE,
            UPDATE,
            LATE_UPDATE,
            EVENT,
            DRAW,
            UI,
            SHUTDOWN,
        };

        /**
         * @brief Manages systems registration, lookup by stage schedule,
         * and retrieval.
         */
        struct SystemManager
        {
            std::unordered_map<Stage, std::vector<System>> systems{};

            std::vector<SetupSystem>    setup_systems{};
            std::vector<EventSystem>    event_systems{};
            std::vector<ShutdownSystem> shutdown_systems{};

            /**
             * @brief Registers a new system for a stage to be called once per
             * stage update.
             *
             * @param stage Stage system will be scheduled to run during.
             * @param system System to add.
             */
            void add(Stage stage, System system)
            {
                systems[stage].push_back(system);
            }

            /**
             * @brief Registers a new system to be called once on startup
             * during the `START` stage.
             *
             * @param system System to add.
             */
            void add_setup(SetupSystem system)
            {
                setup_systems.push_back(system);
            }

            void add_event(EventSystem system)
            {
                event_systems.push_back(system);
            }

            void add_shutdown(ShutdownSystem system)
            {
                shutdown_systems.push_back(system);
            }
        };
    }
}
