
#ifndef CSTL_MATH_H
#define CSTL_MATH_H

#include <Hazel/Core/Types.h>

#ifdef __cplusplus
namespace cstl {
extern "C" {
#endif // __cplusplus

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

// Some fun with macros ==========================================
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

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

/*
Float32 cstl_toRadians(Float32 degrees);
Float32 cstl_toDegrees(Float32 radians);

Float32 cstl_toRadians(Float32 degrees) { return degrees * CSTL_MATH_TAU / 360.0f;}
Float32 cstl_toDegrees(Float32 radians) { return radians * 360.0f / CSTL_MATH_TAU;}

Float32 cstl_sin(Float32 x);
Float32 cstl_cos(Float32 x);
Float32 cstl_tan(Float32 x);
Float32 cstl_arctan(Float32 x);
Float32 cstl_arctan2(Float32 x, Float32 y);
Float32 cstl_exp(Float32 x);
Float32 cstl_log(Float32 x);
Float32 cstl_pow(Float32 x, Float32 y);
Float32 cstl_square(Float32 x);
Float32 cstl_log2(Float32 x);


Float32 cstl_sin(Float32 x) {
    static Float32 const x0 = +1.91059300966915117e-31f;
    static Float32 const x1 = +1.00086760103908896f;
    static Float32 const x2 = -1.21276126894734565e-2f;
    static Float32 const x3 = -1.38078780785773762e-1f;
    static Float32 const x4 = -2.67353392911981221e-2f;
    static Float32 const x5 = +2.08026600266304389e-2f;
    static Float32 const x6 = -3.03996055049204407e-3f;
    static Float32 const x7 = +1.38235642404333740e-4f;
    return x0 + x*(x1 + x*(x2 + x*(x3 + x*(x4 + x*(x5 + x*(x6 + x*x7))))));
}
Float32 cstl_cos(Float32 x) {
    static Float32 const x0 = +1.00238601909309722f;
    static Float32 const x1 = -3.81919947353040024e-2f;
    static Float32 const x2 = -3.94382342128062756e-1f;
    static Float32 const x3 = -1.18134036025221444e-1f;
    static Float32 const x4 = +1.07123798512170878e-1f;
    static Float32 const x5 = -1.86637164165180873e-2f;
    static Float32 const x6 = +9.90140908664079833e-4f;
    static Float32 const x7 = -5.23022132118824778e-14f;
    return x0 + x*(x1 + x*(x2 + x*(x3 + x*(x4 + x*(x5 + x*(x6 + x*x7))))));
}

Float32 cstl_tan(Float32 radians) {
    Float32 rr = radians * radians;
    Float32 x = 9.5168091e-03f;
    x *= rr;
    x += 2.900525e-03f;
    x *= rr;
    x += 2.45650893e-02f;
    x *= rr;
    x += 5.33740603e-02f;
    x *= rr;
    x += 1.333923995e-01f;
    x *= rr;
    x += 3.333314036e-01f;
    x *= rr;
    x += 1.0f;
    x *= radians;
    return x;
}

Float32 cstl_arctan(Float32 x) {
    Float32 u  = x*x;
    Float32 u2 = u*u;
    Float32 u3 = u2*u;
    Float32 u4 = u3*u;
    Float32 f  = 1.0f+0.33288950512027f*u-0.08467922817644f*u2+0.03252232640125f*u3-0.00749305860992f*u4;
    return x/f;
}

Float32 cstl_arctan2(Float32 x, Float32 y) {
    if (CSTL_ABS(y) > CSTL_ABS(x)) {
        Float32 a = cstl_arctan(x/y);
        if (y > 0.0f)
            return a;
        else
            return x > 0.0f ? a + CSTL_MATH_TAU_BY_2 : a - CSTL_MATH_TAU_BY_2;
    } else {
        Float32 a = cstl_arctan(y/x);
        if (y > 0.0f)
            return x > 0.0f ? CSTL_MATH_TAU_BY_4 - a : -CSTL_MATH_TAU_BY_4 - a;
        else
            return x > 0.0f ? CSTL_MATH_TAU_BY_4 + a : -CSTL_MATH_TAU_BY_4 + a;
    }
}

Float32 cstl_exp(Float32 x) {
    union { Float32 f; int i; } u, v;
    u.i = (int)(6051102 * x + 1056478197);
    v.i = (int)(1056478197 - 6051102 * x);
    return u.f / v.f;
}

Float32 cstl_log(Float32 x) {
    union { Float32 f; int i; } u = {x};
    return (u.i - 1064866805) * 8.262958405176314e-8f; // 1 / 12102203.0;
}

Float32 cstl_pow(Float32 x, Float32 y) {
    int flipped = 0, e;
    Float32 f, r = 1.0f;
    if (y < 0) {
        flipped = 1;
        y = -y;
    }

    e = (int)y;
    f = cstl_exp(y - e);

    while (e) {
        if (e & 1) r *= x;
        x *= x;
        e >>= 1;
    }

    r *= f;
    return flipped ? 1.0f/r : r;
}


Float32 cstl_square(Float32 x){
    return cstl_exp(CSTL_MATH_LOG_TWO * x);
}
Float32 cstl_log2(Float32 x){
    return cstl_log(x) / CSTL_MATH_LOG_TWO;
}

*/ 

#ifdef __cplusplus
} // namespace cstl
#endif // __cplusplus
#endif // CSTL_MATH_H
