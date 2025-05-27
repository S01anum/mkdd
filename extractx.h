#ifndef _STD_PAIR_H
#define _STD_PAIR_H

namespace std {
template <typename T1, typename T2>
struct pair {
	T1 first;
	T2 second;
};

template <>
struct pair<float, float> {
	pair()
	    : first(0.0f)
	    , second(0.0f) {};
	float first;
	float second;
};
} // namespace std

#endif

#ifndef _MATH_H
#define _MATH_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

#ifndef __MWERKS__
// Get clangd to shut up about __fabs being undefined.
#define __fabs(x)    (x)
#define __fabsf(x)   (x)
#define __frsqrte(x) (x)
#endif

#define FABS(x) (float)__fabs(x)
// #define __frsqrtes opword

#define SQUARE(v) ((v) * (v))

#define signbit(x) ((int)(__HI(x) & 0x80000000))

#define TAU     6.2831855f
#define PI      3.1415927f
#define HALF_PI 1.5707964f

#define LONG_TAU 6.2831854820251465

extern int __float_nan[];
extern int __float_huge[];
extern int __double_huge[];

#define INFINITY (*(float*)__float_huge)
#define NAN      (*(float*)__float_nan)
#define HUGE_VAL (*(double*)__double_huge)

inline long double fabsl(long double x) { return __fabs((double)x); }

inline double fabs(double x) { return __fabs(x); }

double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double ceil(double);
double floor(double);
double frexp(double, int*);
double ldexp(double, int);
double sqrt(double);

double pow(double, double);

double log(double);
double log10(double);

double fmod(double, double);

double sin(double x);
double cos(double x);

double __ieee754_acos(double);
double __ieee754_fmod(double, double);
double __ieee754_log(double);
double __ieee754_log10(double);
double __ieee754_pow(double, double);
double __ieee754_sqrt(double);
double __ieee754_atan2(double, double);
double __ieee754_asin(double);

double scalbn(double, int);

double __kernel_sin(double, double, int);
double __kernel_cos(double, double);
double __kernel_tan(double, double, int);

int __ieee754_rem_pio2(double, double*);

static inline float atan2f(float y, float x) { return atan2(y, x); }

// float sqrtf(float);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif

#ifndef _FLOAT_H
#define _FLOAT_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

extern int __double_max[];
extern int __extended_min[];
extern int __extended_max[];
extern int __float_max[];
extern int __float_epsilon[];

#define FLT_MANT_DIG   24
#define FLT_DIG        6
#define FLT_MIN_EXP    (-125)
#define FLT_MIN_10_EXP (-37)
#define FLT_MAX_EXP    128
#define FLT_MAX_10_EXP 38

//#define FLT_MAX 0x1.fffffeP127F
//#define FLT_EPSILON 0x1.000000P-23F
#define FLT_MIN 0x1.000000P-126F

#define FLT_MAX     (*(float*)__float_max)
#define FLT_EPSILON (*(float*)__float_epsilon)

#define DBL_MANT_DIG   53
#define DBL_DIG        15
#define DBL_MIN_EXP    (-1021)
#define DBL_MIN_10_EXP (-308)
#define DBL_MAX_EXP    1024
#define DBL_MAX_10_EXP 308

//#define DBL_MAX 0x1.fffffffffffffP1023
#define DBL_EPSILON 0x1.0000000000000P-52
#define DBL_MIN     0x1.0000000000000P-1022

#define DBL_MAX (*(double*)__double_max)

#define LDBL_MANT_DIG   53
#define LDBL_DIG        15
#define LDBL_MIN_EXP    (-1021)
#define LDBL_MIN_10_EXP (-308)
#define LDBL_MAX_EXP    1024
#define LDBL_MAX_10_EXP 308

//#define LDBL_MAX 0x1.fffffffffffffP1023L
#define LDBL_EPSILON 0x1.0000000000000P-52L
//#define LDBL_MIN 0x1.0000000000000P-1022L

#define LDBL_MAX (*(long double*)__extended_max)
#define LDBL_MIN (*(long double*)__extended_min)

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif

#ifndef _RUNTIME_VA_ARG_H
#define _RUNTIME_VA_ARG_H

struct va_list {
	char mG_register;
	char mFloat_register;
	char mPadding[2];
	char* mInput_arg_area;
	char* mReg_save_area;
};

void *__va_arg(struct va_list *v_list, signed long type);
#endif

#ifndef RUNTIME_MEM_H
#define RUNTIME_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

__declspec(section ".init") void* memcpy(void* dest, const void* src, unsigned long n);
__declspec(section ".init") void __fill_mem(void* dest, int val, unsigned long count);
__declspec(section ".init") void* memset(void* dest, int val, unsigned long count);

#ifdef __cplusplus
}
#endif
#endif

#ifndef _MEM_H
#define _MEM_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int memcmp(const void*, const void*, unsigned long);
void *__memrchr(const void *src, int val, unsigned long n);
void *memchr(const void *src, int val, unsigned long n);
void *memmove(void *, const void *, unsigned long);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif

#ifndef _STRING_H
#define _STRING_H

#ifdef __cplusplus
extern "C" {
#endif

char* strcpy(char*, const char*);
char* strncpy(char*, const char*, unsigned long);

char* strcat(char*, const char*);
char* strncat(char*, const char*, unsigned long);

int strcmp(const char*, const char*);
int strncmp(const char*, const char*, unsigned long);
char* strchr(const char*, int);
char* strstr(const char*, const char*);
char* strrchr(const char* str, int chr);
unsigned long strlen(const char*);
long strtol(const char *str, char **end, int base);

int __msl_strnicmp(const char *s1, const char *s2, int n);

#ifdef __cplusplus
}
#endif
#endif
