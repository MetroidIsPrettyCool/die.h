/* filename: die_defaults.inc -*- mode: c-ts; fill-column: 80; -*- */

/* Include at any point to redefine all public, non-configuration-flag
   object-like DIE_* macros to their default replacement lists.

   If __STDC_HOSTED__ == 0, DIE_OUT_DEFAULT and DIE_HOW_DEFAULT will not
   redefined.

   You should include fenv.h prior to including this header to get the proper
   definition of DIE_M_FEFLAGS.
 */

#undef DIE_WHO_DEFAULT
#define DIE_WHO_DEFAULT ""

#undef DIE_WHAT_DEFAULT
#define DIE_WHAT_DEFAULT ""

#if __STDC_HOSTED__ == 1
    #undef DIE_OUT_DEFAULT
    #define DIE_OUT_DEFAULT perror
#endif

#if __STDC_HOSTED__ == 1
    #undef DIE_HOW_DEFAULT
    #define DIE_HOW_DEFAULT exit
#endif

#undef DIE_CODE_DEFAULT
#if __STDC_HOSTED__ == 1
    #define DIE_CODE_DEFAULT EXIT_FAILURE
#else
    #define DIE_CODE_DEFAULT 1
#endif

#undef DIE_WHERE_DEFAULT
#define DIE_WHERE_DEFAULT __FILE__":"DIE_ES(__LINE__)

#undef DIE_M_FEFLAGS
#if defined(FE_DIVBYZERO) && defined(FE_INVALID) && defined(FE_OVERFLOW)
    #define DIE_M_FEFLAGS (FE_DIVBYZERO|FE_INVALID|FE_OVERFLOW)
#else
    #define DIE_M_FEFLAGS 0
#endif

#undef DIE_M_USE_ERRNO
#define DIE_M_USE_ERRNO (math_errhandling&MATH_ERRNO)

#undef DIE_M_USE_EXCEPT
#define DIE_M_USE_EXCEPT (math_errhandling&MATH_ERREXCEPT)
