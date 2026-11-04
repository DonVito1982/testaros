#ifndef MINITEST_H
#define MINITEST_H

#include <stdbool.h>
#include <stdio.h>

// todo archivo que incluya minitest.h debe definir setUp y tearDown (pueden ser
// vacías)

extern int ran_tests;
extern int failed_tests;
extern int skipped_tests;

void report(const char* filename);
void run_single_test(void (*test)(void), void (*setUp)(void),
                     void (*tearDown)(void), const char* funcname);

#define YELLOW(text) "\033[1;33m" text "\033[0m"
#define GREEN(text) "\033[1;32m" text "\033[0m"
#define RED(text) "\033[1;31m" text "\033[0m"

#define RUN_TEST(test) run_single_test(test, setUp, tearDown, #test)

#define SKIP_TEST()                          \
    do {                                     \
        printf(YELLOW(" prueba saltada\n")); \
        skipped_tests++;                     \
        return;                              \
    } while (0)

#define ASSERT_TRUE(expression)                                        \
    do {                                                               \
        if (expression != true) {                                      \
            printf(RED("\n  Fallo en %s:%d. Se esperaba que %s fuese " \
                       "`true`\n"),                                    \
                   __FILE__, __LINE__, #expression);                   \
            failed_tests++;                                            \
            return;                                                    \
        }                                                              \
    } while (0)

#define ASSERT_FALSE(expression)                                       \
    do {                                                               \
        if (expression != false) {                                     \
            printf(RED("\n  Fallo en %s:%d. Se esperaba que %s fuese " \
                       "`false`\n"),                                   \
                   __FILE__, __LINE__, #expression);                   \
            failed_tests++;                                            \
            return;                                                    \
        }                                                              \
    } while (0)

#define ASSERT_EQ_INT(expected, actual)                                        \
    do {                                                                       \
        int exp = (expected);                                                  \
        int act = (actual);                                                    \
        if (exp != act) {                                                      \
            printf(                                                            \
                RED("\n  Fallo en %s:%d. Se esperaba %d y se obtuvo %d\n"), \
                __FILE__, __LINE__, exp, act);                                 \
            failed_tests++;                                                    \
            return;                                                            \
        }                                                                      \
    } while (0)

#define TEST_REPORT() report(__FILE__)

#endif  // MINITEST_H
