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
    TimeFormatDelimiterDot,
    TimeFormatDelimiterHyphen,
    TimeFormatDelimiterSlash,
    TimeFormatDelimiterSpace,
    TimeFormatDelimiterNone, // no delimiter
} TimeFormatDelimiter;

typedef enum {
    BuildModeDefault,
    BuildModeModule,  // build module
} BuildMode;

typedef enum {
    ColourOutputDefault,
    ColourOutputAlways,
    ColourOutputNever
} ColourOutput;

typedef enum {
    CompilerTypeGcc,
    CompilerTypeTinyc,
    CompilerTypeMingw,
    CompilerTypeMsvc,
    CompilerTypeClang,
} CompilerType;

typedef enum {
    OutputArchAuto,
    OutputArchAmd64,
    OutputArchArm64,
    OutputArchArm32,
    OutputArchRv64,
    OutputArchRv32,
    OutputArchI386,
} OutputArch;
