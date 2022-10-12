#pragma once

#include <blocs.h>

namespace blocs
{
    struct Coroutine
    {
        i32 at = 0;

        f32 wait_for   = 0;
        f32 repeat_for = 0;
    };
}

#define rt_begin(routine, dt)                   \
    if (routine.wait_for > 0)                   \
        routine.wait_for -= dt;                 \
    else                                        \
    {                                           \
        auto& __rt = routine; /* routine ref */ \
        auto  __mn = true;    /* move-next */   \
        switch (__rt.at)                        \
        {                                       \
            case 0:                             \
            {
#define rt_step()                 \
    }                             \
    if (__mn) __rt.at = __LINE__; \
    break;                        \
    case __LINE__:                \
    {
#define rt_for(time, dt)                \
    rt_step();                          \
    if (__rt.repeat_for < time)         \
    {                                   \
        __rt.repeat_for += dt;          \
        __mn = __rt.repeat_for >= time; \
        if (__mn) __rt.repeat_for = 0;  \
    }

#define rt_while(condition)       \
    }                             \
    if (__mn) __rt.at = __LINE__; \
    break;                        \
    case __LINE__:                \
        if (condition)            \
        {                         \
            __mn = false

#define rt_until(condition)                                   \
    }                                                         \
    if (__mn) __rt.at = ((condition) ? __LINE__ : -__LINE__); \
    break;                                                    \
    case -__LINE__:                                           \
        if (condition) __rt.at = __LINE__;                    \
        break;                                                \
    case __LINE__:                                            \
    {
#define rt_wait(time)             \
    }                             \
    if (__mn)                     \
    {                             \
        __rt.wait_for = time;     \
        __rt.at       = __LINE__; \
    }                             \
    break;                        \
    case __LINE__:                \
    {
#define rt_end()            \
    }                       \
    if (__mn) __rt.at = -1; \
    break;                  \
    }                       \
    }                       \
    goto rt_end_of_routine; \
    rt_end_of_routine:

#define rt_restart()            \
    do                          \
    {                           \
        __rt.at         = 0;    \
        __rt.wait_for   = 0;    \
        __rt.repeat_for = 0;    \
        goto rt_end_of_routine; \
    } while (false)
