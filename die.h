/* filename: die.h  -*- fill-column: 80; -*- */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

/*                              _ _        _
                             __| (_) ___  | |__
                            / _` | |/ _ \ | '_ \
                           | (_| | |  __/_| | | |
                            \__,_|_|\___(_)_| |_|

                  Macro-only fatal error handling for C23.

                                     ---

   This header is divided into sections using form feed (ASCII ^L, 0x0C)
   characters for easier navigation. You can skip ahead to the API
   documentation, which begins just after the include guard, or else continue
   scrolling for a discussion of implementation concerns.

                                     ---


               @@@@  .@@@@   .@@  @@@@@@@@  @@@@
            --@#@#----####----####-@@@@@##--@-#  .---
          @@@@@ @.@@ @   .@@@@@  .@@@@@@@@@@@@   .@@@@@
        *@@@=@***@ @@+@@**===+@@**===+*@**===+*@@@=@=+***
        @@   .@@@@   .@@@@   .@@@@   .@@@@  @@@@@@   .@@@@
        .:@@@%..@@@@@%...:@@@%...:@@@@...:@@@%...:@@@%..@:@@
        .:@@@%.@@:@@@%...:@@@%...:@@@%...:@@@%...:@@@@@..:@@@
        @@   . @@@   .@@@@   @@@     @@@@@   . @@@@@@@@@@@   .
        **===.****=@@+***                              ***===+
         .@@@@   .                                        @@
       @@#----###.              Here lies                   --
     @@@@    .                    andy                       .
    @@@@  @@@
    @#@@- ###*-                                            ##*
     .@@@@   .     peperony and                              .
    **@===***      chease                               ==****
    @@   .@                                              .@@@@           +
     .@@@@   .       .       .                               .   +       .   .
 .    %@%#:::-%%%#  :    #:::-%%%#:      %%::-%%    :-%% % ::-   + + .  =. =
  +    @ .@@ @   @@@@@   .@@@@   .@@@    .@@@@ @   @@@@@ .@@@@   ++ +  ++
+ +-  +@@+@+ ++@+++++++++++++@@@+@+++++@++++++++++@+++++++++++   +-+   +-    -+-
 +  + @@@@@   @@@@   .@@@@   .@@@@   .@@@@   .@@@@   .@@@@   .+  ++     +.
  ++    @:%%%%::::%%%%::::%%%%::::%%%%::::%%%%@:::%%%%::::%%%. +++ +  +.     %::
 . +    + @@@@   .@@@@   .@@@@   .@@@@   .@@@@   .@@@@   . @@@  ++++ ++ ++
 . +  ++               +      +                         @@            +   +  .++
 . :  :++. :     +:        +       ===*               *           :: .-- .:: .
 .++     .++         ++ ++   @    @@@            .               .++
     .=- .::   - .::-=      -                                     ::
                          Press SPACE BAR to continue
 */

/*                            Notes on Portability
   --------------------------------------------------------------------------

   This header declares no identifiers in any scope or namespace but the
   preprocessor and members of anonymous struct types.

   --------------------------------------------------------------------------

   This header reserves the macro identifier DIE_H, and all macro identifiers
   beginning with DIE followed by a digit (0-9), for its internal use.
   Portable code should not rely on the definition or behavior of these
   macros.

   All other macros defined herein comprise the public API of this header.

   This header reserves all other macro identifiers beginning with DIE
   followed by an underscore for its public API.

   Additional "alias" macros are also defined, unless DIE_NO_ALIASES is
   defined prior to including die.h; these all begin with TRY or FAIL.

   If this isn't acceptable, you can safely rename the prefix by means of
   find and replace, as long as you replace every instance of `DIE` in this
   file (it doesn't matter if you replace them in comments, of course). A
   later revision will include a script to perform this procedure
   mechanically.

   --------------------------------------------------------------------------

   This header defines the preprocessor macro DIE_VERSION to an integer
   constant of the form 0xAA'BB'CCUL, where AA is the major version (in hex),
   BB is the minor version (in hex), and CC is the patch version (still in
   hex), as defined by SemVer 2.0.0. <https://semver.org/>

   --------------------------------------------------------------------------

   If DIE_NO_STD_HEADERS is not defined prior to including this header,
   several standard headers will be included for your convenience.

   If __STDC_HOSTED__ == 1 when this header is included, the automatically
   included headers are <errno.h>, <fenv.h>, <stddef.h>, <stdio.h>, and
   <stdlib.h>; which are enough to use the DIE macro with the default
   defaults, and most of the common TRY/FAIL macros. Other macros defined in
   this header may require additional headers to be included before they can
   be instantiated.

   If __STDC_HOSTED__ == 0 when this header is included, the only
   automatically included header will be <stddef.h>.

   --------------------------------------------------------------------------

   If __STDC_HOSTED__ == 0 when this header is included, the macros
   DIE_OUT_DEFAULT and DIE_HOW_DEFAULT will be undefined. You will need to
   define them prior to instantiating the DIE macro.

   --------------------------------------------------------------------------

   The DIE macro is built around the expectation that, when the DIE macro is
   instantiated, it will be able to undergo several rounds of macro expansion
   without the identifiers `who`, `what`, `out`, `how`, `code`, and `where`
   changing, so that it can eventually perform token joining with them. They
   can be undefined, and they can be function-like macros, but they cannot be
   object-like macros expanding to who-knows-what.

   To this end, this header defines them as object-like macros which expand
   to themselves, so that if any other header included in the same
   translation unit also defines them as macros, this will cause a constraint
   violation and a diagnostic will be issued. Outside the preprocessor you
   should see no effects from this.

   If you have to include another header that either requires these
   identifiers to be undefined (for an ifdef, say), or which defines them as
   function-like (and not-object-like) macros, you can safely disable these
   protective definitions. Define the macro identifier
   DIE_NO_PROTECT_KEYWORDS before you include this header, and the
   definitions will be skipped.

   If, for some godforsaken reason, you NEED to define these identifiers as
   object-like macros, you can disable keyword protection and use the
   alternate keywords with a `DIE_` prefix, like so:

   DIE(DIE_who,malloc, DIE_what,"couldn't allocate string buffer");

   --------------------------------------------------------------------------

   Future versions of this header may define any number of additional macro
   identifiers under the reserved criteria, and any number of aliases
   beginning with TRY or FAIL without increasing the major version.

   --------------------------------------------------------------------------

   On hosted implementations, the expansion of the DIE macro is wrapped in an
   expression of the form ((...,0)?(free(0),0):0), which looks very silly,
   but was the only thing I could contrive that would suppress Clang and
   GCC's unused-values warning as an expression. Didn't work with different
   nesting, didn't work with any attributes, didn't work with anything I
   tried with unreachable.

   The branch with the free won't be taken, obviously, and even if it somehow
   is, it won't have any effect.

   Dead code analysis should have no problem eliminating this nonsense, but
   if you need to be certain, you can define DIE_NO_UNUSED_SUPPRESSION prior
   to including this header, and that wrapper will be elided. You can also
   provide an alternative wrapper by defining DIE_STFU yourself.

   --------------------------------------------------------------------------

   There are several macros and macro parameters herein that are required to
   be string literals. Unless otherwise specified, what I really mean by that
   is "one or more adjacent string literal tokens WITHOUT encoding prefixes".
   Just takes too long to say it every time.

   These are okay: `"abc"` or `"a" "b" "c"`.

   These aren't: `U"abc"`, `strcat(s,"blarg!")`, `(constexpr char[]){"foo"}`.

   An empty parameter or empty definition is also unacceptable.
 */

/* TODO:

   - TRY/FAIL macros for math errors:
     - HUGE_VAL &c.
     - FP_ILOGB0, FP_ILOGBNAN, FP_LLOGB0, FP_LLOGBNAN
     - some kind of fpclassify wrapper?

   - Macro like DIE_STDTRY(FN) that expands to the correct TRY identifier for
     the function identified by FN, for some reasonable percentage of the
     standard library? And same for FAIL.

   - In a million years when C2y comes out, it'll be great to be able to put
     static asserts in expressions without the anon struct trick.
 */

#ifndef DIE_H
#define DIE_H

/* DIE_VERSION: 0.2.5 */
#define DIE_VERSION 0x00'02'05UL

/*                           Configuration Flags
   --------------------------------------------------------------------------

   The following macros can be defined prior to including this header to
   configure it. Their definitions don't matter, only whether they're defined
   at all.

   Portable code which defines any of these macros should not rely on the
   replacement lists being the same after including this header.
 */

/* DIE_NOTHING_FANCY: master opt-out switch for all questionable features.

   If defined, DIE_NO_ALIASES, DIE_NO_PROTECT_KEYWORDS, DIE_NO_STD_HEADERS,
   DIE_NO_UNUSED_SUPPRESSION, and DIE_NO_WARNING will each be defined if they
   weren't already. */
#ifdef DIE_NOTHING_FANCY
    #undef  DIE_NOTHING_FANCY
    #define DIE_NOTHING_FANCY
#endif

/* DIE_NO_ALIASES: do not define any non-reserved identifiers.

   If defined, aliases like `TRY` for `DIE_TRY` and `FAIL_NEOF` for
   `DIE_FAIL_NEOF` will not be defined. DIE_NO_PROTECT_KEYWORDS will also be
   defined, if it wasn't already.

   This will become the default behavior in 0.3.0, and aliases will require
   an explicit opt-in.
 */
#ifdef DIE_NO_ALIASES
    #undef  DIE_NO_ALIASES
    #define DIE_NO_ALIASES
#endif

/* DIE_NO_PROTECT_KEYWORDS: do not define keywords as object-like macros.

   If defined, `who`, `what`, `out`, `how`, `code` and `where` will not
   defined as object-like macros expanding into themselves. */
#ifdef DIE_NO_PROTECT_KEYWORDS
    #undef  DIE_NO_PROTECT_KEYWORDS
    #define DIE_NO_PROTECT_KEYWORDS
#endif

/* DIE_NO_STD_HEADERS: don't include any standard headers.

   If you aren't planning on including <stdlib.h> yourself, you should also
   define DIE_NO_UNUSED_SUPPRESSION to avoid references to the free()
   function when instantiating the DIE macro. */
#ifdef DIE_NO_STD_HEADERS
    #undef  DIE_NO_STD_HEADERS
    #define DIE_NO_STD_HEADERS
#endif

/* DIE_NO_UNUSED_SUPPRESSION: don't attempt to suppress -Wunused-value.

   If defined, the DIE macro will not wrap its expansion in
   (...?(free(0),0):0).

   If __STDC_HOSTED__ == 0, this will be defined if it wasn't already.
 */
#ifdef DIE_NO_UNUSED_SUPPRESSION
    #undef  DIE_NO_UNUSED_SUPPRESSION
    #define DIE_NO_UNUSED_SUPPRESSION
#endif

/* DIE_NO_WARNING: don't give any #errors or #warnings.

   If defined, all the #warning and #error directives that would be issued
   when environmental macros seem to indicate the implementation won't
   support this header will be bypassed. */
#ifdef DIE_NO_WARNING
    #undef  DIE_NO_WARNING
    #define DIE_NO_WARNING
#endif

#ifdef DIE_NOTHING_FANCY
    #define DIE_NO_ALIASES
    #define DIE_NO_PROTECT_KEYWORDS
    #define DIE_NO_STD_HEADERS
    #define DIE_NO_UNUSED_SUPPRESSION
    #define DIE_NO_WARNING
#endif

#ifdef DIE_NO_ALIASES
    #define DIE_NO_PROTECT_KEYWORDS
#endif

#ifndef DIE_NO_WARNING
    #if !defined(__STDC__) || __STDC__ != 1
        #warning die.h requires C23 or later (__STDC_VERSION__ >= 202311L)
    #endif

    #if __STDC_VERSION__ <= 201710L
        #error die.h requires C23 or later (__STDC_VERSION__ >= 202311L)
    #elif __STDC_VERSION__ < 202311L
        #warning die.h requires C23 or later (__STDC_VERSION__ >= 202311L)
    #endif

    #if !defined(__STDC_HOSTED__) \
        || __STDC_HOSTED__ > 1 || __STDC_HOSTED__ < 0
        #error die.h expects __STDC_HOSTED__ to be defined and either 0 or 1
    #endif
#endif

#if __STDC_HOSTED__ == 0
    #define DIE_NO_UNUSED_SUPPRESSION
#endif

#ifndef DIE_NO_STD_HEADERS
    #if __STDC_HOSTED__ == 1
        #include <errno.h>
        #include <fenv.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <stddef.h>
    #elif __STDC_HOSTED__ == 0
        #include <stddef.h>
    #endif
#endif

/* DIE(): produce error message and terminate.

   DIE() accepts a list of keyword parameters, and can be used either as an
   expression statement like `if (fail) DIE();`, or in a logical operator
   expression like `(a != b) || DIE();`. The TRY*() and FAIL*() families of
   function-like macro predicates defined in an upcoming section are provided
   to make common examples of the latter a bit terser. "try p = malloc or
   die".

   All parameters are optional and can be passed in any order. Keywords may
   optionally be written with the prefix `DIE_`, like `DIE_who`. There is no
   difference between the spellings. If a keyword is specified more than
   once, the last one "wins". The argument processing loop can only run a
   bounded number of times, however, so I only guarantee that you can have
   twice as many key-value argument pairs as there are keywords. (i.e. 24
   total arguments.) Any more arguments, and the instantiation may not fully
   expand. The maximum supported argument count will never decrease between
   minor versions.

   Each parameter also has a default keyword that will be assumed if you just
   put a comma, like DIE(,sqrt,,"NaN"). The list of supported keywords
   describes them in the order in which they are assumed. (The first
   parameter is assumed to be `who`, the second is `what`, &c.)

   The default value of each parameter comes from a preprocessor macro of the
   form DIE_*_DEFAULT, e.g. DIE_WHO_DEFAULT, DIE_HOW_DEFAULT, etc. You can
   define these prior to including this header, or un-define and then
   re-define them at any time after including this header, to change the
   defaults for long runs of macro instantiations.

   Unlike keywords, the value of each parameter (and the default value macro)
   must never be empty.

   The intent with DIE is that most fatal error handlers need to do one or
   two things different from every other handler, and usually only one or two
   things, but often not the same one or two things. Thus, keyword arguments.
   Set sane defaults and, fingers crossed, every instantiation should be
   shorter and more readable than the code it expands into.

                                  Examples:

   DIE() expands to: ((perror("foo.c:3"),exit(EXIT_FAILURE),0)?(free(0),0):0)

                                     ---

   Note: the internal machinery underlying this macro is... complicated. If
   you mess up the syntax, or get a warning on the result of the expansion,
   it's not the end of the world, but the compiler diagnostics are going to
   be hard to read. Sorry about that. Also, passing anything like unbalanced
   parentheses, or commas in weird places, or invalid keywords, or
   object-like macros expanding to touchy characters will cause the expansion
   to go seriously awry.

   Don't let that discourage you, though! This isn't a live grenade, you
   don't have to be on edge all the time. Just, y'know, understand that -Wall
   probably won't be very helpful if you make a typo inside.

                                     ---

                             List of Parameters
   --------------------------------------------------------------------------

   who: the identifier of the callee that returned an error.

   It is expected but not required that this is an ordinary identifier, macro
   identifier, or perhaps label identifier representing the source of the
   error the instantiator of the DIE macro is responding to. This will be
   stringized (after some amount of unavoidable macro expansion) and prefixed
   with ": ", unless no value is supplied, in which case this will expand to
   an empty string.

   Practically, though, it can be anything the preprocessor considers a
   single argument (separated by commas not inside matching parentheses).

                                  Examples:

   DIE(who,recv) expands to:
   ((perror("foo.c:3: recv"),exit(EXIT_FAILURE),0)?(free(0),0):0)

                                     ---

   I would advise against overriding the default for this parameter (by
   un-defining and then re-defining DIE_WHO_DEFAULT). If you do so anyway, it
   must expand to a string literal, preferably prefixed with ": ". The macro
   machinery assumes it doesn't have to stringize or prefix the default.

   --------------------------------------------------------------------------

   what: a string literal containing additional context for the error.

   This will be prefixed with ": ", unless no value is supplied, in which
   case this will expand to an empty string.

                                  Examples:

   DIE(what,"double free") expands to:
   ((perror("foo.c:3: double free"),exit(EXIT_FAILURE),0)?(free(0),0):0)

                                     ---

   If you choose to override the default for this parameter by un-defining
   and then re-defining DIE_WHAT_DEFAULT to something that isn't an empty
   string, I would suggest prefixing it with ": " yourself -- the macro
   machinery always assumes it shouldn't modify the default value.

   --------------------------------------------------------------------------

   out: the name of a function or function-like macro to call to print a
   diagnostic; or the same followed by a list of arguments -- all separated
   by commas -- enclosed in parentheses.

   Defaults to perror.

   In either form, a string literal composed by concatenating the where, who,
   and what parameters with delimiters will be passed as the first argument
   (or only argument) to the function identified by out.

   If the list form is used, it should not end with a trailing comma. Neither
   I nor the C preprocessor know how many arguments you actually meant to
   pass, and we will assume that you knew what you were doing with an empty
   trailing preprocessor argument.

                                  Examples:

   DIE(out,my_log) expands to:
   ((my_log("foo.c:3"),exit(EXIT_FAILURE),0)?(free(0),0):0)

   DIE(out,(my_log)) expands to the same:
   ((my_log("foo.c:3"),exit(EXIT_FAILURE),0)?(free(0),0):0)

   DIE(out,(my_log,x,)) expands to:
   ((my_log("foo.c:3",x,),exit(EXIT_FAILURE),0)?(free(0),0):0)

   DIE(what,"%s", out,(DIE_epf,err[e])) expands to:
   ((fprintf(stderr,"foo.c:3: %s\n",err[e]),exit(EXIT_FAILURE),0)?(free(0),0):0)

                                     ---

   In a freestanding implementation, where __STDC_HOSTED__ == 0 and perror
   may not be available, the default DIE_OUT_DEFAULT is not defined. You must
   define it yourself before instantiating the DIE macro.

                                     ---

   Note that many functions do not set errno on failure, or in all failure
   cases, so perror may report stale or unhelpful information. Consider your
   default wisely.

                                     ---

   The function-like macros DIE_epf and DIE_ep are provided as convenient
   wrappers around fprintf(stderr,...) and fputs(...,stderr) for use as the
   value of the out parameter. Also, see DIE_elt, which is meant for use with
   the out parameter, too.

   --------------------------------------------------------------------------

   how: the name of a function or function-like macro to call to return an
   error code; or the same followed by a list of arguments -- all separated
   by commas -- enclosed in parentheses.

   Defaults to exit.

   If the list form is used, it should not end with a trailing comma. Neither
   I nor the C preprocessor know how many arguments you actually meant to
   pass, and we will assume that you knew what you were doing with an empty
   trailing preprocessor argument.

   In either form, the termination function must be one which cannot return
   to the caller. This isn't enforced, but I'm not sure what you're doing
   here if you try.

   The code parameter (see next) will be passed as the last argument (or only
   argument) to the function identified by how.

                                  Examples:

   DIE(how,thrd_exit) expands to:
   ((perror("foo.c:3"),thrd_exit(EXIT_FAILURE),0)?(free(0),0):0)

   DIE(how,(thrd_exit)) expands to the same:
   ((perror("foo.c:3"),thrd_exit(EXIT_FAILURE),0)?(free(0),0):0)

   DIE(how,(thrd_exit,)) expands to:
   ((perror("foo.c:3"),thrd_exit(,EXIT_FAILURE),0)?(free(0),0):0)

   DIE(how,(longjmp,env)) expands to:
   ((perror("foo.c:3"),longjmp(env,EXIT_FAILURE),0)?(free(0),0):0)

   DIE(how,(my_exit_fn,x,y,z)) expands to:
   ((perror("foo.c:3"),my_exit_fn(x,y,z,EXIT_FAILURE),0)?(free(0),0):0)

   DIE(how,DIE_abort) expands to:
   ((perror("foo.c:3"),abort(),0)?(free(0),0):0)

                                     ---

   The function-like macro DIE_abort is provided as a convenient way to
   swallow the exit code, and just expand to `abort()` with no arguments.

                                     ---

   Note that the "termination" function need not actually terminate the
   process, it just has to be something `[[noreturn]]`. As shown in the
   examples, thrd_exit and longjmp may both be good options for errors which
   are only semi-fatal.

                                     ---

   In a freestanding implementation, where __STDC_HOSTED__ == 0 and exit may
   not be available, the default DIE_HOW_DEFAULT is not defined. You must
   define it yourself before instantiating the DIE macro.

   --------------------------------------------------------------------------

   code: the final argument to pass to the function identified by the how
   parameter.

   Defaults to EXIT_FAILURE.

                                  Examples:

   DIE(code,3) expands to: ((perror("foo.c:3"),exit(3),0)?(free(0),0):0)

                                     ---

   If your termination function of choice takes no code argument, such as
   abort, or takes the code argument somewhere other than the last parameter,
   do not use an empty code parameter or empty DIE_CODE_DEFAULT. Instead,
   consider a function-like macro wrapper around the termination function:

   #define MY_TERM_FN(FOO,BAR,CODE) my_term_fn(FOO,CODE,BAR)
   DIE(how,(MY_TERM_FN,foo,bar));

   #define DIE_abort(...) abort()
   DIE(how,DIE_abort);

   DIE_abort is in fact already provided for this purpose.

                                     ---

   In a freestanding implementation, where __STDC_HOSTED__ == 0 and
   EXIT_FAILURE may not be defined, the default is the token `1`.

   --------------------------------------------------------------------------

   where: a string literal with an identifying location or source to
   associate with the diagnostic.

   Defaults to (essentially) "__FILE__:__LINE__", like "foo.c:3".

                                  Examples:

   DIE(where,"usage", what,"foo [-R] [filename]") expands to:
   ((perror("usage: foo [-R] [filename]"),exit(EXIT_FAILURE),0)?(free(0),0):0)

                                     ---

   You may wonder why DIE_WHERE_DEFAULT references __FILE__ and __LINE__ but
   not __func__. Two reasons. First, __func__ isn't a macro. It's just a
   magic identifier for a char[] that's always defined in every function
   body. So we'd need to do some tricks with strcat to load it into the
   diagnostic string.

   Second, it's actually possible to DIE outside of a function body. Here's a
   no-joke standard-compliant example:

   int foo(
       long n,
       char s[const static (TRY(n = sysconf(_SC_PAGESIZE)) || DIE(,sysconf), n)]
   ) {
       // ...
   }

   Believe it or not, this is a legal function definition (assuming
   reasonable definitions of sysconf and _SC_PAGESIZE), that expression
   really does execute, and the __func__ identifier is NOT in scope when it
   does.

   So I chose to play it safe and ignore __func__.
 */
#define DIE(...) DIE3(DIE5(DIE0(2,DIE_WHO_DEFAULT,DIE_WHAT_DEFAULT,DIE_OUT_DEFAULT,DIE_HOW_DEFAULT,DIE_CODE_DEFAULT,DIE_WHERE_DEFAULT,__VA_ARGS__)))
#define DIE0(N,A,B,C,D,E,F,...) DIE1##__VA_OPT__(1)(N,A,B,C,D,E,F __VA_OPT__(,)__VA_ARGS__)
#define DIE00() DIE0
#define DIE1(_,WHO,WHAT,OUT,HOW,CODE,WHERE,...) DIE3A DIE_STFU(DIE66((DIE9F(WHERE WHO WHAT,OUT),DIE9(CODE,HOW),0)))
#define DIE11(N,A,B,C,D,E,F,KEY,...) DIE111##__VA_OPT__(1)(N,A,B,C,D,E,F,KEY __VA_OPT__(,)__VA_ARGS__)
#define DIE111(...) DIE3_odd_number_of_arguments
#define DIE1111(N,A,B,C,D,E,F,KEY,VAL,...) DIE2##KEY(N##2,VAL,A,B,C,D,E,F,__VA_ARGS__)
#define DIE2(N,...) DIE##N(N,__VA_ARGS__)

#if !defined(DIE_WHO_DEFAULT)
    #define DIE_WHO_DEFAULT ""
#endif
#define DIE2who(N,V,A,...) DIE00 DIE7()()(N,": "#V,__VA_ARGS__)
#define DIE2DIE_who(N,V,A,...) DIE00 DIE7()()(N,": "#V,__VA_ARGS__)
#define DIE22(N,V,A,...) DIE00 DIE7()()(N,": "#V,__VA_ARGS__)

#if !defined(DIE_WHAT_DEFAULT)
    #define DIE_WHAT_DEFAULT ""
#endif
#define DIE2what(N,V,A,B,...) DIE00 DIE7()()(N,A,": "V,__VA_ARGS__)
#define DIE2DIE_what(N,V,A,B,...) DIE00 DIE7()()(N,A,": "V,__VA_ARGS__)
#define DIE222(N,V,A,B,...) DIE00 DIE7()()(N,A,": "V,__VA_ARGS__)

#if !defined(DIE_OUT_DEFAULT) && __STDC_HOSTED__ == 1
    #define DIE_OUT_DEFAULT perror
#endif
#define DIE2out(N,V,A,B,C,...) DIE00 DIE7()()(N,A,B,V,__VA_ARGS__)
#define DIE2DIE_out(N,V,A,B,C,...) DIE00 DIE7()()(N,A,B,V,__VA_ARGS__)
#define DIE2222(N,V,A,B,C,...) DIE00 DIE7()()(N,A,B,V,__VA_ARGS__)

#if !defined(DIE_HOW_DEFAULT) && __STDC_HOSTED__ == 1
    #define DIE_HOW_DEFAULT exit
#endif
#define DIE2how(N,V,A,B,C,D,...) DIE00 DIE7()()(N,A,B,C,V,__VA_ARGS__)
#define DIE2DIE_how(N,V,A,B,C,D,...) DIE00 DIE7()()(N,A,B,C,V,__VA_ARGS__)
#define DIE22222(N,V,A,B,C,D,...) DIE00 DIE7()()(N,A,B,C,V,__VA_ARGS__)

#if !defined(DIE_CODE_DEFAULT) && __STDC_HOSTED__ == 1
    #define DIE_CODE_DEFAULT EXIT_FAILURE
#elif !defined(DIE_CODE_DEFAULT)
    #define DIE_CODE_DEFAULT 1
#endif
#define DIE2code(N,V,A,B,C,D,E,...) DIE00 DIE7()()(N,A,B,C,D,V,__VA_ARGS__)
#define DIE2DIE_code(N,V,A,B,C,D,E,...) DIE00 DIE7()()(N,A,B,C,D,V,__VA_ARGS__)
#define DIE222222(N,V,A,B,C,D,E,...) DIE00 DIE7()()(N,A,B,C,D,V,__VA_ARGS__)

#if !defined(DIE_WHERE_DEFAULT)
    #define DIE_WHERE_DEFAULT __FILE__":"DIE_ES(__LINE__)
#endif
#define DIE2where(N,V,A,B,C,D,E,F,...) DIE00 DIE7()()(N,A,B,C,D,E,V,__VA_ARGS__)
#define DIE2DIE_where(N,V,A,B,C,D,E,F,...) DIE00 DIE7()()(N,A,B,C,D,E,V,__VA_ARGS__)
#define DIE2222222(N,V,A,B,C,D,E,F,...) DIE00 DIE7()()(N,A,B,C,D,E,V,__VA_ARGS__)

#define DIE22222222(...) DIE3_too_many_positional_arguments
#define DIE222222222(...) DIE3_too_many_positional_arguments
#define DIE2222222222(...) DIE3_too_many_positional_arguments
#define DIE22222222222(...) DIE3_too_many_positional_arguments
#define DIE222222222222(...) DIE3_too_many_positional_arguments
#define DIE2222222222222(...) DIE3_too_many_positional_arguments
#define DIE3(...)  DIE33(__VA_ARGS__)
#define DIE33(...)  DIE3##__VA_ARGS__
#define DIE3A DIE3A
#define DIE3DIE3A(...) (__VA_ARGS__)
#define DIE3DIE00(...) DIE3DIE33
#define DIE3DIE33(...) DIE3_too_many_arguments
#define DIE3_odd_number_of_arguments DIE3_odd_number_of_arguments
#define DIE3_too_many_positional_arguments DIE3_too_many_positional_arguments
#define DIE3DIE3_odd_number_of_arguments DIE3_odd_number_of_arguments
#define DIE3DIE3_too_many_positional_arguments DIE3_too_many_positional_arguments
#define DIE3_too_many_arguments DIE3_too_many_arguments
#define DIE4STATIC_ASSERT(...) static_assert(__VA_ARGS__)
#define DIE5(...) DIE55(DIE55(DIE55(DIE55(__VA_ARGS__))))
#define DIE55(...) DIE6(DIE6(DIE6(DIE6(__VA_ARGS__))))
#define DIE6(...) __VA_ARGS__
#define DIE66(...) __VA_ARGS__
#define DIE7()
#define DIE9(C,...) DIE9D DIE7 DIE7()()(DIE9C(__VA_ARGS__),C)
#define DIE9F(S,...) DIE9E DIE7 DIE7()()(S,DIE9C(__VA_ARGS__))
#define DIE9D(F,...) F(__VA_ARGS__)
#define DIE9E(S,F,...) F(S __VA_OPT__(,)__VA_ARGS__)
#define DIE9C(...) DIE9B DIE7()(DIE9A(__VA_ARGS__))
#define DIE9A(...) DIE9AA __VA_ARGS__
#define DIE9AA(...) DIE9AAA __VA_ARGS__
#define DIE9B(...) DIE9BB##__VA_ARGS__
#define DIE9BBDIE9AA
#define DIE9BBDIE9AAA

#define DIE_who   DIE_who
#define DIE_what  DIE_what
#define DIE_out   DIE_out
#define DIE_how   DIE_how
#define DIE_code  DIE_code
#define DIE_where DIE_where
#define DIE9AAA   DIE9AAA
#ifndef DIE_NO_PROTECT_KEYWORDS
    #define who   who
    #define what  what
    #define out   out
    #define how   how
    #define code  code
    #define where where
#endif /* DIE_NO_PROTECT_KEYWORDS */

/*                             Utility Macros
   --------------------------------------------------------------------------

   These macros are provided as shorthands for common DIE patterns.
 */

/* DIE_epf(): error print formatted.

   Expands into an fprintf to stderr call, suitable for use as the out
   parameter of DIE. S must be a string literal. A newline is automatically
   appended to S via string literal concatenation. All other arguments become
   subsequent arguments for fprintf.

                                  Examples:

   (*e == '\0')
   || DIE(what,"%s", out,(DIE_epf,s))

   expands to:

   (*e == '\0')
   || ((fprintf(stderr,"foo.c:3: %s\n",s),exit(EXIT_FAILURE),0)?(free(0),0):0)

                                     ---

   Note this may have undefined behavior in the case that the format string
   literal contains percent characters, either from the filename or one's own
   parameter values.
 */
#define DIE_epf(S,...) fprintf(stderr,S"\n"__VA_OPT__(,)__VA_ARGS__)

/* DIE_elt(): error from look-up table.

   This macro expands to a value suitable for use as the out parameter for
   DIE, where fprintf will be used to display a description of an error from
   another function or LUT. It is not meaningful elsewhere.

   DIE_elt() takes one argument, an expression producing a string, e.g.
   `DIE(out,DIE_elt(strerror(errno)))`.

                                     ---

   Note this may have undefined behavior in the case that the format string
   literal contains percent characters, either from the filename or one's own
   parameter values.
 */
#define DIE_elt(...) (DIE8_elt,(__VA_ARGS__))
#define DIE8_elt(S,...) fprintf(stderr,S": %s\n"__VA_OPT__(,)__VA_ARGS__)

/* DIE_ep(): error put.

   Expands into an fputs to stderr call, suitable for use as the out
   parameter of DIE. S must be a string literal. A newline is automatically
   appended to S via string literal concatenation. No other arguments should
   be provided.

                                  Examples:

   DIE(what,"not found", out,DIE_ep) expands to:
   ((fputs("foo.c:3: not found\n",stderr),exit(EXIT_FAILURE),0)?(free(0),0):0)
 */
#define DIE_ep(S,...) fputs(S"\n"__VA_OPT__(,)__VA_ARGS__,stderr)

/* DIE_abort(): die by aborting.

   Ignores all arguments and expands to `abort()`. Suitable for the how
   parameter of DIE, as abort does not take a return code. */
#define DIE_abort(...) abort()

/* DIE_STFU(): suppress the foolish -Wunused-value.

   Takes a single argument, which should be an expression of the form
   `(...,0)`.

   If DIE_NO_UNUSED_SUPPRESSION is defined when this header is included,
   either by defining it explicitly or by defining another configuration
   macro which implies it, this will be defined as a no-op that simply
   expands its arguments.

   It is part of the public API to allow you to piggy-back off of it, or
   redefine it to something more suitable for your implementation.

   If DIE_STFU is already defined prior to including this header, the
   previous definition will be kept.
 */
#ifndef DIE_STFU
    #ifndef DIE_NO_UNUSED_SUPPRESSION
        #define DIE_STFU(...) (__VA_ARGS__?(free(0),0):0)
    #else
        #define DIE_STFU(...) __VA_ARGS__
    #endif
#endif

/* DIE_ES(): macro expand argument and stringize the result.

   This is used in the default definition of DIE_WHERE_DEFAULT, it is
   provided for the public in case anyone downstream wants to piggy-back off
   of it. */
#define DIE_ES(X) DIE8_ES(X)
#define DIE8_ES(X) #X

/*                               Predicates
   --------------------------------------------------------------------------

   The following macros are provided as semantic wrappers for some common
   error-return conventions.

   Unless otherwise noted, each macro takes a single expression as a
   parameter, and expands to an expression translating the result(s) of that
   expression into a truthy or falsy value. TRY macros will be true on
   success and false on failure, and FAIL macros will be false on success and
   true on failure.

   This allows writing things like `TRYA(buf = malloc(n)) || DIE();` or
   `if (FAIL(bytes_sent = send(sock, buf, n, 0))) DIE();`.

   You will need to use the correct predicate for a given expression if you
   want the correct interpretation of its result, of course.

   In an attempt to avoid namespace pollution, each predicate macro is
   defined with the prefix DIE_. An object-like macro "alias" without this
   prefix is defined for every predicate, unless the macro DIE_NO_ALIASES is
   defined prior to including this header.

   All TRY and FAIL macros evaluate each (non-constant) parameter only once.
   Constant parameters may be evaluated multiple times at translation time.
   Parameters may also be macro expanded more than once, though most aren't.
 */

/* TRY(), FAIL(): try something that will only return -1 on failure.

   This is "the usual convention".
 */
#define DIE_TRY(...) ((__VA_ARGS__)!=-1)
#define DIE_FAIL(...) ((__VA_ARGS__)==-1)
#ifndef DIE_NO_ALIASES
    #define TRY DIE_TRY
    #define FAIL DIE_FAIL
#endif

/* TRY0(), FAIL0(): try something that should only return 0 on success. */
#define DIE_TRY0(...) ((__VA_ARGS__)==0)
#define DIE_FAIL0(...) ((__VA_ARGS__)!=0)
#ifndef DIE_NO_ALIASES
    #define TRY0 DIE_TRY0
    #define FAIL0 DIE_FAIL0
#endif

/* TRY_NNEG(), FAIL_NNEG(): try something that fails negative.

   A static assertion in an anonymous struct is used to guard against using
   this predicate with expressions of unsigned type. Until C2y, this
   unfortunately makes it unsuitable for underspecified initializers. */
#define DIE_TRY_NNEG(...) (_Generic((__VA_ARGS__),struct{DIE4STATIC_ASSERT((typeof(__VA_ARGS__))0>(typeof(__VA_ARGS__))(-1));char _;}:0,default:(__VA_ARGS__)>=0))
#define DIE_FAIL_NNEG(...) (_Generic((__VA_ARGS__),struct{DIE4STATIC_ASSERT((typeof(__VA_ARGS__))0>(typeof(__VA_ARGS__))(-1));char _;}:0,default:(__VA_ARGS__)<0))
#ifndef DIE_NO_ALIASES
    #define TRY_NNEG DIE_TRY_NNEG
    #define FAIL_NNEG DIE_FAIL_NNEG
#endif

/* TRYA(), FAILA(): try allocating */
#define DIE_TRYA(...) ((__VA_ARGS__)!=nullptr)
#define DIE_FAILA(...) ((__VA_ARGS__)==nullptr)
#ifndef DIE_NO_ALIASES
    #define TRYA DIE_TRYA
    #define FAILA DIE_FAILA
#endif

/* TRYS(), FAILS(): try something that should return a pointer to something
   owned by the environment.

   For example, setlocale. Stuff that returns a pointer you shouldn't ever
   free or close. This has the same definition as TRYA, but a different name,
   to avoid semantic confusion. */
#define DIE_TRYS(...) ((__VA_ARGS__)!=nullptr)
#define DIE_FAILS(...) ((__VA_ARGS__)==nullptr)
#ifndef DIE_NO_ALIASES
    #define TRYS DIE_TRYS
    #define FAILS DIE_FAILS
#endif

/* TRYE(), FAILE(): zero errno, try something, discard result, test errno.

   For functions like strtol, that communicate success/failure by setting or
   not setting errno. (Note that strtol also requires checking endptr.) */
#define DIE_TRYE(...) (errno=0,(__VA_ARGS__),errno==0)
#define DIE_FAILE(...) (errno=0,(__VA_ARGS__),errno!=0)
#ifndef DIE_NO_ALIASES
    #define TRYE DIE_TRYE
    #define FAILE DIE_FAILE
#endif

/* TRYIO(), FAILIO(): do either of the two most common standard I/O tasks.

   Most standard library I/O functions either return a FILE* pointer on
   success and NULL for errors, or a non-negative int on success and some
   negative value (often EOF) for errors. This macro simplifies these two
   common cases by delegating expressions of type FILE* to TRY_OPN/FAIL_OPN
   and all others to TRY_NNEG/FAIL_NNEG (under the assumption they're an
   integer type of some kind).

   Note that this is not an appropriate predicate for fread, fwrite, or
   related functions that return *unsigned* integers. I'd whitelist signed
   integer types if I could, but I don't want to lock anyone out of most (or
   all) _BitInts.
 */
#define DIE_TRYIO(...) _Generic((__VA_ARGS__),FILE*:DIE_TRY_OPN(_Generic((__VA_ARGS__),FILE*:(__VA_ARGS__),default:0)),default:DIE_TRY_NNEG(_Generic((__VA_ARGS__),FILE*:0,default:(__VA_ARGS__))))
#define DIE_FAILIO(...) _Generic((__VA_ARGS__),FILE*:DIE_FAIL_OPN(_Generic((__VA_ARGS__),FILE*:(__VA_ARGS__),default:0)),default:DIE_FAIL_NNEG(_Generic((__VA_ARGS__),FILE*:0,default:(__VA_ARGS__))))
#ifndef DIE_NO_ALIASES
    #define TRYIO DIE_TRYIO
    #define FAILIO DIE_FAILIO
#endif

/* TRYRD(), FAILRD(): try reading from a stream, accepting EOF.

   Functions like fgetc return EOF on error and also upon successfully
   reaching the end of the file. This macro takes an additional leading
   argument, a stream, whose error indicator will be cross-referenced if the
   expression evaluates to EOF. */
#define DIE_TRYRD(S,...) ((__VA_ARGS__)!=EOF||!ferror(S))
#define DIE_FAILRD(S,...) ((__VA_ARGS__)==EOF&&ferror(S))
#ifndef DIE_NO_ALIASES
    #define TRYRD DIE_TRYRD
    #define FAILRD DIE_FAILRD
#endif

/* TRY_WRD(), FAIL_WRD(): try reading from a wide stream, accepting WEOF.

   You will need to include a header that defines WEOF before instantiating
   this macro. */
#define DIE_TRY_WRD(S,...) ((__VA_ARGS__)!=WEOF||!ferror(S))
#define DIE_FAIL_WRD(S,...) ((__VA_ARGS__)==WEOF&&ferror(S))
#ifndef DIE_NO_ALIASES
    #define TRY_WRD DIE_TRY_WRD
    #define FAIL_WRD DIE_FAIL_WRD
#endif

/* TRY_OPN(), FAIL_OPN(): try opening a file.

   This also has the same definition as TRYA, but a different name, to avoid
   semantic confusion. It is meant for functions returning FILE* pointers. */
#define DIE_TRY_OPN(...) ((__VA_ARGS__)!=nullptr)
#define DIE_FAIL_OPN(...) ((__VA_ARGS__)==nullptr)
#ifndef DIE_NO_ALIASES
    #define TRY_OPN DIE_TRY_OPN
    #define FAIL_OPN DIE_FAIL_OPN
#endif

/* TRY_NEOF(), FAIL_NEOF(): try something that returns EOF on error. */
#define DIE_TRY_NEOF(...) ((__VA_ARGS__)!=EOF)
#define DIE_FAIL_NEOF(...) ((__VA_ARGS__)==EOF)
#ifndef DIE_NO_ALIASES
    #define TRY_NEOF DIE_TRY_NEOF
    #define FAIL_NEOF DIE_FAIL_NEOF
#endif

/* TRY_NWEOF(), FAIL_NWEOF(): try something that returns WEOF on error. */
#define DIE_TRY_NWEOF(...) ((__VA_ARGS__)!=WEOF)
#define DIE_FAIL_NWEOF(...) ((__VA_ARGS__)==WEOF)
#ifndef DIE_NO_ALIASES
    #define TRY_NWEOF DIE_TRY_NWEOF
    #define FAIL_NWEOF DIE_FAIL_NWEOF
#endif

/* TRY_SIG(), FAIL_SIG(): try something that returns SIG_ERR on error.

   You will need to include a header that defines SIG_ERR before
   instantiating this macro. */
#define DIE_TRY_SIG(...) ((__VA_ARGS__)!=SIG_ERR)
#define DIE_FAIL_SIG(...) ((__VA_ARGS__)==SIG_ERR)
#ifndef DIE_NO_ALIASES
    #define TRY_SIG DIE_TRY_SIG
    #define FAIL_SIG DIE_FAIL_SIG
#endif

/* TRY_UCNV(), FAIL_UCNV(): try a uchar.h conversion.

   Failure is interpreted as a return value of (size_t)(-1). */
#define DIE_TRY_UCNV(...) ((__VA_ARGS__)!=(size_t)(-1))
#define DIE_FAIL_UCNV(...) ((__VA_ARGS__)==(size_t)(-1))
#ifndef DIE_NO_ALIASES
    #define TRY_UCNV DIE_TRY_UCNV
    #define FAIL_UCNV DIE_FAIL_UCNV
#endif

/* TRY_UCNV2(), FAIL_UCNV2(): try a uchar.h conversion. (extended)

   Unlike TRY_UCNV(), these consider (size_t)(-1) and (size_t)(-2) to both be
   failure conditions, and as a side effect they will unconditionally set
   errno to EILSEQ in the case of either failure. */
#define DIE_TRY_UCNV2(...) ((((__VA_ARGS__)&(size_t)(-2))!=(size_t)(-2))||(errno=EILSEQ,0))
#define DIE_FAIL_UCNV2(...) ((((__VA_ARGS__)&(size_t)(-2))==(size_t)(-2))&&(errno=EILSEQ,1))
#ifndef DIE_NO_ALIASES
    #define TRY_UCNV2 DIE_TRY_UCNV2
    #define FAIL_UCNV2 DIE_FAIL_UCNV2
#endif

/* TRYT(), FAILT(): do something thread-related, thrd_success means success.

   You will need to include a header that defines thrd_success before
   instantiating this macro. */
#define DIE_TRYT(...) ((__VA_ARGS__)==thrd_success)
#define DIE_FAILT(...) ((__VA_ARGS__)!=thrd_success)
#ifndef DIE_NO_ALIASES
    #define TRYT DIE_TRYT
    #define FAILT DIE_FAILT
#endif

/* TRY_CLK(), FAIL_CLK(): try calling clock().

   Or any other function that returns the sentinel (clock_t)(-1) in case of
   error, clock() is just the only example in the standard library. You will
   need to include a header that defines clock_t before instantiating this
   macro. */
#define DIE_TRY_CLK(...) ((__VA_ARGS__)!=(clock_t)(-1))
#define DIE_FAIL_CLK(...) ((__VA_ARGS__)==(clock_t)(-1))
#ifndef DIE_NO_ALIASES
    #define TRY_CLK DIE_TRY_CLK
    #define FAIL_CLK DIE_FAIL_CLK
#endif

/* TRY_TIM(), FAIL_TIM(): try calling a function w/ error sentinel (time_t)(-1).

   You will need to include a header that defines time_t before instantiating
   this macro. */
#define DIE_TRY_TIM(...) ((__VA_ARGS__)!=(time_t)(-1))
#define DIE_FAIL_TIM(...) ((__VA_ARGS__)==(time_t)(-1))
#ifndef DIE_NO_ALIASES
    #define TRY_TIM DIE_TRY_TIM
    #define FAIL_TIM DIE_FAIL_TIM
#endif

/* TRYNR(), FAILNR(): try calling a function that should not return.

   This is meant for things like POSIX exec where failure is obvious if
   there's still a caller to return to, and the return value doesn't contain
   any useful information. */
#define DIE_TRYNR(...) ((__VA_ARGS__),0)
#define DIE_FAILNR(...) ((__VA_ARGS__),1)
#ifndef DIE_NO_ALIASES
    #define TRYNR DIE_TRYNR
    #define FAILNR DIE_FAILNR
#endif

/* TRYM(), FAILM(): try doing math.

   If DIE_M_USE_ERRNO is true, these predicates will zero errno prior to
   evaluating the argument expression.

   If DIE_M_USE_EXCEPT is true, these predicates will run
   feclearexcept(DIE_M_FEFLAGS) prior to evaluating the argument expression.
   If this fails, the predicate will fail without evaluating the argument
   expression.

   See TRYMC and FAILMC for a version of this macro that doesn't clear
   errno or floating point exceptions.

   The following are considered failures for math functions:
   - the result is NaN, according to isnan
   - DIE_M_USE_ERRNO is true and errno is nonzero
   - DIE_M_USE_EXCEPT is true and fetestexcept returns nonzero.

   DIE_M_USE_ERRNO and DIE_M_USE_EXCEPT defer to the current value of
   math_errhandling by default, but they can be overridden to `true` or
   `false` to force a particular behavior. To do so, define one or both
   macros prior to including this header, or undefine and re-define them at
   any point after including this header to change them for subsequent
   instantiations of TRYM and FAILM.

   The flags checked by fetestexcept come from expanding the macro
   DIE_M_FEFLAGS. You can define this prior to including this header, or
   undefine and re-define it at any point after including this header to
   change it for subsequent instantiations of TRYM and FAILM. The default
   value of DIE_M_FEFLAGS is `FE_DIVBYZERO|FE_INVALID|FE_OVERFLOW` if all
   three are defined, else 0.

   You will need to include <fenv.h> prior to including this header to ensure
   FE_DIVBYZERO, FE_INVALID, and FE_OVERFLOW are already defined if
   available. (This will be done for you if __STDC_HOSTED__ == 1 and
   DIE_NO_STD_HEADERS is not defined.)

   TRYM and FAILM still take only one argument, which is the expression to
   test. Future versions of this macro may allow a second argument to choose
   the flags to test per predicate instantiation, but for the moment I
   presume you'll mostly just want to set that once for every instantiation.

   You will need to include headers that define isnan, math_errhandling,
   MATH_ERRNO, MATH_ERREXCEPT, fetestexcept, feclearexcept, and FE_ALL_EXCEPT
   before instantiating this macro.
 */
#define DIE_TRYM(...) (DIE_M_USE_ERRNO?(errno=0):0,(DIE_M_USE_EXCEPT?!feclearexcept(DIE_M_FEFLAGS):1)&&!isnan(__VA_ARGS__)&&!(DIE_M_USE_ERRNO&&errno)&&!(DIE_M_USE_EXCEPT&&fetestexcept(DIE_M_FEFLAGS)))
#define DIE_FAILM(...) (DIE_M_USE_ERRNO?(errno=0):0,(DIE_M_USE_EXCEPT?feclearexcept(DIE_M_FEFLAGS):0)||isnan(__VA_ARGS__)||(DIE_M_USE_ERRNO&&errno)||(DIE_M_USE_EXCEPT&&fetestexcept(DIE_M_FEFLAGS)))
#ifndef DIE_NO_ALIASES
    #define TRYM DIE_TRYM
    #define FAILM DIE_FAILM
#endif
#ifndef DIE_M_FEFLAGS
    #if defined(FE_DIVBYZERO) && defined(FE_INVALID) && defined(FE_OVERFLOW)
        #define DIE_M_FEFLAGS (FE_DIVBYZERO|FE_INVALID|FE_OVERFLOW)
    #else
        #define DIE_M_FEFLAGS 0
    #endif
#endif
#if !defined(DIE_M_USE_ERRNO)
    #define DIE_M_USE_ERRNO (math_errhandling&MATH_ERRNO)
#endif
#if !defined(DIE_M_USE_EXCEPT)
    #define DIE_M_USE_EXCEPT (math_errhandling&MATH_ERREXCEPT)
#endif

/* TRYMC(), FAILMC(): try doing math, but don't clear anything.

   These behave the same as TRYM and FAILM, with two differences:

   1. TRYMC and FAILMC do not zero errno or attempt to clear floating-point
      exceptions.

   2. TRYMC and FAILMC can be instantiated with an expression argument, like
     `FAILMC(sqrt(4.0))`; or zero arguments, like `TRYMC()`, which will
     expand to just checking errno and fetestexcept as appropriate. Note this
     latter form won't be of much use if neither errno or exceptions are
     enabled.

   You will need to include headers that define isnan, math_errhandling,
   MATH_ERRNO, MATH_ERREXCEPT, fetestexcept, and feclearexcept before
   instantiating this macro.
 */
#define DIE_TRYMC(...) (__VA_OPT__(!isnan(__VA_ARGS__)&&)!(DIE_M_USE_ERRNO&&errno)&&!(DIE_M_USE_EXCEPT&&fetestexcept(DIE_M_FEFLAGS)))
#define DIE_FAILMC(...) (__VA_OPT__(isnan(__VA_ARGS__)||)(DIE_M_USE_ERRNO&&errno)||(DIE_M_USE_EXCEPT&&fetestexcept(DIE_M_FEFLAGS)))
#ifndef DIE_NO_ALIASES
    #define TRYMC DIE_TRYMC
    #define FAILMC DIE_FAILMC
#endif

/* TRYEQ(), FAILEQ(): do something where the result must be equal to X.

   Takes two arguments: X followed by an expression. Success is when the
   result of the expression is equal to X.

   Intended for stuff like fread, where the return value should be equal to
   the item count, or else something went wrong.
 */
#define DIE_TRYEQ(X,...) ((__VA_ARGS__)==(X))
#define DIE_FAILEQ(X,...) ((__VA_ARGS__)!=(X))
#ifndef DIE_NO_ALIASES
    #define TRYEQ DIE_TRYEQ
    #define FAILEQ DIE_FAILEQ
#endif

#ifdef DIE4SELF_TEST
    /* Internal self-test. */

    /* TODO: Incomplete. These tests are written for a default, hosted
       configuration with no features disabled. */

    /* Tested on Linux x86-64 with GCC 16.2.1, Clang 23.1.3, and whatever slimcc
       version AUR package slimcc-git r921.ff77023-2 corresponds to. */

    /* Perform the self test by creating a file called foo.c (or whatever) with
       these contents:

       #define DIE4SELF_TEST
       #include "die.h"

       then compile foo.c, link with libm, and run.
     */

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

    #if DIE_VERSION != 0x00'02'05UL
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
           out,(fputs,stdout), what,"all tests passed!",
           code,EXIT_SUCCESS, how,exit
        );
    }
#endif /* DIE4SELF_TEST */
#endif /* DIE_H */
