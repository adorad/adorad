/*
          _____   ____  _____            _____  
    /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#include <adorad/core/debug.h>
#include <adorad/core/headers.h>

int CSTL_ATTRIBUTE_(format (printf, 2, 3))
cstlColouredPrintf(int colour, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

#ifdef CSTL_OS_UNIX
    {
        const char* str;
        switch(colour) {
            case CSTL_COLOUR_ERROR:    str = "\033[1;31m"; break;
            case CSTL_COLOUR_SUCCESS:  str = "\033[1;32m"; break;
            case CSTL_COLOUR_WARN:     str = "\033[1;33m"; break;
            case CSTL_COLOUR_CYAN:     str = "\033[1;36m"; break;
            case CSTL_COLOUR_BOLD:     str = "\033[1m"; break;
            default:                   str = "\033[0m"; break;
        }
        printf("%s", str);
        n = printf("%s", buffer);
        printf("\033[0m"); // Reset the colour
        return n;
    }
#elif defined(CSTL_OS_WINDOWS)
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(colour) {
            case CSTL_COLOUR_ERROR:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_SUCCESS:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_CYAN:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_WARN:       attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_BOLD:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED | 
                                         FOREGROUND_INTENSITY; break;
            default:                     attr = 0; break;
        }
        if(attr != 0)
            SetConsoleTextAttribute(h, attr);
        n = printf("%s", buffer);
        SetConsoleTextAttribute(h, info.wAttributes);
        return n;
    }
#else
    n = printf("%s", buffer);
    return n;
#endif // CSTL_UNIX_
}

int CSTL_char_is_digit(char c) { 
    return c >= '0' && c <= '9'; 
}

int cstlShouldDecomposeMacro(char const* actual, char const* expected, int isStringCmp) {
    // Signal that the macro can be further decomposed if either of the following symbols are present
    int dots = 0;
    int numActualDigits = 0;
    int numExpectedDigits = 0;

    // If not inside a string comparison, we will return `1` only if we determine that `actual` is a variable 
    // name/expression (i.e for a value, we search through each character verifying that each is a digit
    // - for floats, we allow a maximum of 1 '.' char)
    if(!isStringCmp) {
        for(int i=0; i < strlen(actual); i++) {
            if(CSTL_char_is_digit(actual[i])) { numActualDigits++; }
            else if(actual[i] == '.') { 
                dots++; 
                if(dots > 1) { return 1; }
            }
            else { return 1; }
        }
        // Do the same for `expected`
        dots = 0;
        for(int i=0; i < strlen(expected); i++) {
            if(CSTL_char_is_digit(expected[i])) { numExpectedDigits++; }
            else if(expected[i] == '.') { 
                dots++; 
                if(dots > 1) { return 1; }
            }
            else { return 1; }
        }
    } 
    // Inside a string comparison, we search for common expression tokens like the following:
    // '(', ')', '-'
    else {
        if(strchr(actual, '(') != NULL || strchr(expected, '(') != NULL || 
           actual[0] != '"' || expected[0] != '"' ) {
            return 1;
        }
    }
    return 0;
}

