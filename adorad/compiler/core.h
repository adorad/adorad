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

typedef enum {
    TimeFormatTime__hhmm12,
    TimeFormatTime__hhmm24,
    TimeFormatTime__hhmmss12,
    TimeFormatTime__hhmmss24,
    TimeFormatTime__hhmmss24_milli,
    TimeFormatTime__hhmmss24_micro,
    TimeFormatTime__none // no time
} TimeFormatTime;

typedef enum {
    TimeFormatDate__ddmmyy,
    TimeFormatDate__ddmmyyyy,
    TimeFormatDate__mmddyy,
    TimeFormatDate__mmddyyyy,
    TimeFormatDate__yymmdd,
    TimeFormatDate__yyyymmdd,
    TimeFormatDate__none // no data
} TimeFormatDate;

typedef enum {
    TimeFormatDelimiter__dot,
    TimeFormatDelimiter__hyphen,
    TimeFormatDelimiter__slash,
    TimeFormatDelimiter__space,
    TimeFormatDelimiter__none, // no delimiter
} TimeFormatDelimiter;

typedef enum {
    BuildMode__default,
    BuildMode__module,  // build module
} BuildMode;

typedef enum {
    ColourOutput__default,
    ColourOutput__always,
    ColourOutput__never
} ColourOutput;

typedef enum {
    CompilerType__gcc,
    CompilerType__tinyc,
    CompilerType__mingw,
    CompilerType__msvc,
    CompilerType__clang,
} CompilerType;

typedef enum {
    OutputArch__auto,
    OutputArch__amd64,
    OutputArch__arm64,
    OutputArch__arm32,
    OutputArch__rv64,
    OutputArch__rv32,
    OutputArch__i386,
} OutputArch;
