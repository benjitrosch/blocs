#pragma once

/** Math */
    #include "blocs/math/matrix.h"
    #include "blocs/math/vec.h"
    #include "blocs/math/shapes.h"
#include "blocs/math/calc.h"

/** Entity Component System */
#include "blocs/ecs/world.h"
#include "blocs/ecs/sparse.h"
//  1. Entity
    #include "blocs/ecs/entities/entity.h"
    #include "blocs/ecs/entities/entitymanager.h"
//  2. Component
    #include "blocs/ecs/components/component.h"
    #include "blocs/ecs/components/componentarray.h"
    #include "blocs/ecs/components/componentmanager.h"
//  3. System
    #include "blocs/ecs/systems/system.h"
    #include "blocs/ecs/systems/systemmanager.h"
//  4. Resource
    #include "blocs/ecs/resources/resource.h"
    #include "blocs/ecs/resources/resourcemanager.h"

/** Classic Components */
    #include "blocs/classic/component.h"
    #include "blocs/classic/entity.h"
#include "blocs/classic/world.h"

/** Graphics */
#include "blocs/graphics/renderer.h"
    #include "blocs/graphics/font.h"
    #include "blocs/graphics/image.h"
    #include "blocs/graphics/texture.h"
    #include "blocs/graphics/shader.h"
    #include "blocs/graphics/spritebatch.h"
    #include "blocs/graphics/spriteatlas.h"
    #include "blocs/graphics/spritefont.h"
    #include "blocs/graphics/shadermanager.h"
    #include "blocs/graphics/rendertarget.h"

/** Platform */
#include "blocs/platform/platform.h"
    #include "blocs/platform/input.h"
    #include "blocs/platform/filesystem.h"

/** Debug */
#include "blocs/debug/imgui.h"
#include "blocs/debug/tests.h"
//  1. Explorer
    #include "blocs/debug/explorer/serializer.h"
    #include "blocs/debug/explorer/network.h"
    #include "blocs/debug/explorer/monitor.h"

/** Common */
#include "blocs/common.h"
#include "blocs/game.h"
#include "blocs/time.h"
#include "blocs/coroutine.h"
