#ifndef CSTL_EXCEPTION_H
#define CSTL_EXCEPTION_H 

#include <Hazel/Core/String.h>
#include <Hazel/Core/Types.h>

// #ifdef __cplusplus
// namespace Hazel {
// #endif

// // The Primary CSTL Error Class
// // Provides a complete error message with source code location information via `what()`, and a more conce message via 
// // `what_no_backtrace()`
// // This is here for internal usage only; use CSTL_CHECK/CSTL_ASSERT instead. 
// class Error : public std::exception {
// public:
//     // The actual error message
//     std::string __message;
//     // The C++ backtrace a the point when this exception was raised.
//     // This may be empty if no valid backtrace
//     std::string __backtrace; 
//     std::string __what; 
//     std::string __what_no_backtrace;
//     // Context for the message
//     std::vector<std::string> __context;

//     Error(SourceLocation src_location, std::string msg)
//     : Error(
//             std::move(msg),
//             ::cstl::str("Exception raised from ",
//                 src_location,
//                 " (most recent call first):\n"
//                 // ( *GetFetchStackTrace() )()
//                 )
//             ) {}

//     // Constructor
//     Error(
//         const char* file, 
//         const UInt32 line, 
//         const char* condition, 
//         const std::string& msg,
//         const std::string& backtrace
//     ) : Error(
//             ::cstl::str(
//                 "[enforce fail at ",
//                 detail::stripBaseName(file),
//                 ":",
//                 line,
//                 "] ",
//                 condition,
//                 ". ",
//                 msg
//             )
//             // backtrace
//         ) {} 
    
//     // Base constructor 
//     Error(std::string msg, std::string backtrace) 
//         : __message(std::move(msg)), __backtrace(std::move(backtrace)) {
//             refresh_what();
//         }
    
//     // Add context to the message stack. The last added context will be formatted at the end of the context list 
//     // upon printing. 
//     // NB: This nethod is O(n) in the size of the stack; so use it sparingly
//     void add_context(std::string msg);

//     const std::string& message() const {
//         return __message; 
//     }

//     const std::vector<std::string>& context() const {
//         return __context;
//     }

//     const std::string& backtrace() const {
//         return __backtrace;
//     }

//     // Returns the complete error message, include the source location
//     const char* what() const noexcept override {
//         return __what.c_str();
//     }

//     // Returns only he error message string (without source location)
//     const char* what_no_backtrace() const noexcept {
//         return __what_no_backtrace.c_str();
//     }

// private:
//     std::string compute_what(bool include_backtrace) const {
//         std::ostringstream oss; 

//         oss << __message; 

//         if(__context.size() == 1) {
//             // Fold error and context in one line
//             oss << " (" << __context[0] << ")";
//         } else {
//             for(const auto& c : __context) {
//                 oss << "\n" << c;
//             }
//         }

//         if(include_backtrace) {
//             oss << "\n" << __backtrace;
//         }

//         return oss.str();
//     }

//     void refresh_what() {
//         __what = compute_what(/*include_backtrace*/ true);
//         __what_no_backtrace = compute_what(/*include_backtrace*/ false);
//     }
// }; // class Error

// std::string get_backtrace(Ull frames_to_skip = 0, Ull maximum_number_of_frames = 64);

// namespace {

// // std::function<string(void)>* GetFetchStackTrace() {
// //     static std::function<string(void)> func = []() {
// //         return get_backtrace(/*frames_to_skip=*/1);
// //     };
// //     return &func;
// // };

// } // namespace

// namespace detail {
//     void cstl_check_fail(const char* func, const char* file, UInt32 line, const std::string& msg) {
//         throw ::cstl::Error({func, file, line}, msg);
//     }

//     void cstl_check_fail(const char* func, const char* file, UInt32 line, const char* msg) {
//         throw ::cstl::Error({func, file, line}, msg);
//     }

//     void cstl_assert_fail(const char* func, const char* file, UInt32 line, const char* condMsg, const char* userMsg) {
//         cstl_check_fail(func, file, line, cstl::str(condMsg, userMsg));
//     }
//     // This should never be called. It is provided in case of compilers that don't do any dead code
//     // stripping in debug builds.
//     void cstl_assert_fail(const char* func, const char* file, UInt32 line, const char* condMsg, const std::string& userMsg) {
//         cstl_check_fail(func, file, line, cstl::str(condMsg, userMsg));
//     }

// } // namespace detail

// // ========= ERROR REPORTING MACROS =========

// // Private helper macro for implementing TORCH_INTERNAL_ASSERT and TORCH_CHECK
// //
// // Note: In the debug build with MSVC, __LINE__ might be of long type (a.k.a Int32),
// // but we require an unsigned int (a.k.a UInt32) and may cause a compile error with the message:
// // error C2397: conversion from 'long' to 'uInt32' requires a narrowing conversion
// // Here the static cast is used to pass the build.
// // if this is used inside a lambda the __func__ macro expands to operator(),
// // which isn't very useful, but hard to fix in a macro so suppressing the warning.
// #define CSTL_THROW_ERROR(err_type, msg)
//     throw ::cstl::err_type({__func__, __FILE__, static_cast<UInt32>(__LINE__)}, msg)

// // This is only used in the error repoting macros below. 
// // CSTL_LIKELY/CSTL_UNLIKELY
// //
// // These macros provide parentheses, so you can use these macros as:
// //
// //    if CSTL_LIKELY(some_expr) {
// //      ...
// //    }
// //
// // NB: static_cast to boolean is mandatory in C++, because __builtin_expect
// // takes a long argument, which means you may trigger the wrong conversion
// // without it.
// //
// #if defined(__GNUC__) || defined(__ICL) || defined(__clang__)
//     #define CSTL_LIKELY(expr)    (__builtin_expect(static_cast<bool>(expr), 1))
//     #define CSTL_UNLIKELY(expr)  (__builtin_expect(static_cast<bool>(expr), 0))
// #else
//     #define CSTL_LIKELY(expr)    (expr)
//     #define CSTL_UNLIKELY(expr)  (expr)
// #endif

// #define CSTL_UNLIKELY_OR_CONST(x)   CSTL_UNLIKELY(x)

/**

// Private helper macro for workaround MSVC misexpansion of nested macro invocations involving __VA_ARGS__.  
// See https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define CSTL_EXPAND_MSVC_WORKAROUND(x)      x

// A utility macro that provides assert()-like functionality; that is, it enforces internal invariants in the code 
// It supports an arbitrary number of extra argumens that are evaluated only on failure (which will be 
// printed in the assert failure message using `operator<<` --> this might be useful to print some variables
// which may be useful for debugging.)
// 
// Usage:
//      CSTL_ASSERT(true_cond);
//      CSTL_ASSERT(a == 34, "a = ", a); 
// 
// This is usually meant for internal usage only, although we may expand this globally once stable
// Assuming no bugs in the codebase, the conditions tested by this macro should _always_ be true
// i.e disabling all of these conditions ideally should not change observable human behaviour. 
// 
// NOTE: It is SAFE to use this macro in production code; on failure, this simply raises an exeception.
// It _does NOT_ unceremoniously quit the process (unlike assert())
// 
#ifdef CSTL_STRIP_ERROR_MESSAGES
    #define CSTL_ASSERT(cond, ...) \
        if(CSTL_UNLIKELY_OR_CONST(!(cond))) { \ 
            cstl_check_fail(                                \
                __func__,                                   \
                __FILE__,                                   \
                static_cast<UInt32> (__LINE__),             \
                #cond "CSTL ASSERT FAILED at"               \
                CSTL_STRINGIFY(__FILE__)                    \
            );                                              \
        }
#else 
    #define CSTL_ASSERT(cond, ...) \
        if(CSTL_UNLIKELY_OR_CONST(!(cond))) { \ 
            cstl_assert_fail(                               \
                __func__,                                   \
                __FILE__,                                   \
                static_cast<UInt32> (__LINE__),             \
                #cond "CSTL ASSERT FAILED at"               \
                CSTL_STRINGIFY(__FILE__)                    \
                ":"                                         \
                CSTL_STRINGIFY(__LINE__)                    \
                ", please report this as a bug to Hazel. ", \
                cstl::str(__VA_ARGS)                        \
            );                                              \
        }
#endif // CSTL_STRIP_ERROR_MESSAGES

// A utility macro to make it easier to test for error conditions from user input. 
// This is like CSTL_ASSERT - it supports an arbitrary number of extra arguments (evaluated only on failure), which
// will be printed to the error message using `operator<<`. 
// Most objects in Hazel have these definitions. 
// 
// Usage:
//      CSTL_CHECK(true_cond); // A default error message will be provided, but we recommend writing
// an explicit error message, as it is more user-friendly
//      CSTL_CHECK(a == 34, "Expected a to be 10, but got ", a);
// 
// On failure, this macro will raise an exception. 
// 
// NOTE: It is SAFE to use this macro in production code; on failure, this simply raises an exeception.
// It _does NOT_ unceremoniously quit the process
// 
#ifdef CSTL_STRIP_ERROR_MESSAGES
    #define CSTL_CHECK_MSG(cond, type, ...)
        (#cond #type " CHECK FAILED at"
         CSTL_STRINGIFY(__FILE__))
    
    #define CSTL_CHECK_WITH_MSG(error_t, cond, type, ...) 
        if(CSTL_UNLIKELY_OR_CONST(!(cond))) {
            CSTL_THROW_ERROR(Error, 
                CSTL_CHECK_MSG(cond, type, __VA_ARGS__)
            );
        }
#else 
    #define CSTL_CHECK_MSG(cond, type, ...)                      \
        (cstl_check_msg_impl                                     \
            (                                                    \
                "Expected " #cond " to be true, but got false.", \
                ##__VA_ARGS__                                    \
            )                                                    \
        )
    
    #define CSTL_CHECK_WITH_MSG(error_t, cond, type, ...)    \
        if(CSTL_UNLIKELY_OR_CONST(!(cond))) {                \
            CSTL_THROW_ERROR(                                \
                error_t,                                     \
                CSTL_CHECK_MSG(cond, type, __VA_ARGS__)      \
            );                                               \
        }
#endif // CSTL_STRIP_ERROR_MESSAGES

// The cstl::str() call that creates userMsg can have 1 of 3 return
// types depending on the number and types of arguments passed to
// TORCH_INTERNAL_ASSERT.  0 arguments will get a
// CompileTimeEmptyString, 1 const char * will be passed straight
// through, and anything else will get converted to std::string.
[[noreturn]] void cstl_check_fail(const char* func, const char* file, UInt32 line, const std::string& msg);
[[noreturn]] void cstl_check_fail(const char* func, const char* file, UInt32 line, const char* msg);

[[noreturn]] void cstl_assert_fail(const char* func, const char* file, UInt32 line, const char* condMsg, const char* userMsg);
[[noreturn]] void cstl_assert_fail(const char* func, const char* file, UInt32 line, const char* condMsg, const std::string& userMsg);
[[noreturn]] void cstl_assert_fail(const char* func, const char* file, UInt32 line, const char* condMsg, cstl::detail::CompileTimeEmptyString userMsg) {
    cstl_check_fail(func, file, line, condMsg);
}

#ifdef CSTL_STRIP_ERROR_MESSAGES
    #define CSTL_CHECK(cond, ...)                     \                     
        if (CSTL_UNLIKELY_OR_CONST(!(cond))) {        \                        
            cstl_check_fail(                          \            
                __func__,                             \
                __FILE__,                             \
                static_cast<UInt32>(__LINE__),      \      
                CSTL_CHECK_MSG(cond, "", __VA_ARGS__) \
            );                                        \
        }
#else
    #define CSTL_CHECK(cond, ...)                         \
        if (CSTL_UNLIKELY_OR_CONST(!(cond))) {            \
            cstl_check_fail(                              \
                __func__,                                 \
                __FILE__,                                 \
                static_cast<UInt32>(__LINE__),            \
                CSTL_CHECK_MSG(cond, "", ##__VA_ARGS__)   \
            );                                            \
        }
#endif // CSTL_STRIP_ERROR_MESSAGES

// Debug only version of CSTL_ASSERT. This macro only checks in debug build, and does nothing in release build.  
// It is appropriate to use in situations where you want to add an assert to a hotpath, 
// but it is too expensive to run this assert on production builds.
#ifdef NDEBUG
    // Optimized version - generates no code.
    #define CSTL_ASSERT_DEBUG_ONLY(...) \
        while (false)                               \
            CSTL_EXPAND_MSVC_WORKAROUND(CSTL_ASSERT(__VA_ARGS__))
#else
    #define CSTL_ASSERT_DEBUG_ONLY(...) \
        CSTL_EXPAND_MSVC_WORKAROUND(CSTL_ASSERT(__VA_ARGS__))
#endif // NDEBUG

// Report a warning to the user.  
// This accepts an arbitrary number of extra arguments which are concatenated 
// into the warning message using `operator<<`
//
#ifdef CSTL_STRIP_ERROR_MESSAGES
    #define CSTL_WARN(...)                     \
        ::cstl::Warning::warn({                  \
                __func__,                       \
                __FILE__,                       \
                static_cast<UInt32>(__LINE__) \
                },                              \
            {},                                 \
            false                               \
        )
#else
    #define CSTL_WARN(...)                     \
        ::cstl::Warning::warn({                  \
                __func__,                       \
                __FILE__,                       \
                static_cast<UInt32>(__LINE__) \
                },                              \
            ::cstl::str(__VA_ARGS__),            \
            false                               \
        )
#endif
*/

// #ifdef __cplusplus
// } // namespace Hazel
// #endif

#endif // CSTL_EXCEPTION_H