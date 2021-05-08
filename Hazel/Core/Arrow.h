// /*
// _ _    _           ______   _______        
// | |  | |    /\    /___  /   |  ____|| |    
// | |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
// |  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
// | |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
// |_|_ |_|/_/    \_\/_______\ |______|_\______|

// Licensed under the MIT License <http://opensource.org/licenses/MIT>
// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
// */

// #ifndef CSTL_UTEST_H
// #define CSTL_UTEST_H

// #include <Hazel/Core/Types.h>

// #ifdef _MSC_VER
//     /*
//         Disable warning about not inlining 'inline' functions.
//     */
//     #pragma warning(disable : 4710)

//     /*
//         Disable warning about inlining functions that are not marked 'inline'.
//     */
//     #pragma warning(disable : 4711)
//     #pragma warning(push, 1)
// #endif

// #include <stddef.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <errno.h>

// #ifdef _MSC_VER
//     #pragma warning(pop)
// #endif

// #ifdef __cplusplus
//     #define CSTL_C_FUNC extern "C"
// #else
//     #define CSTL_C_FUNC
// #endif

// #ifdef _MSC_VER
//     // define CSTL_USE_OLD_QPC before #include "cstl_utest.h" to use old QueryPerformanceCounter
//     #ifndef CSTL_USE_OLD_QPC
//         #pragma warning(push, 0)
//         #include <Windows.h>
//         #pragma warning(pop)

//         typedef LARGE_INTEGER LargeInt;
//     #else 
//         //use old QueryPerformanceCounter definitions (not sure is this needed in some edge cases or not)
//         //on Win7 with VS2015 these extern declaration cause "second C linkage of overloaded function not allowed" error
//         typedef union {
//             struct {
//                 unsigned long LowPart;
//                 long HighPart;
//             } DUMMYSTRUCTNAME;
//             struct {
//                 unsigned long LowPart;
//                 long HighPart;
//             } u;
//             Int64 QuadPart;
//         } LargeInt;

//         CSTL_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(
//         LargeInt *);
//         CSTL_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(
//         LargeInt *);
//     #endif

// #elif defined(__linux__)
//     /*
//     slightly obscure include here - we need to include glibc's features.h, but
//     we don't want to just include a header that might not be defined for other
//     c libraries like musl. Instead we include limits.h, which we know on all
//     glibc distributions includes features.h
//     */
//     #include <limits.h>

//     #ifdef __GLIBC__ && __GLIBC_MINOR__
//         #include <time.h>

//         #if ((2 < __GLIBC__) || ((2 == __GLIBC__) && (17 <= __GLIBC_MINOR__)))
//         /* glibc is version 2.17 or above, so we can just use clock_gettime */
//         #define CSTL_USE_CLOCKGETTIME
//         #else
//         #include <sys/syscall.h>
//         #include <unistd.h>
//         #endif
//     #else // Other libc implementations
//         #include <time.h>
//         #define CSTL_USE_CLOCKGETTIME
//     #endif

// #elif defined(__APPLE__)
//     #include <mach/mach_time.h>
// #endif

// #ifdef _MSC_VER && _MSC_VER < 1920
//     #define CSTL_PRId64 "I64d"
//     #define CSTL_PRIu64 "I64u"
// #else
//     #include <inttypes.h>

//     #define CSTL_PRId64 PRId64
//     #define CSTL_PRIu64 PRIu64
// #endif

// #ifdef _MSC_VER
//     #ifdef _WIN64
//         #define CSTL_SYMBOL_PREFIX
//     #else
//         #define CSTL_SYMBOL_PREFIX "_"
//     #endif

//     #ifdef __clang__
//         #define CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
//             _Pragma("clang diagnostic push")                                          \
//             _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")

//         #define CSTL_INITIALIZER_END_DISABLE_WARNINGS \
//             _Pragma("clang diagnostic pop")
//     #else
//         #define CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS
//         #define CSTL_INITIALIZER_END_DISABLE_WARNINGS
//     #endif

//     #pragma section(".CRT$XCU", read)
//     #define CSTL_INITIALIZER(f)                                                   \
//         static void __cdecl f(void);                                                 \
//         CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS                                     \
//             __pragma(comment(linker, "/include:" CSTL_SYMBOL_PREFIX #f "_"))            \
//             CSTL_C_FUNC __declspec(allocate(".CRT$XCU")) void(__cdecl* f##_)(void) = f;      \
//         CSTL_INITIALIZER_END_DISABLE_WARNINGS                                       \
//         static void __cdecl f(void)

// #else
// #ifdef __linux__
//     #ifdef __clang__
//         #if __has_warning("-Wreserved-id-macro")
//         #pragma clang diagnostic push
//         #pragma clang diagnostic ignored "-Wreserved-id-macro"
//     #endif // __clang__
// #endif __linux__

// #define __STDC_FORMAT_MACROS 1

// #ifdef __clang__
//     #if __has_warning("-Wreserved-id-macro")
//     #pragma clang diagnostic pop
//     #endif
// #endif // __clang__
// #endif

// #define CSTL_INITIALIZER(f)                                                   \
//     static void f(void)     __attribute__((constructor));                            \
//     static void f(void)

// #endif // _MSC_VER

// #ifdef __cplusplus
//     #define CSTL_CAST(type, x)      static_cast<type>(x)
//     #define CSTL_PTR_CAST(type, x)  reinterpret_cast<type>(x)
//     #define CSTL_EXTERN             extern "C"
// #else
//     #define CSTL_CAST(type, x)     ((type)x)
//     #define CSTL_PTR_CAST(type, x) ((type)x)
//     #define CSTL_EXTERN            extern
// #endif // __cplusplus


// #ifdef _MSC_VER
//     /*
//     io.h contains definitions for some structures with natural padding. This is
//     uninteresting, but for some reason MSVC's behaviour is to warn about
//     including this system header. That *is* interesting
//     */
//     #pragma warning(disable : 4820)
//     #pragma warning(push, 1)
//     #include <io.h>
//     #pragma warning(pop)
//     #define CSTL_COLOUR_OUTPUT() (_isatty(_fileno(stdout)))
// #else
//     #include <unistd.h>
//     #define CSTL_COLOUR_OUTPUT()    (isatty(STDOUT_FILENO))
// #endif

// static inline void* cstl_utest_realloc(void* const pointer, Ll new_size) {
//     void* const new_pointer = realloc(pointer, new_size);

//     if (new_pointer == null)    free(new_pointer);

//     return new_pointer;
// }

// static inline Int64 cstl_utest_ns(void) {
//     #ifdef _MSC_VER
//         LargeInt counter;
//         LargeInt frequency;
//         QueryPerformanceCounter(&counter);
//         QueryPerformanceFrequency(&frequency);
//         return CSTL_CAST(Int64, (counter.QuadPart * 1000000000) / frequency.QuadPart);

//     #elif defined(__linux) && defined(__STRICT_ANSI__)
//         return CSTL_CAST(Int64, clock()) * 1000000000 / CLOCKS_PER_SEC;

//     #elif defined(__linux)
//         struct timespec ts;
//         #ifdef __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
//             timespec_get(&ts, TIME_UTC);
//         #else
//             const clockid_t cid = CLOCK_REALTIME;

//         #ifdef CSTL_USE_CLOCKGETTIME
//             clock_gettime(cid, &ts);
//         #else
//             syscall(SYS_clock_gettime, cid, &ts);
//         #endif // CSTL_USE_CLOCKGETTIME

//         #endif
//         return CSTL_CAST(Int64, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec;

//     #elif __APPLE__
//         return CSTL_CAST(Int64, mach_absolute_time());
//     #endif
// }

// typedef void (*cstl_utest_testcase_t)(int* , Ll);

// struct cstl_utest_test_state_s {
//     cstl_utest_testcase_t func;
//     Ll index;
//     char* name;
// };

// struct cstl_utest_state_s {
//     struct cstl_utest_test_state_s *tests;
//     Ll tests_length;
//     FILE* output;
// };

// /* extern to the global state cstl_utest needs to execute */
// CSTL_EXTERN struct cstl_utest_state_s cstl_utest_state;

// #ifdef _MSC_VER
//     #define CSTL_WEAK inline
//     #define CSTL_UNUSED
// #else
//     #define CSTL_WEAK __attribute__((weak))
//     #define CSTL_UNUSED __attribute__((unused))
// #endif // _MSC_VER


// #ifdef __clang__
//     #pragma clang diagnostic push
//     #pragma clang diagnostic ignored "-Wvariadic-macros"
//     #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
// #endif // __clang__

// #define CSTL_PRINTF(...)                                                      \
//     if (cstl_utest_state.output) {                                                    \
//         fprintf(cstl_utest_state.output, __VA_ARGS__);                                  \
//     }                                                                            \
//     printf(__VA_ARGS__)

// #ifdef __clang__
//     #pragma clang diagnostic pop
// #endif

// #ifdef __clang__
//     #pragma clang diagnostic push
//     #pragma clang diagnostic ignored "-Wvariadic-macros"
//     #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
// #endif

// #ifdef _MSC_VER
//     #define CSTL_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
// #else
//     #define CSTL_SNPRINTF(...) snprintf(__VA_ARGS__)
// #endif

// #ifdef __clang__
//     #pragma clang diagnostic pop
// #endif

// #if __cplusplus
//     /* if we are using c++ we can use overloaded methods (its in the language) */
//     #define CSTL_OVERLOADABLE 
// #elif __clang__
//     /* otherwise, if we are using clang with c - use the overloadable attribute */
//     #define CSTL_OVERLOADABLE __attribute__((overloadable))
// #endif // __cplusplus

// #if CSTL_OVERLOADABLE
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(float f);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(float f) {
//         CSTL_PRINTF("%f", CSTL_CAST(double, f));
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(double d);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(double d) {
//         CSTL_PRINTF("%f", d);
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long double d);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long double d) {
//         CSTL_PRINTF("%Lf", d);
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(int i);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(int i) {
//         CSTL_PRINTF("%d", i);
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(unsigned int i);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(unsigned int i) {
//         CSTL_PRINTF("%u", i);
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long int i);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long int i) {
//         CSTL_PRINTF("%ld", i);
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long unsigned int i);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long unsigned int i) {
//         CSTL_PRINTF("%lu", i);
//     }

//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(const void* p);
//     CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(const void* p) {
//         CSTL_PRINTF("%p", p);
//     }

//     /*
//     long long is a c++11 extension
//     */
//     #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)

//         #ifdef __clang__
//             #pragma clang diagnostic push
//             #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
//         #endif

//         CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long long int i);
//         CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long long int i) {
//             CSTL_PRINTF("%lld", i);
//         }

//         CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(Ull int i);
//         CSTL_WEAK CSTL_OVERLOADABLE void
//         cstl_utest_type_printer(Ull int i) {
//             CSTL_PRINTF("%llu", i);
//         }

//         #ifdef __clang__
//             #pragma clang diagnostic pop
//         #endif

//     #endif

// #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)

//     #define cstl_utest_type_printer(val)                                       \
//         CSTL_PRINTF(_Generic((val), signed char                                \
//                         : "%d", unsigned char                                  \
//                         : "%u", short                                          \
//                         : "%d", unsigned short                                 \
//                         : "%u", int                                            \
//                         : "%d", long                                           \
//                         : "%ld", long long                                     \
//                         : "%lld", unsigned                                     \
//                         : "%u", unsigned long                                  \
//                         : "%lu", unsigned long long                            \
//                         : "%llu", float                                        \
//                         : "%f", double                                         \
//                         : "%f", long double                                    \
//                         : "%Lf", default                                       \
//                         : _Generic((val - val), ptrdiff_t                      \
//                                     : "%p", default                            \
//                                     : "undef")),                               \
//                 (val))
// #else
// /*
//     we don't have the ability to print the values we got, so we create a macro
//     to tell our users we can't do anything fancy
//     */
//     #define cstl_utest_type_printer(...) CSTL_PRINTF("undef")
// #endif

// #ifdef _MSC_VER
//     #define CSTL_SUPPRESS_WARNING_BEGIN  __pragma(warning(push)) __pragma(warning(disable : 4127))
//     #define CSTL_SUPPRESS_WARNING_END    __pragma(warning(pop))
// #else
//     #define CSTL_SUPPRESS_WARNING_BEGIN
//     #define CSTL_SUPPRESS_WARNING_END
// #endif

// #ifdef __cplusplus && __cplusplus >= 201103L
//     #define CSTL_AUTO(x)        auto

// #elif !defined(__cplusplus)

//     #ifdef __clang__
//         /* clang-format off */
//         /* had to disable clang-format here because it malforms the pragmas */
//         #define CSTL_AUTO(x)                                                          \
//             _Pragma("clang diagnostic push")                                          \
//             _Pragma("clang diagnostic ignored \"-Wgnu-auto-type\"") __auto_type       \
//             _Pragma("clang diagnostic pop")
//         /* clang-format on */
//     #else
//         #define CSTL_AUTO(x)    __typeof__(x + 0)
//     #endif

// #else
//     #define CSTL_AUTO(x)        typeof(x + 0)
// #endif


// #if defined(__clang__)
//     #define CSTL_STRNCMP(x, y, size)                                              \
//         _Pragma("clang diagnostic push")                                             \
//         _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"")       \
//         strncmp(x, y, size) _Pragma("clang diagnostic pop")
// #else
//     #define CSTL_STRNCMP(x, y, size)    strncmp(x, y, size)
// #endif

// #if defined(__clang__)
//     #define CSTL_CHECK(x, y, cond)                                               \
//         CSTL_SUPPRESS_WARNING_BEGIN 
//         do {                                            \
//             _Pragma("clang diagnostic push")                                           \
//             _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
//             _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
//             _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
//             CSTL_AUTO(x) xEval = (x);                                 \
//             CSTL_AUTO(y) yEval = (y);                                                 \
//                                                                                     \
//             if (!((xEval)cond(yEval))) {                                               \
//                 _Pragma("clang diagnostic pop")                                          \
//                 CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                \
//                 CSTL_PRINTF("  Expected : ");                                           \
//                 cstl_utest_type_printer(xEval);                                               \
//                 CSTL_PRINTF("\n");                                                      \
//                 CSTL_PRINTF("    Actual : ");                                           \
//                 cstl_utest_type_printer(yEval);                                               \
//                 CSTL_PRINTF("\n");                                                      \
//                 *cstl_utest_result = 1;                                                       \
//             }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END
    
// #elif defined(__GNUC__)
//     #define CSTL_CHECK(x, y, cond)                                               \
//         CSTL_SUPPRESS_WARNING_BEGIN 
//         do {                                            \
//             const int use_colours = CSTL_COLOUR_OUTPUT();                \
//             const char* colours[] = {"\033[0m", "\033[32m", "\033[31m"}; \
//             if (!use_colours) { \
//                 for (index = 0; index < sizeof colours / sizeof colours[0]; index++) { \
//                     colours[index] = ""; \
//                 } \
//             } \

//             CSTL_AUTO(x) xEval = (x);                                                 \
//             CSTL_AUTO(y) yEval = (y);  
//                                                            \
//             if (!((xEval)cond(yEval))) {                                               \
//                 CSTL_PRINTF("%s:%s%u%s: Failure\n", __FILE__, colours[RED], __LINE__);                    \
//                 CSTL_PRINTF("  Expected : ");                                           \
//                 cstl_utest_type_printer(xEval);                                               \
//                 CSTL_PRINTF("\n");                                                      \
//                 CSTL_PRINTF("    Actual : ");                                           \
//                 cstl_utest_type_printer(yEval);                                               \
//                 CSTL_PRINTF("\n");                                                      \
//                 *cstl_utest_result = 1;                                                       \
//             }                                                                          \
//         }                                                                            \
//         while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #else
//     #define CSTL_CHECK(x, y, cond)                                               \
//         CSTL_SUPPRESS_WARNING_BEGIN 
//         do {                                            \
//             if (!((x)cond(y))) {                                                       \
//                 CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//                 *cstl_utest_result = 1;                                                       \
//             }                                                                          \
//         }                                                                            \
//         while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END
// #endif

// #define EXPECT_TRUE(x)                                                         \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (!(x)) {                                                                \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : true\n");                                     \
//             CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//             *cstl_utest_result = 1;                                                       \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define EXPECT_FALSE(x)                                                        \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (x) {                                                                   \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : false\n");                                    \
//             CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//             *cstl_utest_result = 1;                                                       \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define EXPECT_EQ(x, y)     CSTL_CHECK(x, y, ==)
// #define EXPECT_NE(x, y)     CSTL_CHECK(x, y, !=)
// #define EXPECT_LT(x, y)     CSTL_CHECK(x, y, <)
// #define EXPECT_LE(x, y)     CSTL_CHECK(x, y, <=)
// #define EXPECT_GT(x, y)     CSTL_CHECK(x, y, >)
// #define EXPECT_GE(x, y)     CSTL_CHECK(x, y, >=)


// #define EXPECT_STREQ(x, y)                                                     \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 != strcmp(x, y)) {                                                   \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
//             CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
//             *cstl_utest_result = 1;                                                       \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define EXPECT_STRNE(x, y)                                                     \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 == strcmp(x, y)) {                                                   \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
//             CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
//             *cstl_utest_result = 1;                                                       \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define EXPECT_STRNEQ(x, y, n)                                                 \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 != CSTL_STRNCMP(x, y, n)) {                                         \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
//             CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
//             *cstl_utest_result = 1;                                                       \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define EXPECT_STRNNE(x, y, n)                                                 \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 == CSTL_STRNCMP(x, y, n)) {                                         \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
//             CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
//             *cstl_utest_result = 1;                                                       \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #if defined(__clang__)
//     #define CSTL_ASSERT(x, y, cond)                                               \
//         CSTL_SUPPRESS_WARNING_BEGIN 
//         do {                                            \
//             _Pragma("clang diagnostic push")                                           \
//             _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
//             _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
//             _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \

//             CSTL_AUTO(x) xEval = (x);                                 \
//             CSTL_AUTO(y) yEval = (y);                                                 \

//             if (!((xEval)cond(yEval))) {                                               \
//                 _Pragma("clang diagnostic pop")                                          \
//                 CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                \
//                 CSTL_PRINTF("  Expected : ");                                           \
//                 cstl_utest_type_printer(xEval);                                               \
//                 CSTL_PRINTF("\n");                                                      \
//                 CSTL_PRINTF("    Actual : ");                                           \
//                 cstl_utest_type_printer(yEval);                                               \
//                 CSTL_PRINTF("\n");                                                      \
//                 *cstl_utest_result = 1;                                                       \
//                 return;                                                                  \
//             }                                                                          \
//         }                                                                            \
//         while (0)                                                                    \
//         CSTL_SUPPRESS_WARNING_END

// #elif defined(__GNUC__)

// #define CSTL_ASSERT(x, y, cond)                                               \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         CSTL_AUTO(x) xEval = (x);                                                 \
//         CSTL_AUTO(y) yEval = (y);    
//                                                      \
//         if (!((xEval)cond(yEval))) {                                               \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : ");                                           \
//             cstl_utest_type_printer(xEval);                                               \
//             CSTL_PRINTF("\n");                                                      \
//             CSTL_PRINTF("    Actual : ");                                           \
//             cstl_utest_type_printer(yEval);                                               \
//             CSTL_PRINTF("\n");                                                      \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #else
//     #define CSTL_ASSERT(x, y, cond)                                               \
//         CSTL_SUPPRESS_WARNING_BEGIN 
//         do {                                            \
//             if (!((x)cond(y))) {                                                       \
//                 CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//                 *cstl_utest_result = 1;                                                       \
//                 return;                                                                  \
//             }                                                                          \
//         }                                                                            \
//         while (0)                                                                    \
//         CSTL_SUPPRESS_WARNING_END
// #endif // __clang__

// #define ASSERT_TRUE(x)                                                         \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//     if (!(x)) {                                                                \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : true\n");                                     \
//             CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//     }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END


// #define ASSERT_FALSE(x)                                                        \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (x) {                                                                   \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : false\n");                                    \
//             CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define ASSERT_EQ(x, y)     CSTL_ASSERT(x, y, ==)
// #define ASSERT_NE(x, y)     CSTL_ASSERT(x, y, !=)
// #define ASSERT_LT(x, y)     CSTL_ASSERT(x, y, <)
// #define ASSERT_LE(x, y)     CSTL_ASSERT(x, y, <=)
// #define ASSERT_GT(x, y)     CSTL_ASSERT(x, y, >)
// #define ASSERT_GE(x, y)     CSTL_ASSERT(x, y, >=)

// #define ASSERT_STREQ(x, y)                                                     \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 != strcmp(x, y)) {                                                   \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
//             CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define ASSERT_STRNE(x, y)                                                     \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 == strcmp(x, y)) {                                                   \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
//             CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define ASSERT_STRNEQ(x, y, n)                                                 \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 != CSTL_STRNCMP(x, y, n)) {                                         \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
//             CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define ASSERT_STRNNE(x, y, n)                                                 \
//     CSTL_SUPPRESS_WARNING_BEGIN 
//     do {                                            \
//         if (0 == CSTL_STRNCMP(x, y, n)) {                                         \
//             CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//             CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
//             CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
//             *cstl_utest_result = 1;                                                       \
//             return;                                                                  \
//         }                                                                          \
//     }                                                                            \
//     while (0)                                                                    \
//     CSTL_SUPPRESS_WARNING_END

// #define TEST(SET, NAME)                                                       \
//     CSTL_EXTERN struct cstl_utest_state_s cstl_utest_state;                               \
//     static void cstl_utest_run_##SET##_##NAME(int* cstl_utest_result);                     \
//     static void cstl_utest_##SET##_##NAME(int* cstl_utest_result, Ll cstl_utest_index) {    \
//         (void)cstl_utest_index;                                                         \
//         cstl_utest_run_##SET##_##NAME(cstl_utest_result);                                    \
//     }                                        
//                                         \
//     CSTL_INITIALIZER(cstl_utest_register_##SET##_##NAME) {                           \
//         const Ll index = cstl_utest_state.tests_length++;                           \
//         const char* name_part = #SET "." #NAME;                                    \
//         const Ll name_size = strlen(name_part) + 1;                            \
//         char* name = CSTL_PTR_CAST(char* , malloc(name_size));                    \
//         cstl_utest_state.tests = CSTL_PTR_CAST(                                        \
//             struct cstl_utest_test_state_s *,                                           \
//             cstl_utest_realloc(CSTL_PTR_CAST(void* , cstl_utest_state.tests),               \
//                           sizeof(struct cstl_utest_test_state_s) *                      \
//                           cstl_utest_state.tests_length));                          \
//         cstl_utest_state.tests[index].func = &cstl_utest_##SET##_##NAME;                     \
//         cstl_utest_state.tests[index].name = name;                                      \
//         cstl_utest_state.tests[index].index = 0;                                        \
//         CSTL_SNPRINTF(name, name_size, "%s", name_part);                          \
//     }                                                                            \
//     void cstl_utest_run_##SET##_##NAME(int* cstl_utest_result)


// // 
// // ====================================================================================================================
// // ================================================ MAYBE NOT REQUIRED ================================================
// // ====================================================================================================================
// // 

// #define CSTL_F_TESTSETUP(FIXTURE)                                                 \
//     static void cstl_utest_f_setup_##FIXTURE(int* cstl_utest_result,                       \
//                                         struct FIXTURE *cstl_utest_fixture)

// #define CSTL_F_TEARDOWN(FIXTURE)                                              \
//     static void cstl_utest_f_teardown_##FIXTURE(int* cstl_utest_result,                    \
//                                             struct FIXTURE *cstl_utest_fixture)

// #define CSTL_F(FIXTURE, NAME)                                                 \
//     CSTL_EXTERN struct cstl_utest_state_s cstl_utest_state;                               \
//     static void cstl_utest_f_setup_##FIXTURE(int* , struct FIXTURE *);                \
//     static void cstl_utest_f_teardown_##FIXTURE(int* , struct FIXTURE *);             \
//     static void cstl_utest_run_##FIXTURE##_##NAME(int* , struct FIXTURE *);           \
//     static void cstl_utest_f_##FIXTURE##_##NAME(int* cstl_utest_result,                    \
//                                             Ll cstl_utest_index) {                 \
//         struct FIXTURE fixture;                                                    \
//         (void)cstl_utest_index;                                                         \
//         memset(&fixture, 0, sizeof(fixture));                                      \
//         cstl_utest_f_setup_##FIXTURE(cstl_utest_result, &fixture);                           \
//         if (0 != *cstl_utest_result) {                                                  \
//             return;                                                                  \
//         }                                                                          \
//         cstl_utest_run_##FIXTURE##_##NAME(cstl_utest_result, &fixture);                      \
//         cstl_utest_f_teardown_##FIXTURE(cstl_utest_result, &fixture);                        \
//     }                                                                            \
//     CSTL_INITIALIZER(cstl_utest_register_##FIXTURE##_##NAME) {                       \
//         const Ll index = cstl_utest_state.tests_length++;                           \
//         const char* name_part = #FIXTURE "." #NAME;                                \
//         const Ll name_size = strlen(name_part) + 1;                            \
//         char* name = CSTL_PTR_CAST(char* , malloc(name_size));                    \
//         cstl_utest_state.tests = CSTL_PTR_CAST(                                        \
//             struct cstl_utest_test_state_s *,                                           \
//             cstl_utest_realloc(CSTL_PTR_CAST(void* , cstl_utest_state.tests),               \
//                           sizeof(struct cstl_utest_test_state_s) *                      \
//                           cstl_utest_state.tests_length));                          \
//         cstl_utest_state.tests[index].func = &cstl_utest_f_##FIXTURE##_##NAME;               \
//         cstl_utest_state.tests[index].name = name;                                      \
//         CSTL_SNPRINTF(name, name_size, "%s", name_part);                          \
//     }                                                                            \
//     void cstl_utest_run_##FIXTURE##_##NAME(int* cstl_utest_result,                         \
//                                       struct FIXTURE *cstl_utest_fixture)

// #define CSTL_I_SETUP(FIXTURE)                                                 \
//     static void cstl_utest_i_setup_##FIXTURE(                                         \
//         int* cstl_utest_result, struct FIXTURE *cstl_utest_fixture, Ll cstl_utest_index)

// #define CSTL_I_TEARDOWN(FIXTURE)                                              \
//     static void cstl_utest_i_teardown_##FIXTURE(                                      \
//         int* cstl_utest_result, struct FIXTURE *cstl_utest_fixture, Ll cstl_utest_index)

// #define CSTL_I(FIXTURE, NAME, INDEX)                                          \
//     CSTL_EXTERN struct cstl_utest_state_s cstl_utest_state;                               \
//     static void cstl_utest_run_##FIXTURE##_##NAME##_##INDEX(int* , struct FIXTURE *); \
//     static void cstl_utest_i_##FIXTURE##_##NAME##_##INDEX(int* cstl_utest_result, Ll index) {             \
//         struct FIXTURE fixture;                                                    \
//         memset(&fixture, 0, sizeof(fixture));                                      \
//         cstl_utest_i_setup_##FIXTURE(cstl_utest_result, &fixture, index);                    \
//         if (0 != *cstl_utest_result) {                                                  \
//             return;                                                                  \
//         }                                                                          \
//         cstl_utest_run_##FIXTURE##_##NAME##_##INDEX(cstl_utest_result, &fixture);            \
//         cstl_utest_i_teardown_##FIXTURE(cstl_utest_result, &fixture, index);                 \
//     }                                                                            \
//     CSTL_INITIALIZER(cstl_utest_register_##FIXTURE##_##NAME##_##INDEX) {             \
//         Ll i;                                                                  \
//         UInt64 iUp;                                                        \
//         for (i = 0; i < (INDEX); i++) {                                            \
//             const Ll index = cstl_utest_state.tests_length++;                         \
//             const char* name_part = #FIXTURE "." #NAME;                              \
//             const Ll name_size = strlen(name_part) + 32;                         \
//             char* name = CSTL_PTR_CAST(char* , malloc(name_size));                  \
//             cstl_utest_state.tests = CSTL_PTR_CAST(                                      \
//                 struct cstl_utest_test_state_s *,                                         \
//                 cstl_utest_realloc(CSTL_PTR_CAST(void* , cstl_utest_state.tests),             \
//                             sizeof(struct cstl_utest_test_state_s) *                    \
//                                 cstl_utest_state.tests_length));                        \
//             cstl_utest_state.tests[index].func = &cstl_utest_i_##FIXTURE##_##NAME##_##INDEX;   \
//             cstl_utest_state.tests[index].index = i;                                      \
//             cstl_utest_state.tests[index].name = name;                                    \
//             iUp = CSTL_CAST(UInt64, i);                                     \
//             CSTL_SNPRINTF(name, name_size, "%s/%" CSTL_PRIu64, name_part, iUp);    \
//         }                                                                          \
//     }                                                                            \
//     void cstl_utest_run_##FIXTURE##_##NAME##_##INDEX(int* cstl_utest_result,               \
//                                                 struct FIXTURE *cstl_utest_fixture)

// // 
// // ====================================================================================================================
// // ================================================ MAYBE NOT REQUIRED ================================================
// // ====================================================================================================================
// // 

// CSTL_WEAK int cstl_utest_should_filter_test(const char* filter, const char* testcase);
// CSTL_WEAK int cstl_utest_should_filter_test(const char* filter, const char* testcase) {
//     if (filter) {
//         const char* filter_cur = filter;
//         const char* testcase_cur = testcase;
//         const char* filter_wildcard = null;

//         while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
//             if ('*' == *filter_cur) {
//             /* store the position of the wildcard */
//             filter_wildcard = filter_cur;

//             /* skip the wildcard character */
//             filter_cur++;

//             while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
//                 if ('*' == *filter_cur) {
//                 /*
//                     we found another wildcard (filter is something like *foo*) so we
//                     exit the current loop, and return to the parent loop to handle
//                     the wildcard case
//                 */
//                 break;
//                 } else if (*filter_cur != *testcase_cur) {
//                     /* otherwise our filter didn't match, so reset it */
//                     filter_cur = filter_wildcard;
//                 }

//                 /* move testcase along */
//                 testcase_cur++;

//                 /* move filter along */
//                 filter_cur++;
//             }

//             if (('\0' == *filter_cur) && ('\0' == *testcase_cur)) {
//                 return 0;
//             }

//             /* if the testcase has been exhausted, we don't have a match! */
//             if ('\0' == *testcase_cur) {
//                 return 1;
//             }
//             } else {
//                 if (*testcase_cur != *filter_cur) {
//                     /* test case doesn't match filter */
//                     return 1;
//                 } else {
//                     /* move our filter and testcase forward */
//                     testcase_cur++;
//                     filter_cur++;
//                 }
//             }
//         }

//         if (('\0' != *filter_cur) || (('\0' != *testcase_cur) && ((filter == filter_cur) || ('*' != filter_cur[-1])))) {
//             /* we have a mismatch! */
//             return 1;
//         }
//     }
//     return 0;
// }

// static inline FILE* cstl_readFile(const char* filename, const char* mode) {
//     #ifdef _MSC_VER
//         FILE* file;
//         if (0 == fopen_s(&file, filename, mode)) {
//             return file;
//         } else {
//             return null;
//         }
//     #else
//         return fopen(filename, mode);
//     #endif
// }

// static inline int cstl_utest_main(int argc, const char* const argv[]);
// int cstl_utest_main(int argc, const char* const argv[]) {
//     UInt64 failed = 0;
//     Ll index = 0;
//     Ll* failed_testcases = null;
//     Ll failed_testcases_length = 0;
//     const char* filter = null;
//     UInt64 ran_tests = 0;

//     enum colours { RESET, GREEN, RED };

//     const int use_colours = CSTL_COLOUR_OUTPUT();
//     const char* colours[] = {"\033[0m", "\033[32m", "\033[31m"};
//     if (!use_colours) {
//         for (index = 0; index < sizeof colours / sizeof colours[0]; index++) {
//             colours[index] = "";
//         }
//     }
//     /* loop through all arguments looking for our options */
//     for (index = 1; index < CSTL_CAST(Ll, argc); index++) {
//         /* Informational switches */
//         const char help_str[] = "--help";
//         const char list_str[] = "--list-tests";
//         /* Test config switches */
//         const char filter_str[] = "--filter=";
//         const char output_str[] = "--output=";

//         if (CSTL_STRNCMP(argv[index], help_str, strlen(help_str)) == 0) {
//             printf("cstl_utest.h - the single file unit testing solution for C/C++!\n"
//                     "Command line Options:\n"
//                     "  --help            Show this message and exit.\n"
//                     "  --filter=<filter> Filter the test cases to run (EG. MyTest*.a "
//                     "would run MyTestCase.a but not MyTestCase.b).\n"
//                     "  --list-tests      List testnames, one per line. Output names "
//                     "can be passed to --filter.\n"
//                     "  --output=<output> Output an xunit XML file to the file "
//                     "specified in <output>.\n");
//             goto cleanup;
//         } else if (CSTL_STRNCMP(argv[index], filter_str, strlen(filter_str)) == 0) {
//             /* user wants to filter what test cases run! */
//             filter = argv[index] + strlen(filter_str);
//         } else if (CSTL_STRNCMP(argv[index], output_str, strlen(output_str)) == 0) {
//             cstl_utest_state.output = cstl_readFile(argv[index] + strlen(output_str), "w+");
//         } else if (CSTL_STRNCMP(argv[index], list_str, strlen(list_str)) == 0) {
//             for (index = 0; index < cstl_utest_state.tests_length; index++) {
//                 CSTL_PRINTF("%s\n", cstl_utest_state.tests[index].name);
//             }
//             /* when printing the test list, don't actually run the tests */
//             return 0;
//         }
//     }

//     for (index = 0; index < cstl_utest_state.tests_length; index++) {
//         if (cstl_utest_should_filter_test(filter, cstl_utest_state.tests[index].name)) {
//             continue;
//         }
//         ran_tests++;
//     }

//     printf("%s[==========]%s Running %" CSTL_PRIu64 " test cases.\n",
//             colours[GREEN], colours[RESET], CSTL_CAST(UInt64, ran_tests));

//     if (cstl_utest_state.output) {
//         fprintf(cstl_utest_state.output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
//         fprintf(cstl_utest_state.output,
//                 "<testsuites tests=\"%" CSTL_PRIu64 "\" name=\"All\">\n",
//                 CSTL_CAST(UInt64, ran_tests));
//         fprintf(cstl_utest_state.output,
//                 "<testsuite name=\"Tests\" tests=\"%" CSTL_PRIu64 "\">\n",
//                 CSTL_CAST(UInt64, ran_tests));
//     }

//     for (index = 0; index < cstl_utest_state.tests_length; index++) {
//         int result = 0;
//         Int64 ns = 0;

//         if (cstl_utest_should_filter_test(filter, cstl_utest_state.tests[index].name)) {
//             continue;
//         }

//         printf("%s[ RUN      ]%s %s\n", colours[GREEN], colours[RESET],
//                 cstl_utest_state.tests[index].name);

//         if (cstl_utest_state.output) {
//             fprintf(cstl_utest_state.output, "<testcase name=\"%s\">",
//                     cstl_utest_state.tests[index].name);
//         }

//         ns = cstl_utest_ns();
//         errno = 0;
//         cstl_utest_state.tests[index].func(&result, cstl_utest_state.tests[index].index);
//         ns = cstl_utest_ns() - ns;

//         if (cstl_utest_state.output) {
//             fprintf(cstl_utest_state.output, "</testcase>\n");
//         }

//         if (result != 0) {
//             const Ll failed_testcase_index = failed_testcases_length++;
//             failed_testcases = CSTL_PTR_CAST(
//                 Ll* , cstl_utest_realloc(CSTL_PTR_CAST(void* , failed_testcases),
//                                         sizeof(Ll) * failed_testcases_length));
//             failed_testcases[failed_testcase_index] = index;
//             failed++;
//             printf("%s[  FAILED  ]%s %s (%" CSTL_PRId64 "ns)\n", colours[RED],
//                     colours[RESET], cstl_utest_state.tests[index].name, ns);
//         } else {
//             printf("%s[       OK ]%s %s (%" CSTL_PRId64 "ns)\n", colours[GREEN],
//                     colours[RESET], cstl_utest_state.tests[index].name, ns);
//         }
//     }

//     printf("%s[==========]%s %" CSTL_PRIu64 " test cases ran.\n", colours[GREEN],
//             colours[RESET], ran_tests);
//     printf("%s[  PASSED  ]%s %" CSTL_PRIu64 " tests.\n", colours[GREEN],
//             colours[RESET], ran_tests - failed);

//     if (0 != failed) {
//         printf("%s[  FAILED  ]%s %" CSTL_PRIu64 " tests, listed below:\n",
//                 colours[RED], colours[RESET], failed);
//         for (index = 0; index < failed_testcases_length; index++) {
//             printf("%s[  FAILED  ]%s %s\n", colours[RED], colours[RESET],
//                     cstl_utest_state.tests[failed_testcases[index]].name);
//         }
//     }

//     if (cstl_utest_state.output) {
//         fprintf(cstl_utest_state.output, "</testsuite>\n</testsuites>\n");
//     }

// cleanup:
//     for (index = 0; index < cstl_utest_state.tests_length; index++) {
//         free(CSTL_PTR_CAST(void* , cstl_utest_state.tests[index].name));
//     }

//     free(CSTL_PTR_CAST(void* , failed_testcases));
//     free(CSTL_PTR_CAST(void* , cstl_utest_state.tests));

//     if (cstl_utest_state.output) {
//         fclose(cstl_utest_state.output);
//     }

//     return CSTL_CAST(int, failed);
// }

// /*
//   define a main() function to call into cstl_utest.h and start executing tests! A
//   user can optionally not use this macro, and instead define their own main()
//   function and manually call cstl_utest_main. The user must, in exactly one source
//   file, use the CSTL_STATE macro to declare a global struct variable that
//   cstl_utest requires.
// */
// #define CSTL_MAIN()   
//     // we need, in exactly one source file, define the global struct that will hold
//     // the data we need to run cstl_utest. 
//     // This macro allows the user to declare the data without having to use the CSTL_MAIN macro, thus allowing them to write
//     // their own main() function.                               \                         
//     struct cstl_utest_state_s cstl_utest_state = {0, 0, 0}                                                           \
//     int main(int argc, const char* const argv[]) {                               \
//         return cstl_utest_main(argc, argv);                                             \
//     }

// #endif // CSTL_UTEST_H

// ##########################################################################################################################
// ##########################################################################################################################
// ##########################################################################################################################
// ##########################################################################################################################
// ##########################################################################################################################
// ##########################################################################################################################
// ##########################################################################################################################
// ##########################################################################################################################

/*
    /\    |  __ \|  __ \ / __ \| |  | |
   /  \   | |__) | |__) | |  | | |  | |
  / /\ \  |  _  /|  _  /| |  | | |/\| |
 / ____ \ | | \ \| | \ \| |__| | /  \ |
/_/    \_\|_|__\_|_|__\__\____/|_ /\ _|


Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef CSTL_UTEST_H
#define CSTL_UTEST_H

#include <Hazel/Core/Types.h>

#ifdef _MSC_VER
    /*
        Disable warning about not inlining 'inline' functions.
    */
    #pragma warning(disable : 4710)

    /*
        Disable warning about inlining functions that are not marked 'inline'.
    */
    #pragma warning(disable : 4711)
    #pragma warning(push, 1)
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#if defined(__cplusplus)
    #define CSTL_C_FUNC extern "C"
#else
    #define CSTL_C_FUNC
#endif

#ifdef _MSC_VER
    // define CSTL_USE_OLD_QPC before #include "cstl_utest.h" to use old QueryPerformanceCounter
    #ifndef CSTL_USE_OLD_QPC
        #pragma warning(push, 0)
        #include <Windows.h>
        #pragma warning(pop)

        typedef LARGE_INTEGER LargeInt;
    #else 
        //use old QueryPerformanceCounter definitions (not sure is this needed in some edge cases or not)
        //on Win7 with VS2015 these extern declaration cause "second C linkage of overloaded function not allowed" error
        typedef union {
            struct {
                unsigned long LowPart;
                long HighPart;
            } DUMMYSTRUCTNAME;
            struct {
                unsigned long LowPart;
                long HighPart;
            } u;
            Int64 QuadPart;
        } LargeInt;

        CSTL_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(
        LargeInt *);
        CSTL_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(
        LargeInt *);
    #endif

#elif defined(__linux__)
    /*
    slightly obscure include here - we need to include glibc's features.h, but
    we don't want to just include a header that might not be defined for other
    c libraries like musl. Instead we include limits.h, which we know on all
    glibc distributions includes features.h
    */
    #include <limits.h>

    #if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
    #include <time.h>

    #if ((2 < __GLIBC__) || ((2 == __GLIBC__) && (17 <= __GLIBC_MINOR__)))
    /* glibc is version 2.17 or above, so we can just use clock_gettime */
    #define CSTL_USE_CLOCKGETTIME
    #else
    #include <sys/syscall.h>
    #include <unistd.h>
    #endif
    #else // Other libc implementations
    #include <time.h>
    #define CSTL_USE_CLOCKGETTIME
    #endif

#elif defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1920)
    #define CSTL_PRId64     "I64d"
    #define CSTL_PRIu64     "I64u"
#else
    #include <inttypes.h>

    #define CSTL_PRId64     PRId64
    #define CSTL_PRIu64     PRIu64
#endif

#ifdef _MSC_VER
    #ifdef _WIN64
        #define CSTL_SYMBOL_PREFIX
    #else
        #define CSTL_SYMBOL_PREFIX "_"
    #endif

    #ifdef __clang__
        #define CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
            _Pragma("clang diagnostic push")                                          \
            _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")

        #define CSTL_INITIALIZER_END_DISABLE_WARNINGS \
            _Pragma("clang diagnostic pop")
    #else
        #define CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS
        #define CSTL_INITIALIZER_END_DISABLE_WARNINGS
    #endif

    #pragma section(".CRT$XCU", read)
    #define CSTL_INITIALIZER(f)                                                   \
        static void __cdecl f(void);                                                 \
        CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS                                     \
            __pragma(comment(linker, "/include:" CSTL_SYMBOL_PREFIX #f "_"))            \
            CSTL_C_FUNC __declspec(allocate(".CRT$XCU")) void(__cdecl * f##_)(void) = f;      \
        CSTL_INITIALIZER_END_DISABLE_WARNINGS                                       \
        static void __cdecl f(void)

#else
#ifdef __linux__
    #ifdef __clang__
        #if __has_warning("-Wreserved-id-macro")
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #endif // __clang__
#endif __linux__

#define __STDC_FORMAT_MACROS 1

#ifdef __clang__
    #if __has_warning("-Wreserved-id-macro")
    #pragma clang diagnostic pop
    #endif
#endif // __clang__
#endif

#define CSTL_INITIALIZER(f)                                                   \
    static void f(void) __attribute__((constructor));                            \
    static void f(void)

#endif // _MSC_VER

#ifdef __cplusplus
    #define CSTL_CAST(type, x)      static_cast<type>(x)
    #define CSTL_PTR_CAST(type, x)  reinterpret_cast<type>(x)
    #define CSTL_EXTERN             extern "C"
#else
    #define CSTL_CAST(type, x)     ((type)x)
    #define CSTL_PTR_CAST(type, x) ((type)x)
    #define CSTL_EXTERN            extern
#endif // __cplusplus


#ifdef _MSC_VER
    /*
    io.h contains definitions for some structures with natural padding. This is
    uninteresting, but for some reason MSVC's behaviour is to warn about
    including this system header. That *is* interesting
    */
    #pragma warning(disable : 4820)
    #pragma warning(push, 1)
    #include <io.h>
    #pragma warning(pop)
    #define CSTL_COLOUR_OUTPUT() (_isatty(_fileno(stdout)))
#else
    #include <unistd.h>
    #define CSTL_COLOUR_OUTPUT()    (isatty(STDOUT_FILENO))
#endif

static inline void* cstl_utest_realloc(void* const pointer, Ll new_size) {
    void* const new_pointer = realloc(pointer, new_size);

    if (new_pointer == null) {
        free(new_pointer);
    }

    return new_pointer;
}

static inline Int64 cstl_utest_ns(void) {
    #ifdef _MSC_VER
        LargeInt counter;
        LargeInt frequency;
        QueryPerformanceCounter(&counter);
        QueryPerformanceFrequency(&frequency);
        return CSTL_CAST(Int64,
                        (counter.QuadPart * 1000000000) / frequency.QuadPart);
    #elif defined(__linux) && defined(__STRICT_ANSI__)
        return CSTL_CAST(Int64, clock()) * 1000000000 / CLOCKS_PER_SEC;
    #elifdef __linux
        struct timespec ts;
        #ifdef __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
            timespec_get(&ts, TIME_UTC);
        #else
            const clockid_t cid = CLOCK_REALTIME;

        #ifdef CSTL_USE_CLOCKGETTIME
            clock_gettime(cid, &ts);
        #else
            syscall(SYS_clock_gettime, cid, &ts);
        #endif // CSTL_USE_CLOCKGETTIME

        #endif
        return CSTL_CAST(Int64, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec;
    #elif __APPLE__
        return CSTL_CAST(Int64, mach_absolute_time());
    #endif
}

typedef void (*cstl_utest_testcase_t)(int* , Ll);

struct cstl_utest_test_state_s {
    cstl_utest_testcase_t func;
    Ll index;
    char* name;
};

struct cstl_utest_state_s {
    struct cstl_utest_test_state_s *tests;
    Ll tests_length;
    FILE* output;
};

/* extern to the global state cstl_utest needs to execute */
CSTL_EXTERN struct cstl_utest_state_s cstl_utest_state;

#ifdef _MSC_VER
    #define CSTL_WEAK inline
    #define CSTL_UNUSED
#else
    #define CSTL_WEAK __attribute__((weak))
    #define CSTL_UNUSED __attribute__((unused))
#endif // _MSC_VER


#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wvariadic-macros"
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif // __clang__

#define CSTL_PRINTF(...)                                                      \
    if (cstl_utest_state.output) {                                                    \
        fprintf(cstl_utest_state.output, __VA_ARGS__);                                  \
    }                                                                            \
    printf(__VA_ARGS__)

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wvariadic-macros"
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#ifdef _MSC_VER
    #define CSTL_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
    #define CSTL_SNPRINTF(...) snprintf(__VA_ARGS__)
#endif

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#if __cplusplus
    /* if we are using c++ we can use overloaded methods (its in the language) */
    #define CSTL_OVERLOADABLE

#elif __clang__
    /* otherwise, if we are using clang with c - use the overloadable attribute */
    #define CSTL_OVERLOADABLE __attribute__((overloadable))
#endif // __cplusplus

#if CSTL_OVERLOADABLE
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(float f);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(float f) {
        CSTL_PRINTF("%f", CSTL_CAST(double, f));
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(double d);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(double d) {
        CSTL_PRINTF("%f", d);
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long double d);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long double d) {
        CSTL_PRINTF("%Lf", d);
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(int i);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(int i) {
        CSTL_PRINTF("%d", i);
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(unsigned int i);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(unsigned int i) {
        CSTL_PRINTF("%u", i);
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long int i);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long int i) {
        CSTL_PRINTF("%ld", i);
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long unsigned int i);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long unsigned int i) {
        CSTL_PRINTF("%lu", i);
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(const void* p);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(const void* p) {
        CSTL_PRINTF("%p", p);
    }

    /*
    long long is a c++11 extension
    */
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)

        #ifdef __clang__
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
        #endif

        CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long long int i);
        CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(long long int i) {
            CSTL_PRINTF("%lld", i);
        }

        CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(Ull int i);
        CSTL_WEAK CSTL_OVERLOADABLE void
        cstl_utest_type_printer(Ull int i) {
            CSTL_PRINTF("%llu", i);
        }

        #ifdef __clang__
            #pragma clang diagnostic pop
        #endif

    #endif

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)

    #define cstl_utest_type_printer(val)                                                \
        CSTL_PRINTF(_Generic((val), signed char                                     \
                        : "%d", unsigned char                                  \
                        : "%u", short                                          \
                        : "%d", unsigned short                                 \
                        : "%u", int                                            \
                        : "%d", long                                           \
                        : "%ld", long long                                     \
                        : "%lld", unsigned                                     \
                        : "%u", unsigned long                                  \
                        : "%lu", unsigned long long                            \
                        : "%llu", float                                        \
                        : "%f", double                                         \
                        : "%f", long double                                    \
                        : "%Lf", default                                       \
                        : _Generic((val - val), ptrdiff_t                      \
                                    : "%p", default                             \
                                    : "undef")),                                \
                (val))
#else
/*
    we don't have the ability to print the values we got, so we create a macro
    to tell our users we can't do anything fancy
    */
    #define cstl_utest_type_printer(...) CSTL_PRINTF("undef")
#endif

#ifdef _MSC_VER
    #define CSTL_SUPPRESS_WARNING_BEGIN                                           \
    __pragma(warning(push)) __pragma(warning(disable : 4127))
    #define CSTL_SUPPRESS_WARNING_END __pragma(warning(pop))
#else
    #define CSTL_SUPPRESS_WARNING_BEGIN
    #define CSTL_SUPPRESS_WARNING_END
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define CSTL_AUTO(x) auto

#elif !defined(__cplusplus)

    #if defined(__clang__)
        /* clang-format off */
        /* had to disable clang-format here because it malforms the pragmas */
        #define CSTL_AUTO(x)                                                          \
        _Pragma("clang diagnostic push")                                             \
            _Pragma("clang diagnostic ignored \"-Wgnu-auto-type\"") __auto_type      \
                _Pragma("clang diagnostic pop")
        /* clang-format on */
    #else
        #define CSTL_AUTO(x) __typeof__(x + 0)
    #endif

#else
    #define CSTL_AUTO(x) typeof(x + 0)
#endif

#if defined(__clang__)
    #define CSTL_STRNCMP(x, y, size)                                              \
        _Pragma("clang diagnostic push")                                             \
        _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"")       \
            strncmp(x, y, size) _Pragma("clang diagnostic pop")
#else
    #define CSTL_STRNCMP(x, y, size) strncmp(x, y, size)
#endif

#if defined(__clang__)
    #define CSTL_EXPECT(x, y, cond)                                               \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    _   Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
        _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
        _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
        CSTL_AUTO(x) xEval = (x);                                 \
    CSTL_AUTO(y) yEval = (y);                                                 \
                                                                                \
    if (!((xEval)cond(yEval))) {                                               \
        _Pragma("clang diagnostic pop")                                          \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                \
        CSTL_PRINTF("  Expected : ");                                           \
        cstl_utest_type_printer(xEval);                                               \
        CSTL_PRINTF("\n");                                                      \
        CSTL_PRINTF("    Actual : ");                                           \
        cstl_utest_type_printer(yEval);                                               \
        CSTL_PRINTF("\n");                                                      \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END
#elif defined(__GNUC__)
    #define CSTL_EXPECT(x, y, cond)                                               \
        CSTL_SUPPRESS_WARNING_BEGIN \
        do {                                            \
            const int use_colours = CSTL_COLOUR_OUTPUT();                \
            const char* colours[] = {"\033[0m", "\033[32m", "\033[31m"}; \
            if (!use_colours) { \
                for (index = 0; index < sizeof colours / sizeof colours[0]; index++) { \
                    colours[index] = ""; \
                } \
            } \
            CSTL_AUTO(x) xEval = (x);                                                 \
            CSTL_AUTO(y) yEval = (y);                                                 \
            if (!((xEval)cond(yEval))) {                                               \
                CSTL_PRINTF("%s:%s%u%s: Failure\n", __FILE__, colours[RED], __LINE__);                    \
                CSTL_PRINTF("  Expected : ");                                           \
                cstl_utest_type_printer(xEval);                                               \
                CSTL_PRINTF("\n");                                                      \
                CSTL_PRINTF("    Actual : ");                                           \
                cstl_utest_type_printer(yEval);                                               \
                CSTL_PRINTF("\n");                                                      \
                *cstl_utest_result = 1;                                                       \
            }                                                                          \
        }                                                                            \
        while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END
#else
    #define CSTL_EXPECT(x, y, cond)                                               \
        CSTL_SUPPRESS_WARNING_BEGIN \
        do {                                            \
        if (!((x)cond(y))) {                                                       \
            CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            *cstl_utest_result = 1;                                                       \
        }                                                                          \
        }                                                                            \
        while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END
#endif

#define EXPECT_TRUE(x)                                                         \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (!(x)) {                                                                \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : true\n");                                     \
        CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define EXPECT_FALSE(x)                                                        \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (x) {                                                                   \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : false\n");                                    \
        CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define EXPECT_EQ(x, y)     CSTL_EXPECT(x, y, ==)
#define EXPECT_NE(x, y)     CSTL_EXPECT(x, y, !=)
#define EXPECT_LT(x, y)     CSTL_EXPECT(x, y, <)
#define EXPECT_LE(x, y)     CSTL_EXPECT(x, y, <=)
#define EXPECT_GT(x, y)     CSTL_EXPECT(x, y, >)
#define EXPECT_GE(x, y)     CSTL_EXPECT(x, y, >=)

#define EXPECT_STREQ(x, y)                                                     \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 != strcmp(x, y)) {                                                   \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
        CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define EXPECT_STRNE(x, y)                                                     \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 == strcmp(x, y)) {                                                   \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
        CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define EXPECT_STRNEQ(x, y, n)                                                 \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 != CSTL_STRNCMP(x, y, n)) {                                         \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
        CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define EXPECT_STRNNE(x, y, n)                                                 \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 == CSTL_STRNCMP(x, y, n)) {                                         \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
        CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
        *cstl_utest_result = 1;                                                       \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#if defined(__clang__)
    #define CSTL_ASSERT(x, y, cond)                                               \
        CSTL_SUPPRESS_WARNING_BEGIN \
        do {                                            \
        _Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
        _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
        _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
        CSTL_AUTO(x) xEval = (x);                                 \
        CSTL_AUTO(y) yEval = (y);                                                 \
        if (!((xEval)cond(yEval))) {                                               \
            _Pragma("clang diagnostic pop")                                          \
            CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                \
            CSTL_PRINTF("  Expected : ");                                           \
            cstl_utest_type_printer(xEval);                                               \
            CSTL_PRINTF("\n");                                                      \
            CSTL_PRINTF("    Actual : ");                                           \
            cstl_utest_type_printer(yEval);                                               \
            CSTL_PRINTF("\n");                                                      \
            *cstl_utest_result = 1;                                                       \
            return;                                                                  \
        }                                                                          \
        }                                                                            \
        while (0)                                                                    \
        CSTL_SUPPRESS_WARNING_END

#elif defined(__GNUC__)

#define CSTL_ASSERT(x, y, cond)                                               \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    CSTL_AUTO(x) xEval = (x);                                                 \
    CSTL_AUTO(y) yEval = (y);                                                 \
    if (!((xEval)cond(yEval))) {                                               \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : ");                                           \
        cstl_utest_type_printer(xEval);                                               \
        CSTL_PRINTF("\n");                                                      \
        CSTL_PRINTF("    Actual : ");                                           \
        cstl_utest_type_printer(yEval);                                               \
        CSTL_PRINTF("\n");                                                      \
        *cstl_utest_result = 1;                                                       \
        return;                                                                  \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#else
    #define CSTL_ASSERT(x, y, cond)                                               \
        CSTL_SUPPRESS_WARNING_BEGIN \
        do {                                            \
        if (!((x)cond(y))) {                                                       \
            CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            *cstl_utest_result = 1;                                                       \
            return;                                                                  \
        }                                                                          \
        }                                                                            \
        while (0)                                                                    \
        CSTL_SUPPRESS_WARNING_END
#endif // __clang__

#define ASSERT_TRUE(x)                                                         \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (!(x)) {                                                                \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : true\n");                                     \
        CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
        *cstl_utest_result = 1;                                                       \
        return;                                                                  \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END


#define ASSERT_FALSE(x)                                                        \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (x) {                                                                   \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : false\n");                                    \
        CSTL_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
        *cstl_utest_result = 1;                                                       \
        return;                                                                  \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define ASSERT_EQ(x, y)     CSTL_ASSERT(x, y, ==)
#define ASSERT_NE(x, y)     CSTL_ASSERT(x, y, !=)
#define ASSERT_LT(x, y)     CSTL_ASSERT(x, y, <)
#define ASSERT_LE(x, y)     CSTL_ASSERT(x, y, <=)
#define ASSERT_GT(x, y)     CSTL_ASSERT(x, y, >)
#define ASSERT_GE(x, y)     CSTL_ASSERT(x, y, >=)

#define ASSERT_STREQ(x, y)                                                     \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
        if (0 != strcmp(x, y)) {                                                   \
            CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
            CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
            *cstl_utest_result = 1;                                                       \
            return;                                                                  \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define ASSERT_STRNE(x, y)                                                     \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 == strcmp(x, y)) {                                                   \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%s\"\n", x);                                \
        CSTL_PRINTF("    Actual : \"%s\"\n", y);                                \
        *cstl_utest_result = 1;                                                       \
        return;                                                                  \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define ASSERT_STRNEQ(x, y, n)                                                 \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 != CSTL_STRNCMP(x, y, n)) {                                         \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
        CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
        *cstl_utest_result = 1;                                                       \
        return;                                                                  \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define ASSERT_STRNNE(x, y, n)                                                 \
    CSTL_SUPPRESS_WARNING_BEGIN \
    do {                                            \
    if (0 == CSTL_STRNCMP(x, y, n)) {                                         \
        CSTL_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
        CSTL_PRINTF("  Expected : \"%.*s\"\n", CSTL_CAST(int, n), x);          \
        CSTL_PRINTF("    Actual : \"%.*s\"\n", CSTL_CAST(int, n), y);          \
        *cstl_utest_result = 1;                                                       \
        return;                                                                  \
    }                                                                          \
    }                                                                            \
    while (0)                                                                    \
    CSTL_SUPPRESS_WARNING_END

#define TEST(TESTSUITE, TESTNAME)                                                       \
    CSTL_EXTERN struct cstl_utest_state_s cstl_utest_state;                               \
    static void cstl_utest_run_##TESTSUITE##_##TESTNAME(int* cstl_utest_result);                     \
    static void cstl_utest_##TESTSUITE##_##TESTNAME(int* cstl_utest_result, Ll cstl_utest_index) {    \
        (void)cstl_utest_index;                                                         \
        cstl_utest_run_##TESTSUITE##_##TESTNAME(cstl_utest_result);                                    \
    }                                                                            \
    CSTL_INITIALIZER(cstl_utest_register_##TESTSUITE##_##TESTNAME) {                           \
        const Ll index = cstl_utest_state.tests_length++;                           \
        const char* name_part = #TESTSUITE "." #TESTNAME;                                    \
        const Ll name_size = strlen(name_part) + 1;                            \
        char* name = CSTL_PTR_CAST(char* , malloc(name_size));                    \
        cstl_utest_state.tests = CSTL_PTR_CAST(                                        \
            struct cstl_utest_test_state_s *,                                           \
            cstl_utest_realloc(CSTL_PTR_CAST(void* , cstl_utest_state.tests),               \
                          sizeof(struct cstl_utest_test_state_s) *                      \
                          cstl_utest_state.tests_length));                          \
        cstl_utest_state.tests[index].func = &cstl_utest_##TESTSUITE##_##TESTNAME;                     \
        cstl_utest_state.tests[index].name = name;                                      \
        cstl_utest_state.tests[index].index = 0;                                        \
        CSTL_SNPRINTF(name, name_size, "%s", name_part);                          \
    }                                                                            \
    void cstl_utest_run_##TESTSUITE##_##TESTNAME(int* cstl_utest_result)


static inline FILE* cstl_readFile(const char* filename, const char* mode) {
    #ifdef _MSC_VER
        FILE* file;
        if (fopen_s(&file, filename, mode) == 0) {
            return file;
        } else {
            return null;
        }
    #else
        return fopen(filename, mode);
    #endif
}

static inline int cstl_utest_main(int argc, const char* const argv[]);
int cstl_utest_main(int argc, const char* const argv[]) {
    UInt64 failed = 0;
    Ll index = 0;
    Ll* failed_testcases = null;
    Ll failed_testcases_length = 0;
    const char* filter = null;
    UInt64 ran_tests = 0;

    enum colours { RESET, GREEN, RED };

    const int use_colours = CSTL_COLOUR_OUTPUT();
    const char* colours[] = {"\033[0m", "\033[32m", "\033[31m"};
    if (!use_colours) {
        for (index = 0; index < sizeof colours / sizeof colours[0]; index++) {
            colours[index] = "";
        }
    }

    // // 
    // // ====================================================================================================================
    // // ================================================ MAYBE NOT REQUIRED ================================================
    // // ====================================================================================================================
    // // 
    // /* loop through all arguments looking for our options */
    // for (index = 1; index < CSTL_CAST(Ll, argc); index++) {
    //     /* Informational switches */
    //     const char help_str[] = "--help";
    //     const char list_str[] = "--list-tests";
    //     /* Test config switches */
    //     const char filter_str[] = "--filter=";
    //     const char output_str[] = "--output=";

    //     if (CSTL_STRNCMP(argv[index], help_str, strlen(help_str)) == 0) {
    //         printf("cstl_utest.h - the single file unit testing solution for C/C++!\n"
    //                 "Command line Options:\n"
    //                 "  --help            Show this message and exit.\n"
    //                 "  --filter=<filter> Filter the test cases to run (EG. MyTest*.a "
    //                 "would run MyTestCase.a but not MyTestCase.b).\n"
    //                 "  --list-tests      List testnames, one per line. Output names "
    //                 "can be passed to --filter.\n"
    //                 "  --output=<output> Output an xunit XML file to the file "
    //                 "specified in <output>.\n");
    //         goto cleanup;
    //     } else if (CSTL_STRNCMP(argv[index], filter_str, strlen(filter_str)) == 0) {
    //         /* user wants to filter what test cases run! */
    //         filter = argv[index] + strlen(filter_str);
    //     } else if (CSTL_STRNCMP(argv[index], output_str, strlen(output_str)) == 0) {
    //         cstl_utest_state.output = cstl_readFile(argv[index] + strlen(output_str), "w+");
    //     } else if (CSTL_STRNCMP(argv[index], list_str, strlen(list_str)) == 0) {
    //         for (index = 0; index < cstl_utest_state.tests_length; index++) {
    //             CSTL_PRINTF("%s\n", cstl_utest_state.tests[index].name);
    //         }
    //         /* when printing the test list, don't actually run the tests */
    //         return 0;
    //     }
    // }
    // // 
    // // ====================================================================================================================
    // // ================================================ MAYBE NOT REQUIRED ================================================
    // // ====================================================================================================================
    // // 

    for (index = 0; index < cstl_utest_state.tests_length; index++) {
        // if (cstl_utest_should_filter_test(filter, cstl_utest_state.tests[index].name)) {
        //     continue;
        // }
        ran_tests++;
    }

    printf("%s[==========]%s Running %" CSTL_PRIu64 " test cases.\n",
            colours[GREEN], colours[RESET], CSTL_CAST(UInt64, ran_tests));

    if (cstl_utest_state.output) {
        fprintf(cstl_utest_state.output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(cstl_utest_state.output,
                "<testsuites tests=\"%" CSTL_PRIu64 "\" name=\"All\">\n",
                CSTL_CAST(UInt64, ran_tests));
        fprintf(cstl_utest_state.output,
                "<testsuite name=\"Tests\" tests=\"%" CSTL_PRIu64 "\">\n",
                CSTL_CAST(UInt64, ran_tests));
    }

    for (index = 0; index < cstl_utest_state.tests_length; index++) {
        int result = 0;
        Int64 ns = 0;

        // if (cstl_utest_should_filter_test(filter, cstl_utest_state.tests[index].name)) {
        //     continue;
        // }

        printf("%s[ RUN      ]%s %s\n", colours[GREEN], colours[RESET],
                cstl_utest_state.tests[index].name);

        if (cstl_utest_state.output) {
            fprintf(cstl_utest_state.output, "<testcase name=\"%s\">",
                    cstl_utest_state.tests[index].name);
        }

        ns = cstl_utest_ns();
        errno = 0;
        cstl_utest_state.tests[index].func(&result, cstl_utest_state.tests[index].index);
        ns = cstl_utest_ns() - ns;

        if (cstl_utest_state.output) {
            fprintf(cstl_utest_state.output, "</testcase>\n");
        }

        if (result != 0) {
            const Ll failed_testcase_index = failed_testcases_length++;
            failed_testcases = CSTL_PTR_CAST(
                Ll* , cstl_utest_realloc(CSTL_PTR_CAST(void* , failed_testcases),
                                        sizeof(Ll) * failed_testcases_length));
            failed_testcases[failed_testcase_index] = index;
            failed++;
            printf("%s[  FAILED  ]%s %s (%" CSTL_PRId64 "ns)\n", colours[RED],
                    colours[RESET], cstl_utest_state.tests[index].name, ns);
        } else {
            printf("%s[       OK ]%s %s (%" CSTL_PRId64 "ns)\n", colours[GREEN],
                    colours[RESET], cstl_utest_state.tests[index].name, ns);
        }
    }

    printf("%s[==========]%s %" CSTL_PRIu64 " test cases ran.\n", colours[GREEN],
            colours[RESET], ran_tests);
    printf("%s[  PASSED  ]%s %" CSTL_PRIu64 " tests.\n", colours[GREEN],
            colours[RESET], ran_tests - failed);

    if (0 != failed) {
        printf("%s[  FAILED  ]%s %" CSTL_PRIu64 " tests, listed below:\n",
                colours[RED], colours[RESET], failed);
        for (index = 0; index < failed_testcases_length; index++) {
            printf("%s[  FAILED  ]%s %s\n", colours[RED], colours[RESET],
                    cstl_utest_state.tests[failed_testcases[index]].name);
        }
    }

    if (cstl_utest_state.output) {
        fprintf(cstl_utest_state.output, "</testsuite>\n</testsuites>\n");
    }

cleanup:
    for (index = 0; index < cstl_utest_state.tests_length; index++) {
        free(CSTL_PTR_CAST(void* , cstl_utest_state.tests[index].name));
    }

    free(CSTL_PTR_CAST(void* , failed_testcases));
    free(CSTL_PTR_CAST(void* , cstl_utest_state.tests));

    if (cstl_utest_state.output) {
        fclose(cstl_utest_state.output);
    }

    return CSTL_CAST(int, failed);
} // cstl_utest_main()

/*
    Defines a main function and begins executing tests
*/
#define CSTL_MAIN()      
    // we need, in exactly one source file, define the global struct that will hold
    // the data we need to run cstl_utest. This macro allows the user to declare the
    // data without having to use the CSTL_MAIN macro, thus allowing them to write
    // their own main() function.                                                    
    struct cstl_utest_state_s cstl_utest_state = {0, 0, 0};     \
    int main(int argc, const char* const argv[]) {              \
        return cstl_utest_main(argc, argv);                     \
    }

#endif // CSTL_UTEST_H