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

#ifndef CSTL_MATH_H
#define CSTL_MATH_H

#include <adorad/core/types.h>

#ifndef CSTL_MATH_CONSTANTS
#define CSTL_MATH_CONSTANTS
    #define CSTL_MATH_EPSILON      1.19209290e-7f
    #define CSTL_MATH_EPSILON      1.19209290e-7f
    #define CSTL_MATH_ZERO         0.0f
    #define CSTL_MATH_ONE          1.0f
    #define CSTL_MATH_TWO_THIRDS   0.666666666666666666666666666666666666667f

    #define CSTL_MATH_TAU          6.28318530717958647692528676655900576f
    #define CSTL_MATH_PI           3.14159265358979323846264338327950288f
    #define CSTL_MATH_ONE_BY_TAU   0.159154943091895335768883763372514362f
    #define CSTL_MATH_ONE_BY_PI    0.318309886183790671537767526745028724f

    #define CSTL_MATH_TAU_BY_2     3.14159265358979323846264338327950288f
    #define CSTL_MATH_TAU_BY_4     1.570796326794896619231321691639751442f
    #define CSTL_MATH_TAU_BY_8     0.785398163397448309615660845819875721f

    #define CSTL_MATH_E            2.7182818284590452353602874713526625f
    #define CSTL_MATH_SQRT_TWO     1.41421356237309504880168872420969808f
    #define CSTL_MATH_SQRT_THREE   1.73205080756887729352744634150587236f
    #define CSTL_MATH_SQRT_FIVE    2.23606797749978969640917366873127623f

    #define CSTL_MATH_LOG_TWO      0.693147180559945309417232121458176568f
    #define CSTL_MATH_LOG_TEN      2.30258509299404568401799145468436421f
#endif

// Some fun with macros
#ifndef CSTL_MAX
    #define CSTL_MAX(x, y)     ((x) > (y) ? (x) : (y))
#endif 

#ifndef CSTL_MIN
    #define CSTL_MIN(x, y)     ((x) < (y) ? (x) : (y))
#endif 

#ifndef CSTL_CLAMP
    #define CSTL_CLAMP(x, lower, upper)     CSTL_MIN(CSTL_MAX((x), (lower)), (upper))
#endif 

#ifndef CSTL_CLAMP01
	#define CSTL_CLAMP01(x)		CSTL_CLAMP((x), 0, 1)
#endif 

#ifndef CSTL_IS_BETWEEN
    #define CSTL_IS_BETWEEN(x, lower, upper)     (((lower) <= (x)) && ((x) <= (upper)))
#endif 

#ifndef CSTL_ABS
    #define CSTL_ABS(x)     ((x) < 0 ? -(x) : (x))
#endif

#ifndef CSTL_SQUARE
	#define CSTL_SQUARE(x)	((x)*(x))
#endif 

#ifndef CSTL_CUBE
	#define CSTL_CUBE(x)	((x)*(x)*(x))
#endif 

#endif // CSTL_MATH_H
