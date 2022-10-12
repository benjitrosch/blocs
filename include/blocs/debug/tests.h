#pragma once

#ifdef DEBUG
#define PRINT_TEST_ARG_(NAME) #NAME
#define PRINT_TEST_ARG(NAME)  PRINT_TEST_ARG_(NAME)

#define DESCRIBE(name, ...)                                              \
    do                                                                   \
    {                                                                    \
        int passes = 0;                                                  \
        int fails  = 0;                                                  \
                                                                         \
        std::stringstream ss;                                            \
        for (auto e : __VA_ARGS__)                                       \
        {                                                                \
            if (e.passed)                                                \
            {                                                            \
                passes++;                                                \
                ss << "\t" << GRN("✓ \"" << e.describe << "\"") << "\n"; \
            }                                                            \
            else                                                         \
            {                                                            \
                fails++;                                                 \
                ss << "\t" << RED("x \"" << e.describe << "\"") << "\n"  \
                   << "\t\t"                                             \
                   << "expect:\t"                                        \
                   << "\x1B[1m" << YEL(e.expect) << "\n"                 \
                   << "\t\t"                                             \
                   << "result:\t"                                        \
                   << "\x1B[1m" << (e.passed ? "\x1B[32m" : "\x1B[31m")  \
                   << e.result << "\x1B[0m"                              \
                   << "\n";                                              \
            }                                                            \
        }                                                                \
        ss << "\n";                                                      \
        ss << "\tran " << CYN(passes + fails) << " test(s)\n";           \
        ss << "\tpass:\t" << GRN(passes) << "\n";                        \
        ss << "\tfail:\t" << RED(fails);                                 \
                                                                         \
        std::cout << (fails == 0 ? "\x1B[42m PASS \x1B[0m"               \
                                 : "\x1B[41m FAIL \x1B[0m")              \
                  << " " << BOLD(__FILENAME__) << "\n"                   \
                  << CYN("\"" << name << "\""                            \
                              << "\n")                                   \
                  << ss.str() << "\n"                                    \
                  << std::endl;                                          \
    } while (0)

#define EXPECT(describe, a, b)                               \
    blocs::debug::test::compare(                             \
        describe, a, b, PRINT_TEST_ARG(a), PRINT_TEST_ARG(b) \
    )

namespace blocs
{
    namespace debug
    {
        namespace test
        {
            struct test_result
            {
                str describe;
                str expect;
                str result;

                bool passed;
            };

            template<typename T>
            test_result compare(
                const str& describe, T a, T b, str a_str, str b_str
            )
            {
                return {describe, b_str, a_str, a == b};
            }
        }
    }
}
#else
#define DESCRIBE(name, ...) \
    do                      \
    {                       \
    } while (0)
#define EXPECT(describe, a, b) \
    do                         \
    {                          \
    } while (0)
#endif
