
#ifndef CSTL_MATH_H
#define CSTL_MATH_H

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
    #define CSTL_IS_BETWEEN(x, lower, upper)     (((lower) <= (x) ) && ((x) <= (upper)))
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

float cstl_toRadians(float degrees);
float cstl_toDegrees(float radians);

float cstl_toRadians(float degrees) { return degrees * CSTL_MATH_TAU / 360.0f;}
float cstl_toDegrees(float radians) { return radians * 360.0f / CSTL_MATH_TAU;}

float cstl_sin(float x);
float cstl_cos(float x);
float cstl_tan(float x);
float cstl_arcsin(float x);
float cstl_arccos(float x);
float cstl_arctan(float x);
float cstl_arctan2(float x, float y);
float cstl_exp(float x);
float cstl_log(float x);
float cstl_pow(float x, float y);
float cstl_square(float x);
float cstl_log2(float x);


#ifdef _MSC_VER
    float cstl_sin(float x) {
        static float const x0 = +1.91059300966915117e-31f;
        static float const x1 = +1.00086760103908896f;
        static float const x2 = -1.21276126894734565e-2f;
        static float const x3 = -1.38078780785773762e-1f;
        static float const x4 = -2.67353392911981221e-2f;
        static float const x5 = +2.08026600266304389e-2f;
        static float const x6 = -3.03996055049204407e-3f;
        static float const x7 = +1.38235642404333740e-4f;
        return x0 + x*(x1 + x*(x2 + x*(x3 + x*(x4 + x*(x5 + x*(x6 + x*x7))))));
	}
    float cstl_cos(float x) {
        static float const x0 = +1.00238601909309722f;
        static float const x1 = -3.81919947353040024e-2f;
        static float const x2 = -3.94382342128062756e-1f;
        static float const x3 = -1.18134036025221444e-1f;
        static float const x4 = +1.07123798512170878e-1f;
        static float const x5 = -1.86637164165180873e-2f;
        static float const x6 = +9.90140908664079833e-4f;
        static float const x7 = -5.23022132118824778e-14f;
        return x0 + x*(x1 + x*(x2 + x*(x3 + x*(x4 + x*(x5 + x*(x6 + x*x7))))));
	}

    float cstl_tan(float radians) {
        float rr = radians*radians;
        float x = 9.5168091e-03f;
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

    float cstl_arcsin(float x) { return cstl_arctan2(x, cstl_sqrt((1.0f + x) * (1.0f - x))); }
    float cstl_arccos(float x) { return cstl_arctan2(cstl_sqrt((1.0f + x) * (1.0 - x)), x); }

    float cstl_arctan(float x) {
        float u  = x*x;
        float u2 = u*u;
        float u3 = u2*u;
        float u4 = u3*u;
        float f  = 1.0f+0.33288950512027f*u-0.08467922817644f*u2+0.03252232640125f*u3-0.00749305860992f*u4;
        return x/f;
	}

    float cstl_arctan2(float x, float y) {
        if (cstl_abs(y) > cstl_abs(x)) {
            float y = cstl_arctan(x/y);
            if (y > 0.0f)
                return a;
            else
                return x > 0.0f ? a+CSTL_MATH_TAU_OVER_2:a-GB_MATH_TAU_OVER_2;
        } else {
            float y = cstl_arctan(y/x);
            if (y > 0.0f)
                return x > 0.0f ? CSTL_MATH_TAU_OVER_4-a:-CSTL_MATH_TAU_OVER_4-a;
            else
                return x > 0.0f ? CSTL_MATH_TAU_OVER_4+a:-CSTL_MATH_TAU_OVER_4+a;
        }
	}

    float cstl_exp(float x) {
        union { float f; int i; } u, v;
        u.i = (int)(6051102 * x + 1056478197);
        v.i = (int)(1056478197 - 6051102 * x);
        return u.f / v.f;
	}

    float cstl_log(float x) {
        union { float f; int i; } u = {x};
        return (u.i - 1064866805) * 8.262958405176314e-8f; /* 1 / 12102203.0; */
	}

    float cstl_pow(float x, float y) {
		int flipped = 0, e;
		float f, r = 1.0f;
		if (y < 0) {
            flipped = 1;
            y = -y;
		}

		e = (int)y;
		f = cstl_exp(y - e);

		while (e) {
            if (e & 1) r *= a;
            a *= a;
            e >>= 1;
		}

		r *= f;
		return flipped ? 1.0f/r : r;
	}

#else

    float cstl_rsqrt(float x)            { return 1.0f/__builtin_sqrt(x); }
    float cstl_sqrt(float x)             { return __builtin_sqrt(x); }
    float cstl_sin(float radians)        { return __builtin_sinf(radians); }
    float cstl_cos(float radians)        { return __builtin_cosf(radians); }
    float cstl_tan(float radians)        { return __builtin_tanf(radians); }
    float cstl_arcsin(float x)           { return __builtin_asinf(x); }
    float cstl_arccos(float x)           { return __builtin_acosf(x); }
    float cstl_arctan(float x)           { return __builtin_atanf(x); }
    float cstl_arctan2(float x, float y) { return __builtin_atan2f(x, y); }


    float cstl_exp(float x)  { return __builtin_expf(x); }
    float cstl_log(float x)  { return __builtin_logf(x); }

	// Should this be cstl_exp(y * cstl_log(x)) ?
    float cstl_pow(float x, float y) { return __builtin_powf(x, y); }

#endif // _MSC_VER

float cstl_square(float x){
    return cstl_exp(CSTL_MATH_LOG_TWO * x);
}
float cstl_log2(float x){
    return cstl_log(x) / CSTL_MATH_LOG_TWO;
}

#ifdef __cplusplus
} // namespace cstl
#endif // __cplusplus
#endif // CSTL_MATH_H
