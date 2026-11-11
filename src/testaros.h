#ifndef TESTAROS_H
#define TESTAROS_H

#include <stdbool.h>

typedef enum { TR_OK = 0, TR_FAIL, TR_SKIP, TR_ERROR } TrResult;

typedef TrResult (*TrTestFunc)(void);
typedef void (*TrFixtureFunc)(void);

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

typedef struct {
    const char* name;
    TrTestFunc test;
    TrFixtureFunc setup;
    TrFixtureFunc teardown;
} TrTest;

typedef struct TrSuite {
    const char* prefix;
    const TrTest* tests;                 /* termina con test == NULL */
    const struct TrSuite* const* suites; /* termina con puntero NULL */
    unsigned int iterations;
} TrSuite;

int tr_run_suites(const TrSuite* suite, int argc, char** argv);

void tr_runner_init(TrRunner* runner);

void tr_count_result(TrReport* report, TrResult result);
void tr_report_print(const TrReport* report);
int tr_report_status(const TrReport* report); /* 0 = ok, 1 = hubo fallos */

void tr_fail_at(const char* file, int line, const char* fmt, ...);
void tr_skip_at(const char* file, int line, const char* reason);

#define SKIP_TEST()                                       \
    do {                                                  \
        tr_skip_at(__FILE__, __LINE__, "prueba saltada"); \
        return TR_SKIP;                                   \
    } while (0)

#define ASSERT_TRUE(expression)                                             \
    do {                                                                    \
        if (!(expression)) {                                                \
            tr_fail_at(__FILE__, __LINE__, "se esperaba que %s fuese true", \
                       #expression);                                        \
            return TR_FAIL;                                                 \
        }                                                                   \
    } while (0)

#define ASSERT_FALSE(expression)                                             \
    do {                                                                     \
        if (expression) {                                                    \
            tr_fail_at(__FILE__, __LINE__, "se esperaba que %s fuese false", \
                       #expression);                                         \
            return TR_FAIL;                                                  \
        }                                                                    \
    } while (0)

#define ASSERT_EQ_INT(tr_expected, tr_actual)                                 \
    do {                                                                      \
        int exp = (tr_expected);                                              \
        int act = (tr_actual);                                                \
        if (exp != act) {                                                     \
            tr_fail_at(__FILE__, __LINE__, "Se esperaba %d y se obtuvo %d", \
                       exp, act);                                             \
            return TR_FAIL;                                                   \
        }                                                                     \
    } while (0)

#endif  // TESTAROS_H
