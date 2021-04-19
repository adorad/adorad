#ifndef CSTL_DEBUG_H
#define CSTL_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

// Debug + Asserts  ==========================================
#ifndef CSTL_CHECK
	#define CSTL_CHECK3(cond, msg) typedef char static_assertion_##msg[(!!(cond))*2-1]
	// NOTE(jasmcaus): Token pasting madness!!
	#define CSTL_CHECK2(cond, line) CSTL_CHECK3(cond, static_assertion_at_line_##line)
	#define CSTL_CHECK1(cond, line) CSTL_CHECK2(cond, line)
	#define CSTL_CHECK(cond)        CSTL_CHECK1(cond, __LINE__)
#endif


// Some compilers support applying printf-style warnings to user functions.
// #if defined(__clang__) || defined(__GNUC__)
//     #define CSTL_PRINT_ARGS(FMT)     __attribute__((format(printf, FMT, (FMT+1))))
// #else
//     #define CSTL_PRINT_ARGS(FMT)
// #endif

// CSTL_DEF void assert_handler(char const *prefix, char const *condition, char const *file, Int32 line, char const *msg, ...);
// CSTL_DEF ptrdiff_t CSTL_PRINT_ERR    (char const* fmt, ...) CSTL_PRINT_ARGS(1);
// CSTL_DEF ptrdiff_t CSTL_PRINT_ERR_VA (char const *fmt, va_list va);

// #ifndef CSTL_ASSERT_MSG
//     #define CSTL_ASSERT_MSG(cond, msg, ...) do { \
//         if (!(cond)) { \
//             assert_handler("Assertion Failure", #cond, __FILE__, cast(Int64)__LINE__, msg, ##__VA_ARGS__); \
//         } \
//     } while (0)
// #endif

// void assert_handler(char const* prefix, char const* condition, char const* file, Int32 line, char const* msg, ...) {
//     CSTL_PRINT_ERR("%s(%d): %s: ", file, line, prefix);
//     if (condition)
//         CSTL_PRINT_ERR( "`%s` ", condition);
//     if (msg) {
//         va_list va;
//         va_start(va, msg);
//         CSTL_PRINT_ERR_VA(msg, va);
//         va_end(va);
//     }
//     CSTL_PRINT_ERR("\n");
// }

// #ifndef CSTL_ASSERT
//     #define CSTL_ASSERT(cond)      CSTL_ASSERT_MSG(cond, NULL)
// #endif

// #ifndef CSTL_ASSERT_NOT_NULL
//     #define CSTL_ASSERT_NOT_NULL(ptr)      CSTL_ASSERT_MSG((ptr) != NULL, #ptr " must not be NULL")
// #endif

#if defined(__cplusplus)
}
#endif

#endif // CSTL_DEBUG_H