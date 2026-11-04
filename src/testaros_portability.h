#ifndef TESTAROS_PORTABILITY_H
#define TESTAROS_PORTABILITY_H

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#  error "Testaros requiere C11 o posterior"
#elif __STDC_VERSION__ >= 202311L
#  define TR_THREAD_LOCAL  thread_local
#  define TR_NORETURN      [[noreturn]]
#  define TR_STATIC_ASSERT static_assert
#else
#  define TR_THREAD_LOCAL  _Thread_local
#  define TR_NORETURN      _Noreturn
#  define TR_STATIC_ASSERT _Static_assert
#endif

#endif
