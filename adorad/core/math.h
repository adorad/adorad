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

#ifndef CORETEN_MATH_H
#define CORETEN_MATH_H

#include <adorad/core/types.h>

#ifndef CORETEN_MATH_CONSTANTS
#define CORETEN_MATH_CONSTANTS
    #define CORETEN_MATH_EPSILON      1.19209290e-7f
    #define CORETEN_MATH_EPSILON      1.19209290e-7f
    #define CORETEN_MATH_ZERO         0.0f
    #define CORETEN_MATH_ONE          1.0f
    #define CORETEN_MATH_TWO_THIRDS   0.666666666666666666666666666666666666667f

    #define CORETEN_MATH_TAU          6.28318530717958647692528676655900576f
    #define CORETEN_MATH_PI           3.14159265358979323846264338327950288f
    #define CORETEN_MATH_ONE_BY_TAU   0.159154943091895335768883763372514362f
    #define CORETEN_MATH_ONE_BY_PI    0.318309886183790671537767526745028724f

    #define CORETEN_MATH_TAU_BY_2     3.14159265358979323846264338327950288f
    #define CORETEN_MATH_TAU_BY_4     1.570796326794896619231321691639751442f
    #define CORETEN_MATH_TAU_BY_8     0.785398163397448309615660845819875721f

    #define CORETEN_MATH_E            2.7182818284590452353602874713526625f
    #define CORETEN_MATH_SQRT_TWO     1.41421356237309504880168872420969808f
    #define CORETEN_MATH_SQRT_THREE   1.73205080756887729352744634150587236f
    #define CORETEN_MATH_SQRT_FIVE    2.23606797749978969640917366873127623f

    #define CORETEN_MATH_LOG_TWO      0.693147180559945309417232121458176568f
    #define CORETEN_MATH_LOG_TEN      2.30258509299404568401799145468436421f
#endif

// Some fun with macros
#ifndef CORETEN_MATH_MACROS
    #define CORETEN_MATH_MACROS
    #define CORETEN_MAX(x, y)                ((x) > (y) ? (x) : (y))
    #define CORETEN_MIN(x, y)                ((x) < (y) ? (x) : (y))
    #define CORETEN_CLAMP(x, low, up)        CORETEN_MIN(CORETEN_MAX((x), (low)), (up))
	#define CORETEN_CLAMP01(x)		          CORETEN_CLAMP((x), 0, 1)
    #define CORETEN_IS_BETWEEN(x, low, up)   (((low) <= (x)) && ((x) <= (up)))
    #define CORETEN_ABS(x)                   ((x) < 0 ? -(x) : (x))
	#define CORETEN_SQUARE(x)	              ((x)*(x))
	#define CORETEN_CUBE(x)	              ((x)*(x)*(x))
#endif // CORETEN_MATH_MACROS

#endif // CORETEN_MATH_H
