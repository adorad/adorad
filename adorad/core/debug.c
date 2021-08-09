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

int CORETEN_ATTRIBUTE_(format (printf, 2, 3))
cstlColouredPrintf(int colour, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

#if defined(CORETEN_OS_UNIX)
    {
        const char* str;
        switch(colour) {
            case CORETEN_COLOUR_ERROR:    str = "\033[1;31m"; break;
            case CORETEN_COLOUR_SUCCESS:  str = "\033[1;32m"; break;
            case CORETEN_COLOUR_WARN:     str = "\033[1;33m"; break;
            case CORETEN_COLOUR_CYAN:     str = "\033[1;36m"; break;
            case CORETEN_COLOUR_BOLD:     str = "\033[1m"; break;
            default:                      str = "\033[0m"; break;
        }
        printf("%s", str);
        n = printf("%s", buffer);
        printf("\033[0m"); // Reset the colour
        return n;
    }
#elif defined(CORETEN_OS_WINDOWS)
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(colour) {
            case CORETEN_COLOUR_ERROR:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_SUCCESS:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_CYAN:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_WARN:       attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_BOLD:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED | 
                                                   FOREGROUND_INTENSITY; break;
            default:                        attr = 0; break;
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
#endif // CORETEN_UNIX_
}

void coreten_panic(PanicLevel pl, const char* format, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';
    const char* str;

    switch(pl) {
        case PanicLevelUnreachable: str = "CoretenUnreachable: "; break;
        default: str = "Panic: "; break;
    }
    cstlColouredPrintf(CORETEN_COLOUR_ERROR, "%s", str);
    printf("%s\n", buffer);
    abort();
}