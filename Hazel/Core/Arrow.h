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
    #define CSTL_C_FUNC     extern "C"
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
            } s;
            struct {
                unsigned long LowPart;
                long HighPart;
            } u;
            Int64 QuadPart;
        } LargeInt;

        CSTL_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(LargeInt*);
        CSTL_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(LargeInt*);
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

        #if ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 17)))
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
    #ifdef _WIN32
        // Fixes the `warning C4668: '__cplusplus' is not defined as a preprocessor macro`
        #pragma warning (disable : 4668)
    #endif
    
    #ifdef _WIN64
        #define CSTL_SYMBOL_PREFIX
    #else
        #define CSTL_SYMBOL_PREFIX      "_"
    #endif

    #ifdef __clang__
        #define CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
            _Pragma("clang diagnostic push")                                          \
            _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")

        #define CSTL_INITIALIZER_END_DISABLE_WARNINGS                                 \
            _Pragma("clang diagnostic pop")
    #else
        #define CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS
        #define CSTL_INITIALIZER_END_DISABLE_WARNINGS
    #endif // __clang__

    #pragma section(".CRT$XCU", read)
    #define CSTL_INITIALIZER(f)                                                         \
        static void __cdecl f(void);                                                    \
        CSTL_INITIALIZER_BEGIN_DISABLE_WARNINGS                                         \
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
#endif // __linux__

#define __STDC_FORMAT_MACROS 1

#ifdef __clang__
    #if __has_warning("-Wreserved-id-macro")
    #pragma clang diagnostic pop
    #endif
#endif // __clang__
#endif

#define CSTL_INITIALIZER(f)                                                   \
    static void f(void)     __attribute__((constructor));                            \
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
    #define CSTL_COLOUR_OUTPUT()    (_isatty(_fileno(stdout)))
#else
    #include <unistd.h>
    #define CSTL_COLOUR_OUTPUT()    ((int)isatty(STDOUT_FILENO))
#endif

// 
// ===============================================================
// =========================== COLOURS ===========================
// ===============================================================
//
// This will be populated in cstl_utest_main()
bool cstl_use_colour_output;
enum CSTL_COLOURS_ENUM {
    RESET = 0, 
    GREEN, 
    RED
};

static const char* CSTL_OUTPUT_COLOURS[] = {
    "\033[0m",  // RESET colours to default 
    "\033[32m", // GREEN (pass)
    "\033[31m"  // RED (failures)
};

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

    #elif defined(__linux)
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
    struct cstl_utest_test_state_s* tests;
    Ll tests_length;
    FILE* output;
};

/* extern to the global state cstl_utest needs to execute */
CSTL_EXTERN struct cstl_utest_state_s   cstl_utest_state;

#ifdef _MSC_VER
    #define CSTL_WEAK       inline
#else
    #define CSTL_WEAK       __attribute__((weak))
#endif // _MSC_VER


#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wvariadic-macros"
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif // __clang__


#define CSTL_PRINTF(...)                                    \
    if (cstl_utest_state.output) {                          \
        fprintf(cstl_utest_state.output, __VA_ARGS__);      \
    }                                                       \
    printf(__VA_ARGS__)

// Prints text green and then resets the colour
#define CSTL_PRINTF_GREEN(message)                                                      \
    CSTL_PRINTF("%s%s%s", CSTL_OUTPUT_COLOURS[GREEN], message, CSTL_OUTPUT_COLOURS[RESET])

// Prints text red and then resets the colour
#define CSTL_PRINTF_RED(message)                                                      \
    CSTL_PRINTF("%s%s%s", CSTL_OUTPUT_COLOURS[RED], message, CSTL_OUTPUT_COLOURS[RESET])


#ifdef __clang__
    // #pragma clang diagnostic pop
    // #pragma clang diagnostic push
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

// Attempting to support Function Overloading
// This is built-in to the C++ Language, but getting it to work in C requires a bit more work
#if __cplusplus
    #define CSTL_OVERLOADABLE
#elif __clang__
    // Clang has the nice overloadable attribute 
    #if __has_attribute(overloadable)
        #define CSTL_OVERLOADABLE   __attribute__((overloadable))
    #endif // __hasattribute
#endif // __cplusplus


#if CSTL_OVERLOADABLE
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(Float32 f);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(Float32 f) {
        CSTL_PRINTF("%f", CSTL_CAST(double, f));
    }

    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(Float64 d);
    CSTL_WEAK CSTL_OVERLOADABLE void cstl_utest_type_printer(Float64 d) {
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
        CSTL_PRINTF(_Generic((val),  \
                        signed char        : "%d",        \
                        unsigned char      : "%u",        \
                        short              : "%d",        \
                        unsigned short     : "%u",        \
                        int                : "%d",        \
                        long               : "%ld",       \
                        long long          : "%lld",      \
                        unsigned           : "%u",        \
                        unsigned long      : "%lu",       \
                        unsigned long long : "%llu",      \
                        float              : "%f",        \
                        double             : "%f",        \
                        long double        : "%Lf",       \
                        default            : _Generic((val - val),        \
                                                ptrdiff_t : "%p",         \
                                                default   : "undef")),    \
                    (val))
#else
/*
    we don't have the ability to print the values we got, so we create a macro
    to tell our users we can't do anything fancy
    */
    #define cstl_utest_type_printer(...)    CSTL_PRINTF("undef")
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
    #define CSTL_AUTO(x)        auto

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
        #define CSTL_AUTO(x)    __typeof__(x + 0)
    #endif

#else
    #define CSTL_AUTO(x)        typeof(x + 0)
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
    #define CSTL_CHECK(x, y, cond)                                               \
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
    #define CSTL_CHECK(x, y, cond)                                               \
        CSTL_SUPPRESS_WARNING_BEGIN \
        do {                                            \
            CSTL_AUTO(x) xEval = (x);                                                 \
            CSTL_AUTO(y) yEval = (y);                                                 \
            if (!((xEval)cond(yEval))) {                                               \
                CSTL_PRINTF("%s:%s%u%s: Failure\n", __FILE__, CSTL_OUTPUT_COLOURS[RED], __LINE__);                    \
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
    #define CSTL_CHECK(x, y, cond)                                               \
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

#define CHECK_TRUE(x)                                                         \
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

#define CHECK_FALSE(x)                                                        \
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


#define CHECK_EQ(x, y)     CSTL_CHECK(x, y, ==)
#define CHECK_NE(x, y)     CSTL_CHECK(x, y, !=)
#define CHECK_LT(x, y)     CSTL_CHECK(x, y, <)
#define CHECK_LE(x, y)     CSTL_CHECK(x, y, <=)
#define CHECK_GT(x, y)     CSTL_CHECK(x, y, >)
#define CHECK_GE(x, y)     CSTL_CHECK(x, y, >=)

#define CHECK_STREQ(x, y)                                                     \
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

#define CHECK_STRNE(x, y)                                                     \
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

#define CHECK_STRNEQ(x, y, n)                                                 \
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

#define CHECK_STRNNE(x, y, n)                                                 \
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
            \
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
                return;                                                                  \
            }                                                                          \
        }                                                                            \
        while (0)                                                                    \
        CSTL_SUPPRESS_WARNING_END

#elif defined(__GNUC__)

#define CSTL_ASSERT(x, y, cond)                                               \
    CSTL_SUPPRESS_WARNING_BEGIN                                               \ 
    do {                                                                      \
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
    \
    CSTL_INITIALIZER(cstl_utest_register_##TESTSUITE##_##TESTNAME) {                           \
    \
        const Ll index = cstl_utest_state.tests_length++;                           \
        const char* name_part = #TESTSUITE "." #TESTNAME;                                    \
        const Ll name_size = strlen(name_part) + 1;                            \
        char* name = CSTL_PTR_CAST(char* , malloc(name_size));                    \
        \
        cstl_utest_state.tests = CSTL_PTR_CAST(                                        \
            struct cstl_utest_test_state_s *,                                           \
            cstl_utest_realloc(CSTL_PTR_CAST(void* , cstl_utest_state.tests),               \
                          sizeof(struct cstl_utest_test_state_s) *                      \
                          cstl_utest_state.tests_length));                          \
                          \
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

    // This variable is false if the current system doesn't support Terminal Colours
    cstl_use_colour_output = CSTL_COLOUR_OUTPUT();
    if (!cstl_use_colour_output) {
        for(index = 0; index < sizeof(CSTL_OUTPUT_COLOURS) / sizeof(CSTL_OUTPUT_COLOURS[0]); index++) {
            CSTL_OUTPUT_COLOURS[index] = "";
        }
    }

    // For supported CLI options
    // for (index = 1; index < CSTL_CAST(Ll, argc); index++) {
    //     /* Informational switches */
    //     const char help_str[] = "--help";
    //     const char list_str[] = "--list-tests";
    //     /* Test config switches */
    //     const char filter_str[] = "--filter=";
    //     const char output_str[] = "--output=";

    //     if (CSTL_STRNCMP(argv[index], help_str, strlen(help_str)) == 0) {
    //         printf("Arrow - the Micro Unit Testing Framework for C/C++.\n"
    //                 "Usage:\n"
    //                 "       --help            Show this message and exit.\n"
    //                 "       --filter=<filter> Filter the test cases to run (EG. MyTest*.a "
    //                 "would run MyTestCase.a but not MyTestCase.b).\n"
    //                 "       --list-tests      List testnames, one per line. Output names "
    //                 "can be passed to --filter.\n"
    //                 "       --output=<output> Output an xunit XML file to the file "
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
    //         // when printing the test list, don't actually run the tests
    //         return 0;
    //     }
    // }

    for (index = 0; index < cstl_utest_state.tests_length; index++) {
        // if (utest_should_filter_test(filter, cstl_utest_state.tests[index].name)) {
        //     continue;
        // }
        ran_tests++;
    }

    printf("%s[==========]%s Running %" CSTL_PRIu64 " test cases.\n",
            CSTL_OUTPUT_COLOURS[GREEN], CSTL_OUTPUT_COLOURS[RESET], CSTL_CAST(UInt64, ran_tests));

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

        printf("%s[ RUN      ]%s %s\n", CSTL_OUTPUT_COLOURS[GREEN], CSTL_OUTPUT_COLOURS[RESET],
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
            printf("%s[  FAILED  ]%s %s (%" CSTL_PRId64 "ns)\n", CSTL_OUTPUT_COLOURS[RED],
                    CSTL_OUTPUT_COLOURS[RESET], cstl_utest_state.tests[index].name, ns);
        } else {
            printf("%s[       OK ]%s %s (%" CSTL_PRId64 "ns)\n", CSTL_OUTPUT_COLOURS[GREEN],
                    CSTL_OUTPUT_COLOURS[RESET], cstl_utest_state.tests[index].name, ns);
        }
    }

    printf("%s[==========]%s %" CSTL_PRIu64 " test cases ran.\n", CSTL_OUTPUT_COLOURS[GREEN],
            CSTL_OUTPUT_COLOURS[RESET], ran_tests);
    printf("%s[  PASSED  ]%s %" CSTL_PRIu64 " tests.\n", CSTL_OUTPUT_COLOURS[GREEN],
            CSTL_OUTPUT_COLOURS[RESET], ran_tests - failed);

    if (0 != failed) {
        printf("%s[  FAILED  ]%s %" CSTL_PRIu64 " tests, listed below:\n",
                CSTL_OUTPUT_COLOURS[RED], CSTL_OUTPUT_COLOURS[RESET], failed);
        for (index = 0; index < failed_testcases_length; index++) {
            printf("%s[  FAILED  ]%s %s\n", CSTL_OUTPUT_COLOURS[RED], CSTL_OUTPUT_COLOURS[RESET],
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


// Defines a main function and begins executing tests
#ifndef CSTL_MAIN
    #define CSTL_MAIN()      \
        /* we need, in exactly one source file, define the global struct that will hold */ \
        /* the data we need to run cstl_utest. This macro allows the user to declare the */ \
        /* data without having to use the CSTL_MAIN macro, thus allowing them to write */ \
        /* their own main() function. */ \
        struct cstl_utest_state_s    cstl_utest_state = {0, 0, 0};     \
        int main(int argc, const char* const argv[]) {              \
            return cstl_utest_main(argc, argv);                     \
        }

    // If the user defines their own main function, the following NEEDS to be declared globally, 
    // otherwise you'll run into some pretty weird errors
    struct cstl_utest_state_s        cstl_utest_state = {0, 0, 0};
#endif // CSTL_MAIN

#endif // CSTL_UTEST_H
