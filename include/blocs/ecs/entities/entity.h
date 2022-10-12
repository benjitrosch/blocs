#pragma once

namespace blocs
{
    namespace ecs
    {
        using Entity = u64;

        // TODO: implement versioning to prevent errors from accessing data of
        // deleted entities
        using EntityIndex   = u32;
        using EntityVersion = u32;
    }
}
