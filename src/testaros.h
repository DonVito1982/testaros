#ifndef MINITEST_H
#define MINITEST_H

#include <stdbool.h>
#include <stdio.h>

// todo archivo que incluya testaros.h debe definir setUp y tearDown (pueden ser
// vacías)

typedef enum { TR_OK = 0, TR_FAIL, TR_SKIP, TR_ERROR } TrResult;

typedef struct {
    unsigned int total;
    unsigned int passed;
    unsigned int failed;
    unsigned int skipped;
} TrReport;

typedef struct {
    TrReport report;
    TrResult pending_result;
    const char* current_test;
} TrRunner;

void tr_runner_init(TrRunner* runner);

TrResult tr_run_test(TrRunner* runner, TrResult (*test)(void),
                     void (*setup)(void), void (*teardown)(void),
                     const char* name);

void tr_count_result(TrReport* report, TrResult result);
void tr_report_print(const TrReport* report, const char* label);
int tr_report_status(const TrReport* report); /* 0 = ok, 1 = hubo fallos */
// extern int ran_tests;
// extern int failed_tests;
// extern int skipped_tests;

void report(const char* filename);
// void run_single_test(void (*test)(void), void (*setUp)(void),
//                      void (*tearDown)(void), const char* funcname);

#define YELLOW(text) "\033[1;33m" text "\033[0m"
#define GREEN(text) "\033[1;32m" text "\033[0m"
#define RED(text) "\033[1;31m" text "\033[0m"

// #define RUN_TEST(test) run_single_test(test, setUp, tearDown, #test)

void tr_fail_at(const char* file, int line, const char* fmt, ...);
void tr_skip_at(const char* file, int line, const char* reason);

#define SKIP_TEST()                                       \
    do {                                                  \
        tr_skip_at(__FILE__, __LINE__, "prueba saltada"); \
        return TR_SKIP;                                   \
    } while (0)

#define ASSERT_TRUE(expression)                                             \
    do {                                                                    \
        if (expression != true) {                                           \
            tr_fail_at(__FILE__, __LINE__, "se esperaba que %s fuese true", \
                       #expression);                                        \
            return TR_FAIL;                                                 \
        }                                                                   \
    } while (0)

#define ASSERT_FALSE(expression)                                             \
    do {                                                                     \
        if (expression != false) {                                           \
            tr_fail_at(__FILE__, __LINE__, "se esperaba que %s fuese false", \
                       #expression);                                         \
            return TR_FAIL;                                                          \
        }                                                                    \
    } while (0)

#define ASSERT_EQ_INT(expected, actual)                                       \
    do {                                                                      \
        int exp = (expected);                                                 \
        int act = (actual);                                                   \
        if (exp != act) {                                                     \
            tr_fail_at(__FILE__, __LINE__, "Se esperaba %d y se obtuvo %d\n", \
                       exp, act);                                             \
            return TR_FAIL;                                                   \
        }                                                                     \
    } while (0)

// #define TEST_REPORT() report(__FILE__)

#define TR_RUN_TEST(runner, test) \
    tr_run_test((runner), (test), setUp, tearDown, #test)
#define TR_TEST_REPORT(runner) tr_report_print(&(runner)->report, __FILE__)

#endif  // MINITEST_H
