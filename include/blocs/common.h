#pragma once

#ifdef _WIN32
#define NOMINMAX
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#include <bitset>
#include <cassert>
#include <iostream>
#include <math.h>
#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <utility>

#define RED(x)  "\x1B[31m" << x << "\x1B[0m"
#define GRN(x)  "\x1B[32m" << x << "\x1B[0m"
#define YEL(x)  "\x1B[33m" << x << "\x1B[0m"
#define BLU(x)  "\x1B[34m" << x << "\x1B[0m"
#define MAG(x)  "\x1B[35m" << x << "\x1B[0m"
#define CYN(x)  "\x1B[36m" << x << "\x1B[0m"
#define WHT(x)  "\x1B[37m" << x << "\x1B[0m"

#define BOLD(x) "\x1B[1m" << x << "\x1B[0m"
#define UNDL(x) "\x1B[4m" << x << "\x1B[0m"

#ifdef DEBUG
#define __FILENAME__ \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG(msg)            \
    do                            \
    {                             \
        std::cout << msg << "\n"; \
    } while (0)

#define LOG_INFO(msg)                                            \
    do                                                           \
    {                                                            \
        std::cout << CYN("[" << __FILENAME__ << ", " << __LINE__ \
                             << "]: " << msg)                    \
                  << "\n";                                       \
    } while (0)

#define LOG_GOOD(msg)                                            \
    do                                                           \
    {                                                            \
        std::cout << GRN("[" << __FILENAME__ << ", " << __LINE__ \
                             << "]: " << msg)                    \
                  << "\n";                                       \
    } while (0)

#define LOG_WARN(msg)                                            \
    do                                                           \
    {                                                            \
        std::cout << YEL("[" << __FILENAME__ << ", " << __LINE__ \
                             << "]: " << msg)                    \
                  << "\n";                                       \
    } while (0)

#define LOG_ERR(msg)                                             \
    do                                                           \
    {                                                            \
        std::cerr << RED("[" << __FILENAME__ << ", " << __LINE__ \
                             << "]: " << msg)                    \
                  << "\n";                                       \
    } while (0)
#else
#define LOG_DEBUG(msg) \
    do                 \
    {                  \
    } while (0)
#define LOG_INFO(msg) \
    do                \
    {                 \
    } while (0)
#define LOG_GOOD(msg) \
    do                \
    {                 \
    } while (0)
#define LOG_WARN(msg) \
    do                \
    {                 \
    } while (0)
#define LOG_ERR(msg) \
    do               \
    {                \
    } while (0)
#endif

#define LOG_ASSERT(condition, msg) \
    do                             \
    {                              \
        if (!(condition))          \
        {                          \
            LOG_ERR(msg);          \
            abort();               \
        }                          \
    } while (0)

namespace blocs
{
    using i8     = int8_t;
    using i16    = int16_t;
    using i32    = int32_t;
    using i64    = int64_t;
    using u8     = uint8_t;
    using u16    = uint16_t;
    using u32    = uint32_t;
    using u64    = uint64_t;
    using uint   = unsigned int;
    using ushort = unsigned short;
    using uchar  = unsigned char;
    using byte   = signed char;
    using f32    = float;
    using f64    = double;
    using size   = size_t;

    using ss   = std::stringstream;
    using str  = std::string;
    using cstr = const char*;
}
