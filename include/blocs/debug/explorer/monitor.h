#pragma once

#include <thread>

namespace blocs
{
    namespace explorer
    {
        template<typename... Types>
        struct MonitorComponentsQuery
        {
        };

        template<typename... Types>
        struct MonitorSystemsQuery
        {
        };

        template<typename... Types>
        struct MonitorResourcesQuery
        {
        };

        template<
            typename... Components, typename... Systems, typename... Resources>
        void monitor(
            ecs::World* world,
            const MonitorComponentsQuery<Components...>& = {},
            const MonitorSystemsQuery<Systems...>&       = {},
            const MonitorResourcesQuery<Resources...>& = {}, i32 port = 4001
        )
        {
#ifdef DEBUG
            std::thread(
                [world, port]()
                {
                    network::on(
                        port, "/",
                        [&]()
                        {
                            return (
                                str("{") + "\"project\":{" +
                                // "\"title\":\"" + str(title) + "\"," +
                                // "\"fps\":" + std::to_string((f32)time.fps) +
                                "}," + "\"ecs\":{" + "\"entities\":" +
                                serializer::json::names_to_json(
                                    world->entities.get_all_active_names()
                                ) +
                                ",\"components\":" +
                                serializer::json::components_to_json(
                                    world->iter<Components>()...
                                ) +
                                ",\"systems\":" + "{}" + ",\"resources\":" +
                                serializer::json::resources_to_json(
                                    *world->resources.get<Resources>()...
                                ) +
                                "}" + "}"
                            );
                        }
                    );
                }
            ).detach();
#endif
        }
    }
}
