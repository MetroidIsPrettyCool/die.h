/* TODO: Incomplete. These tests are written for a default, hosted
   configuration with no features disabled. */

/* Tested on Linux x86-64 with GCC 16.2.1, Clang 23.1.3, and whatever slimcc
   version AUR package slimcc-git r921.ff77023-2 corresponds to. */

#include <assert.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>

#include "die.h"

/* make sure these aren't function-like macros, which would screw up tests
   that check the exact result of expansion: */
#undef abort
#undef exit
#undef ferror
#undef fprintf
#undef fputs
#undef longjmp
#undef perror
#undef strerror

#if DIE_VERSION != 0x00'03'00UL
    #error version
#endif

static jmp_buf env;
static volatile int exits = 0;

#define SOME_FN_LIKE_MACRO(...)

int out(char* s) {
    assert(strcmp(s, "where string") == 0);
    return puts(s);
}

[[noreturn]]
void fake_exit(int code) {
    ++exits;
    longjmp(env, code);
}

int main() {
    fputs("running tests expected to succeed...\n", stdout);
    {
        long n;
        FILE* f;
        TRYIO(n = fprintf(stdout, "testing, testing\n")) || DIE(,fprintf);
        TRYIO(f = fopen("TESTING.TXT", "w+")) || DIE(,fopen);
        TRYIO(fclose(f)) || DIE(,fclose);
    }

    {
        mbstate_t ps = {};
        char8_t buf[256] = {};
        char* s = "abc";
        size_t r, j = 0, i = 0;
        while (
            (TRY_UCNV(r = mbrtoc8(&buf[j], &s[i], sizeof "abc" - i, &ps))
             || DIE(,mbrtoc8, ,"converting abc"))
            && r != 0
        ) {
            ++j;
            if (r != (size_t)(-3)) i += r;
        }
        assert(buf[0] == u8'a');
        assert(buf[1] == u8'b');
        assert(buf[2] == u8'c');
        assert(buf[3] == u8'\0');
    }

    {
        mbstate_t ps = {};
        char8_t buf[256] = {};
        char* s = "abc";
        size_t r, j = 0, i = 0;
        while (
            (TRY_UCNV2(r = mbrtoc8(&buf[j], &s[i], sizeof "abc" - i, &ps))
             || DIE(,mbrtoc8, ,"converting abc"))
            && r != 0
        ) {
            ++j;
            if (r != (size_t)(-3)) i += r;
        }
        assert(buf[0] == u8'a');
        assert(buf[1] == u8'b');
        assert(buf[2] == u8'c');
        assert(buf[3] == u8'\0');
    }

    {
        assert(DIE_TRY(0) == true);
        assert(DIE_TRY(1) == true);
        assert(DIE_TRY(-1) == false);

        assert(DIE_FAIL(0) == false);
        assert(DIE_FAIL(1) == false);
        assert(DIE_FAIL(-1) == true);
    }

    {
        assert(DIE_TRY0(0) == true);
        assert(DIE_TRY0(1) == false);
        assert(DIE_TRY0(-1) == false);

        assert(DIE_FAIL0(0) == false);
        assert(DIE_FAIL0(1) == true);
        assert(DIE_FAIL0(-1) == true);
    }

    {
        assert(DIE_TRY_NNEG(0) == true);
        assert(DIE_TRY_NNEG(1) == true);
        assert(DIE_TRY_NNEG(-1) == false);

        assert(DIE_FAIL_NNEG(0) == false);
        assert(DIE_FAIL_NNEG(1) == false);
        assert(DIE_FAIL_NNEG(-1) == true);
    }

    {
        assert(DIE_TRYA(nullptr) == false);
        assert(DIE_TRYA(&(int){}) == true);

        assert(DIE_FAILA(nullptr) == true);
        assert(DIE_FAILA(&(int){}) == false);
    }

    {
        assert(DIE_TRYS(nullptr) == false);
        assert(DIE_TRYS(&(int){}) == true);

        assert(DIE_FAILS(nullptr) == true);
        assert(DIE_FAILS(&(int){}) == false);
    }

    {
        assert(DIE_TRYE((void)0) == true);
        assert(errno == 0);
        assert(DIE_TRYE(errno = ERANGE) == false);
        assert(errno != 0);

        assert(DIE_FAILE((void)0) == false);
        assert(errno == 0);
        assert(DIE_FAILE(errno = ERANGE) == true);
        assert(errno != 0);
    }

    {
        assert(DIE_TRYIO(stdout) == true);
        assert(DIE_TRYIO((FILE*)nullptr) == false);
        assert(DIE_TRYIO(1) == true);
        assert(DIE_TRYIO(0) == true);
        assert(DIE_TRYIO(-1) == false);
        assert(DIE_TRYIO((signed char)1) == true);
        assert(DIE_TRYIO((signed char)0) == true);
        assert(DIE_TRYIO((signed char)-1) == false);
        assert(DIE_TRYIO((short)1) == true);
        assert(DIE_TRYIO((short)0) == true);
        assert(DIE_TRYIO((short)-1) == false);
        assert(DIE_TRYIO(1L) == true);
        assert(DIE_TRYIO(0L) == true);
        assert(DIE_TRYIO(-1L) == false);
        assert(DIE_TRYIO(1LL) == true);
        assert(DIE_TRYIO(0LL) == true);
        assert(DIE_TRYIO(-1LL) == false);
        assert(DIE_TRYIO(1wb) == true);
        assert(DIE_TRYIO(0wb) == true);
        assert(DIE_TRYIO(-1wb) == false);

        assert(DIE_FAILIO(stdout) == false);
        assert(DIE_FAILIO((FILE*)nullptr) == true);
        assert(DIE_FAILIO(1) == false);
        assert(DIE_FAILIO(0) == false);
        assert(DIE_FAILIO(-1) == true);
        assert(DIE_FAILIO((signed char)1) == false);
        assert(DIE_FAILIO((signed char)0) == false);
        assert(DIE_FAILIO((signed char)-1) == true);
        assert(DIE_FAILIO((short)1) == false);
        assert(DIE_FAILIO((short)0) == false);
        assert(DIE_FAILIO((short)-1) == true);
        assert(DIE_FAILIO(1L) == false);
        assert(DIE_FAILIO(0L) == false);
        assert(DIE_FAILIO(-1L) == true);
        assert(DIE_FAILIO(1LL) == false);
        assert(DIE_FAILIO(0LL) == false);
        assert(DIE_FAILIO(-1LL) == true);
        assert(DIE_FAILIO(1wb) == false);
        assert(DIE_FAILIO(0wb) == false);
        assert(DIE_FAILIO(-1wb) == true);
    }

    {
        #define ferror(...) false
        assert(DIE_TRYRD(f, 10) == true);
        assert(DIE_TRYRD(f, EOF) == true);
        assert(DIE_FAILRD(f, 10) == false);
        assert(DIE_FAILRD(f, EOF) == false);
        #undef ferror
        #define ferror(...) true
        assert(DIE_TRYRD(f, 10) == true);
        assert(DIE_TRYRD(f, EOF) == false);
        assert(DIE_FAILRD(f, 10) == false);
        assert(DIE_FAILRD(f, EOF) == true);
        #undef ferror
    }

    {
        #define ferror(...) false
        assert(DIE_TRY_WRD(f, 10) == true);
        assert(DIE_TRY_WRD(f, WEOF) == true);
        assert(DIE_FAIL_WRD(f, 10) == false);
        assert(DIE_FAIL_WRD(f, WEOF) == false);
        #undef ferror
        #define ferror(...) true
        assert(DIE_TRY_WRD(f, 10) == true);
        assert(DIE_TRY_WRD(f, WEOF) == false);
        assert(DIE_FAIL_WRD(f, 10) == false);
        assert(DIE_FAIL_WRD(f, WEOF) == true);
        #undef ferror
    }

    {
        assert(DIE_TRY_OPN(nullptr) == false);
        assert(DIE_TRY_OPN(&(int){}) == true);

        assert(DIE_FAIL_OPN(nullptr) == true);
        assert(DIE_FAIL_OPN(&(int){}) == false);
    }

    {
        assert(DIE_TRY_NEOF(EOF) == false);
        assert(DIE_TRY_NEOF(70) == true);

        assert(DIE_FAIL_NEOF(EOF) == true);
        assert(DIE_FAIL_NEOF(70) == false);
    }

    {
        assert(DIE_TRY_NWEOF(WEOF) == false);
        assert(DIE_TRY_NWEOF(70) == true);

        assert(DIE_FAIL_NWEOF(WEOF) == true);
        assert(DIE_FAIL_NWEOF(70) == false);
    }

    {
        assert(DIE_TRY_SIG(signal(SIGABRT, SIG_DFL)) == true);
        assert(DIE_TRY_SIG(SIG_ERR) == false);

        assert(DIE_FAIL_SIG(signal(SIGFPE, SIG_DFL)) == false);
        assert(DIE_FAIL_SIG(SIG_ERR) == true);
    }

    {
        assert(DIE_TRY_UCNV((size_t)(3)) == true);
        assert(DIE_TRY_UCNV((size_t)(2)) == true);
        assert(DIE_TRY_UCNV((size_t)(1)) == true);
        assert(DIE_TRY_UCNV((size_t)(0)) == true);
        assert(DIE_TRY_UCNV((size_t)(-1)) == false);
        assert(DIE_TRY_UCNV((size_t)(-2)) == true);
        assert(DIE_TRY_UCNV((size_t)(-3)) == true);

        assert(DIE_FAIL_UCNV((size_t)(3)) == false);
        assert(DIE_FAIL_UCNV((size_t)(2)) == false);
        assert(DIE_FAIL_UCNV((size_t)(1)) == false);
        assert(DIE_FAIL_UCNV((size_t)(0)) == false);
        assert(DIE_FAIL_UCNV((size_t)(-1)) == true);
        assert(DIE_FAIL_UCNV((size_t)(-2)) == false);
        assert(DIE_FAIL_UCNV((size_t)(-3)) == false);
    }

    {
        errno = 0;
        assert(TRY_UCNV2((size_t)(-1))  == false);
        assert(errno == EILSEQ);
        errno = 0;
        assert(TRY_UCNV2((size_t)(-2))  == false);
        assert(errno == EILSEQ);
        assert(TRY_UCNV2((size_t)(-3))  == true);
        assert(TRY_UCNV2((size_t)(-4))  == true);
        assert(TRY_UCNV2((size_t)(-5))  == true);
        assert(TRY_UCNV2((size_t)(0))   == true);
        assert(TRY_UCNV2((size_t)(1))   == true);
        assert(TRY_UCNV2((size_t)(2))   == true);
        assert(TRY_UCNV2((size_t)(3))   == true);
        assert(TRY_UCNV2((size_t)(4))   == true);
        assert(TRY_UCNV2((size_t)(5))   == true);

        errno = 0;
        assert(FAIL_UCNV2((size_t)(-1)) == true);
        assert(errno == EILSEQ);
        errno = 0;
        assert(FAIL_UCNV2((size_t)(-2)) == true);
        assert(errno == EILSEQ);
        assert(FAIL_UCNV2((size_t)(-3)) == false);
        assert(FAIL_UCNV2((size_t)(-4)) == false);
        assert(FAIL_UCNV2((size_t)(-5)) == false);
        assert(FAIL_UCNV2((size_t)(0))  == false);
        assert(FAIL_UCNV2((size_t)(1))  == false);
        assert(FAIL_UCNV2((size_t)(2))  == false);
        assert(FAIL_UCNV2((size_t)(3))  == false);
        assert(FAIL_UCNV2((size_t)(4))  == false);
        assert(FAIL_UCNV2((size_t)(5))  == false);
    }

    {
        assert(DIE_TRYT(thrd_success) == true);
        assert(DIE_TRYT(thrd_timedout) == false
               || thrd_timedout == thrd_success);
        assert(DIE_TRYT(thrd_busy) == false
               || thrd_busy == thrd_success);
        assert(DIE_TRYT(thrd_error) == false
               || thrd_error == thrd_success);
        assert(DIE_TRYT(thrd_nomem) == false
               || thrd_nomem == thrd_success);

        assert(DIE_FAILT(thrd_success) == false);
        assert(DIE_FAILT(thrd_timedout) == true
               || thrd_timedout == thrd_success);
        assert(DIE_FAILT(thrd_busy) == true
               || thrd_busy == thrd_success);
        assert(DIE_FAILT(thrd_error) == true
               || thrd_error == thrd_success);
        assert(DIE_FAILT(thrd_nomem) == true
               || thrd_nomem == thrd_success);
    }

    {
        assert(DIE_TRY_CLK((clock_t)0.0) == true);
        assert(DIE_TRY_CLK((clock_t)(-1)) == false);

        assert(DIE_FAIL_CLK((clock_t)0.0) == false);
        assert(DIE_FAIL_CLK((clock_t)(-1)) == true);
    }

    {
        assert(DIE_TRY_TIM((time_t)0.0) == true);
        assert(DIE_TRY_TIM((time_t)(-1)) == false);

        assert(DIE_FAIL_TIM((time_t)0.0) == false);
        assert(DIE_FAIL_TIM((time_t)(-1)) == true);
    }

    {
        assert(DIE_TRYNR((void)0) == false);
        assert(DIE_FAILNR((void)0) == true);

        /* these asserts should never have a chance to fail */
        if (setjmp(env) == 0) assert(DIE_TRYNR(longjmp(env, 1)) == true);
        if (setjmp(env) == 0) assert(DIE_FAILNR(longjmp(env, 1)) == false);
    }

    {
        int i = 0;
        #ifdef NAN
            #ifndef DIE4NAN
                #define DIE4NAN NAN
            #endif
            puts("NAN");
            assert(DIE_TRYM((++i, NAN)) == false);
            assert(DIE_FAILM((++i, NAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef DEC_NAN
            #ifndef DIE4NAN
                #define DIE4NAN DEC_NAN
            #endif
            puts("DEC_NAN");
            assert(DIE_TRYM((++i, DEC_NAN)) == false);
            assert(DIE_FAILM((++i, DEC_NAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef FLT_SNAN
            #ifndef DIE4NAN
                #define DIE4NAN FLT_SNAN
            #endif
            puts("FLT_SNAN");
            assert(DIE_TRYM((++i, FLT_SNAN)) == false);
            assert(DIE_FAILM((++i, FLT_SNAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef DBL_SNAN
            #ifndef DIE4NAN
                #define DIE4NAN DBL_SNAN
            #endif
            puts("DBL_SNAN");
            assert(DIE_TRYM((++i, DBL_SNAN)) == false);
            assert(DIE_FAILM((++i, DBL_SNAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef LDBL_SNAN
            #ifndef DIE4NAN
                #define DIE4NAN LDBL_SNAN
            #endif
            puts("LDBL_SNAN");
            assert(DIE_TRYM((++i, LDBL_SNAN)) == false);
            assert(DIE_FAILM((++i, LDBL_SNAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef DEC32_SNAN
            #ifndef DIE4NAN
                #define DIE4NAN DEC32_SNAN
            #endif
            puts("DEC32_SNAN");
            assert(DIE_TRYM((++i, DEC32_SNAN)) == false);
            assert(DIE_FAILM((++i, DEC32_SNAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef DEC64_SNAN
            #ifndef DIE4NAN
                #define DIE4NAN DEC64_SNAN
            #endif
            puts("DEC64_SNAN");
            assert(DIE_TRYM((++i, DEC64_SNAN)) == false);
            assert(DIE_FAILM((++i, DEC64_SNAN)) == true);
        #else
            i += 2;
        #endif
        #ifdef DEC128_SNAN
            #ifndef DIE4NAN
                #define DIE4NAN DEC128_SNAN
            #endif
            puts("DEC128_SNAN");
            assert(DIE_TRYM((++i, DEC128_SNAN)) == false);
            assert(DIE_FAILM((++i, DEC128_SNAN)) == true);
        #else
            i += 2;
        #endif

        assert(i == 16);

        if (math_errhandling & MATH_ERRNO) {
            puts("MATH_ERRNO");
            i = 0;
            assert(DIE_TRYM((++i, errno = ERANGE, 0.0)) == false);
            assert(DIE_TRYM((++i, 0.0)) == true);
            assert(errno == 0);
            assert(DIE_TRYM((++i, errno = EDOM, 0.0)) == false);
            assert(DIE_FAILM((++i, errno = ERANGE, 0.0)) == true);
            assert(DIE_FAILM((++i, 0.0)) == false);
            assert(errno == 0);
            assert(DIE_FAILM((++i, errno = EDOM, 0.0)) == true);
            assert(i == 6);

            double volatile f = 4.0;
            assert(DIE_TRYM(f = sqrt(f)) == true);
            assert(f == 2.0);
            f = 9.0;
            assert(DIE_FAILM(f = sqrt(f)) == false);
            assert(f == 3.0);

            f = -4.0;
            assert(DIE_TRYM(f = sqrt(-4.0)) == false);
            f = -9.0;
            assert(DIE_FAILM(f = sqrt(-9.0)) == true);
        }

        if (math_errhandling & MATH_ERREXCEPT) {
            puts("MATH_ERREXCEPT");
            i = 0;
            assert(DIE_TRYM((++i, assert(!fesetexcept(FE_DIVBYZERO)), 0.0)) == false);
            assert(DIE_TRYM((++i, 0.0)) == true);
            assert(fetestexcept(FE_DIVBYZERO|FE_INVALID|FE_OVERFLOW) == 0);
            assert(DIE_TRYM((++i, assert(!fesetexcept(FE_INVALID)), 0.0)) == false);
            assert(DIE_TRYM((++i, assert(!fesetexcept(FE_OVERFLOW)), 0.0)) == false);
            assert(DIE_FAILM((++i, assert(!fesetexcept(FE_DIVBYZERO)), 0.0)) == true);
            assert(DIE_FAILM((++i, 0.0)) == false);
            assert(fetestexcept(FE_DIVBYZERO|FE_INVALID|FE_OVERFLOW) == 0);
            assert(DIE_FAILM((++i, assert(!fesetexcept(FE_INVALID)), 0.0)) == true);
            assert(DIE_FAILM((++i, assert(!fesetexcept(FE_OVERFLOW)), 0.0)) == true);
            assert(i == 8);

            double volatile f = 4.0;
            assert(DIE_TRYM(f = sqrt(f)) == true);
            assert(f == 2.0);
            f = 9.0;
            assert(DIE_FAILM(f = sqrt(f)) == false);
            assert(f == 3.0);

            f = -4.0;
            assert(DIE_TRYM(f = sqrt(-4.0)) == false);
            f = -9.0;
            assert(DIE_FAILM(f = sqrt(-9.0)) == true);
        }

        {
            #undef DIE_M_USE_ERRNO
            #define DIE_M_USE_ERRNO false
            #undef DIE_M_USE_EXCEPT
            #define DIE_M_USE_EXCEPT false
            puts("0");
            i = 0;
            assert(DIE_TRYM((++i, 0.0)) == true);
            assert(DIE_FAILM((++i, 0.0)) == false);
            #ifdef DIE4NAN
                assert(DIE_TRYM((++i, DIE4NAN)) == false);
                assert(DIE_FAILM((++i, DIE4NAN)) == true);
            #else
                i += 2;
            #endif
            if (math_errhandling & MATH_ERRNO) {
                puts("0 MATH_ERRNO");
                assert(DIE_TRYM((++i, errno = EDOM, 0.0)) == true);
                assert(DIE_FAILM((++i, errno = EDOM, 0.0)) == false);
                #ifdef DIE4NAN
                    assert(DIE_TRYM((++i, errno = EDOM, DIE4NAN)) == false);
                    assert(DIE_FAILM((++i, errno = EDOM, DIE4NAN)) == true);
                #else
                    i += 2;
                #endif
            }

            if (math_errhandling & MATH_ERREXCEPT) {
                puts("0 MATH_ERREXCEPT");
                assert(DIE_TRYM((++i, assert(!fesetexcept(FE_INVALID)), 0.0)) == true);
                assert(DIE_FAILM((++i, assert(!fesetexcept(FE_INVALID)), 0.0)) == false);
                #ifdef DIE4NAN
                    assert(DIE_TRYM((++i, assert(!fesetexcept(FE_INVALID)), DIE4NAN)) == false);
                    assert(DIE_FAILM((++i, assert(!fesetexcept(FE_INVALID)), DIE4NAN)) == true);
                #else
                    i += 2;
                #endif
            }

            assert(i == 12);
            #undef DIE_M_USE_ERRNO
            #define DIE_M_USE_ERRNO (math_errhandling&MATH_ERRNO)
            #undef DIE_M_USE_EXCEPT
            #define DIE_M_USE_EXCEPT (math_errhandling&MATH_ERREXCEPT)
        }
    }

    {
        errno = 0;
        assert(feclearexcept(DIE_M_FEFLAGS) == 0);

        assert(DIE_TRYMC(0.0) == true);
        if (math_errhandling != 0) assert(DIE_TRYMC() == true);
        assert(DIE_FAILMC(0.0) == false);
        if (math_errhandling != 0) assert(DIE_FAILMC() == false);
        #ifdef DIE4NAN
            assert(DIE_TRYMC(DIE4NAN) == false);
            if (math_errhandling != 0) assert(DIE_TRYMC() == true);
            assert(DIE_FAILMC(DIE4NAN) == true);
            if (math_errhandling != 0) assert(DIE_FAILMC() == false);
        #endif
        if (math_errhandling & MATH_ERRNO) {
            puts("MC MATH_ERRNO");
            assert(DIE_TRYMC((errno = EDOM, 0.0)) == false);
            assert(DIE_TRYMC() == false);
            assert(DIE_FAILMC(0.0) == true);
            assert(DIE_FAILMC() == true);
            #ifdef DIE4NAN
                assert(DIE_TRYMC((errno = EDOM, DIE4NAN)) == false);
                assert(DIE_TRYMC() == false);
                assert(DIE_FAILMC(DIE4NAN) == true);
                assert(DIE_FAILMC() == true);
            #endif
            errno = 0;
        }
        if (math_errhandling & MATH_ERREXCEPT) {
            puts("MC MATH_ERREXCEPT");
            assert(DIE_TRYMC((assert(fesetexcept(FE_INVALID) == 0), 0.0)) == false);
            assert(DIE_TRYMC() == false);
            assert(DIE_FAILMC(0.0) == true);
            assert(DIE_FAILMC() == true);
            #ifdef DIE4NAN
                assert(DIE_TRYMC(DIE4NAN) == false);
                assert(DIE_TRYMC() == false);
                assert(DIE_FAILMC(DIE4NAN) == true);
                assert(DIE_FAILMC() == true);
            #endif
            assert(feclearexcept(DIE_M_FEFLAGS) == 0);
        }
    }

    {
        assert(DIE_TRYEQ(0,0) == true);
        assert(DIE_TRYEQ(0,1) == false);
        assert(DIE_TRYEQ(1,0) == false);
        assert(DIE_TRYEQ(1,1) == true);

        assert(DIE_FAILEQ(0,0) == false);
        assert(DIE_FAILEQ(0,1) == true);
        assert(DIE_FAILEQ(1,0) == true);
        assert(DIE_FAILEQ(1,1) == false);
    }

    {
        #define DONT(...) (void)0
        #define STRDUP_MSG(S,...) (s=strdup(S))
        int n = snprintf(0, 0, "%s:%d", __FILE__, __LINE__+6);
        assert(n > 0);
        char* t = malloc(n+1);
        assert(t != nullptr);
        assert(snprintf(t, n+1, "%s:%d", __FILE__, __LINE__+2)==n);
        char* s = nullptr;
        DIE(out,STRDUP_MSG, how,DONT);
        assert(s != nullptr);
        assert(strcmp(s,t) == 0);
        free(s), free(t);
    }

    {
        #define COPY_VALS(A,B,C,D) (x=A,y=B,z=C,w=D)
        int x = 0, y = 0, z = 0, w = 0;
        DIE(code,1453, out,DONT, how,(COPY_VALS,5,23,137));
        assert(x == 5);
        assert(y == 23);
        assert(z == 137);
        assert(w == 1453);
    }

    #define EXPAND_AND_STRINGIZE(X) EXPAND_AND_STRINGIZE1(X)
    #define EXPAND_AND_STRINGIZE1(X) #X
    #define CHECK_EXPANSION(X,...) do{int n = snprintf(0,0,__VA_ARGS__);   \
                                   assert(n>0);char*t=malloc(n+1);         \
                                   assert(t!=nullptr);                     \
                                   assert(snprintf(t,n+1,__VA_ARGS__)==n); \
                                   char*s=EXPAND_AND_STRINGIZE(X);         \
                                   puts(s);                                \
                                   puts(t);                                \
                                   assert(strcmp(s,t) == 0);               \
                                   free(t);}while(0)

    CHECK_EXPANSION(DIE(), "((perror(\"%s\"\":\"\"%d\" \"\" \"\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(,SOME_FN_LIKE_MACRO), "((perror(\"%s\"\":\"\"%d\" \": \"\"SOME_FN_LIKE_MACRO\" \"\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(who,SOME_FN_LIKE_MACRO), "((perror(\"%s\"\":\"\"%d\" \": \"\"SOME_FN_LIKE_MACRO\" \"\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(DIE_who,SOME_FN_LIKE_MACRO), "((perror(\"%s\"\":\"\"%d\" \": \"\"SOME_FN_LIKE_MACRO\" \"\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(DIE_who,SOME_FN_LIKE_MACRO, ,"cheese"), "((perror(\"%s\"\":\"\"%d\" \": \"\"SOME_FN_LIKE_MACRO\" \": \"\"cheese\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(,who, ,"what"), "((perror(\"%s\"\":\"\"%d\" \": \"\"who\" \": \"\"what\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(,what, ,"who"), "((perror(\"%s\"\":\"\"%d\" \": \"\"what\" \": \"\"who\" ),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(,malloc, ,"allocation failed", ,DIE_epf, ,(longjmp, there), ,137, ,"right here"), "((fprintf(stderr,\"right here\" \": \"\"malloc\" \": \"\"allocation failed\"\"\\n\"),longjmp(there,137),0)?(free(0),0):0)");
    CHECK_EXPANSION(DIE(who,malloc, what,"allocation failed", out,DIE_epf, how,(longjmp, there), code,137, where,"right here"), "((fprintf(stderr,\"right here\" \": \"\"malloc\" \": \"\"allocation failed\"\"\\n\"),longjmp(there,137),0)?(free(0),0):0)");
    CHECK_EXPANSION(DIE(DIE_who,malloc, DIE_what,"allocation failed", DIE_out,DIE_epf, DIE_how,(longjmp, there), DIE_code,137, DIE_where,"right here"), "((fprintf(stderr,\"right here\" \": \"\"malloc\" \": \"\"allocation failed\"\"\\n\"),longjmp(there,137),0)?(free(0),0):0)");
    CHECK_EXPANSION(DIE(DIE_who,malloc, DIE_what,"allocation failed", DIE_out,DIE_epf, DIE_how,(longjmp, there), DIE_code,137, DIE_where,"right here", DIE_who,calloc, DIE_what,"something something", DIE_out,DIE_ep, DIE_how,(longjmp, where), DIE_code,23, DIE_where,"over there"), "((fputs(\"over there\" \": \"\"calloc\" \": \"\"something something\"\"\\n\",stderr),longjmp(where,23),0)?(free(0),0):0)");
    CHECK_EXPANSION(DIE(how,DIE_abort), "((perror(\"%s\"\":\"\"%d\" \"\" \"\" ),abort(),0)?(free(0),0):0)", __FILE__, __LINE__);
    CHECK_EXPANSION(DIE(out,(DIE_epf, x, y, z)), "((fprintf(stderr,\"%s\"\":\"\"%d\" \"\" \"\"\"\\n\",x, y, z),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(out,DIE_elt(strerror(code))), "((fprintf(stderr,\"%s\"\":\"\"%d\" \"\" \"\"\": %%s\\n\",(strerror(code))),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);
    CHECK_EXPANSION(DIE(DIE_out,(DIE_ep)), "((fputs(\"%s\"\":\"\"%d\" \"\" \"\"\"\\n\",stderr),exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);

    CHECK_EXPANSION(DIE(,a,,"a",,perror,,exit,,1,,"here",,,,,,,,), "DIE3_too_many_positional_arguments");
    CHECK_EXPANSION(DIE(how), "DIE3_odd_number_of_arguments");
    CHECK_EXPANSION(DIE(,a,who), "DIE3_odd_number_of_arguments");
    CHECK_EXPANSION(DIE(,a,,"a",out), "DIE3_odd_number_of_arguments");
    CHECK_EXPANSION(DIE(,a,,"a",,perror,,exit,,1,,"here",what), "DIE3_odd_number_of_arguments");
    CHECK_EXPANSION(DIE(DIE_who,malloc, DIE_what,"allocation failed", DIE_out,DIE_epf, DIE_how,(longjmp, there), DIE_code,137, DIE_where,"right here", DIE_who,calloc, DIE_what,"something something", DIE_out,DIE_ep, DIE_how,(longjmp, where), DIE_code,23, DIE_where,"over there", who,test, who,test2, who,test3, who,test4, who,test5, who,test6, who,test7, who,test8, who,test9, who,test10), "DIE3_too_many_arguments");

    #undef DIE_HOW_DEFAULT
    #define DIE_HOW_DEFAULT fake_exit

    CHECK_EXPANSION(DIE(), "((perror(\"%s\"\":\"\"%d\" \"\" \"\" ),fake_exit(%d),0)?(free(0),0):0)", __FILE__, __LINE__, EXIT_FAILURE);

    assert(exits == 0);

    fputs("\nrunning tests expected to fail...\n", stdout);
    {
        int c = 0;
        if ((c = setjmp(env)) == 0) {
            errno = EILSEQ;
            DIE(code,7);
        } else {
            assert(c == 7);
        }
    }
    assert(exits == 1);

    {
        constexpr int code = 30;
        int c = 0;
        if ((c = setjmp(env)) == 0) {
            errno = ERANGE;
            DIE(code,code);
        } else {
            assert(c == 30);
        }
    }
    assert(exits == 2);

    {
        #define STRDUP_MSG(S,...) (s=strdup(S))
        int n = snprintf(0, 0, "%s:%d: me", __FILE__, __LINE__+19);
        assert(n > 0);
        char* t = malloc(n+1);
        assert(t != nullptr);
        assert(snprintf(t, n+1, "%s:%d: me", __FILE__, __LINE__+15)==n);
        char* volatile s = nullptr;
        int volatile x = 0;

        typedef struct how {
            int who, what, out, code;
        } where;
        where foo = {
            .who = 1,
            .what = 2,
            .out = 3,
            .code = 4,
        };
        int c = 0;
        if ((c = setjmp(env)) == 0) {
            TRY0(x = abs(-1)) || DIE(who,me, out,STRDUP_MSG, code,foo.code);
        } else {
            assert(c == 4);
            assert(s != nullptr);
            puts(s);
            assert(strcmp(s, t) == 0);
            assert(foo.who  == 1);
            assert(foo.what == 2);
            assert(foo.out  == 3);
            assert(foo.code == 4);
            assert(&foo.who  == (int*)((char*)&foo + offsetof(where,who)));
            assert(&foo.what == (int*)((char*)&foo + offsetof(where,what)));
            assert(&foo.out  == (int*)((char*)&foo + offsetof(where,out)));
            assert(&foo.code == (int*)((char*)&foo + offsetof(where,code)));
            assert(x == 1);
        }
        free(s), free(t);
    }
    assert(exits == 3);

    {
        int c = 0;
        int* volatile x = &c;
        if ((c = setjmp(env)) == 0) {
            TRYA(x = nullptr) || DIE(out,out, where,"where string");
        } else {
            assert(c == EXIT_FAILURE);
            assert(x == nullptr);
        }
    }
    assert(exits == 4);

    {
        /* The following expressions should fail their static asserts in
           real code, so we'll invert the definition of DIE4STATIC_ASSERT.
           Comment out this redefinition to see the failures. */
        #undef DIE4STATIC_ASSERT
        #define DIE4STATIC_ASSERT(...) static_assert(!(__VA_ARGS__))

        if (false) {
            (void)DIE_FAIL_NNEG(0u);
            (void)TRYIO(fread((char[100]){}, 1, 100, stdin));
        }

        #undef DIE4STATIC_ASSERT
        #define DIE4STATIC_ASSERT(...) static_assert(__VA_ARGS__)
    }

    fputs("\ndone!\n", stdout);
    DIE(
       ,self_test,
       out,(fputs,stdout), what,"all tests passed!\n",
       code,EXIT_SUCCESS, how,exit
    );
}
