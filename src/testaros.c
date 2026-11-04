#include <stdarg.h>
#include <stdio.h>

#include "testaros.h"
#include "testaros_portability.h"

int ran_tests = 0;
int failed_tests = 0;
int skipped_tests = 0;

static TR_THREAD_LOCAL TrRunner* tr_current;
static TrReport tr_report = {0, 0, 0, 0};

void tr_runner_init(TrRunner* runner) {
    tr_current = runner;
    tr_current->report = tr_report;
    tr_current->current_test = NULL;
}

void tr_skip_at(const char* file, int line, const char* reason) {
    printf(YELLOW("\n  %s. %s:%d\n"), reason, file, line);
}

void tr_fail_at(const char* file, int line, const char* fmt, ...) {
    printf(RED("\n  Fallo en %s:%d. "), file, line);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}

int tr_report_status(const TrReport* report) { return (report->failed > 0); }

void tr_report_print(const TrReport* report, const char* label) {
    printf("Resultados de %s -  ", label);
    printf("Total: %d, ", report->total);
    printf(GREEN("Pasadas") ": %d", report->passed);
    if (report->failed != 0) {
        printf(", " RED("Falladas") ": %d", report->failed);
    }
    if (report->skipped != 0) {
        printf(", " YELLOW("Saltadas") ": %d.", report->skipped);
    }
    printf("\n\n");
}

void report(const char* filename) {
    printf("Resultados de %s -  ", filename);
    printf("Total: %d, ", ran_tests);
    printf(GREEN("Pasadas") ": %d", ran_tests - failed_tests - skipped_tests);
    if (failed_tests != 0) {
        printf(", " RED("Falladas") ": %d", failed_tests);
    }
    if (skipped_tests != 0) {
        printf(", " YELLOW("Saltadas") ": %d.", skipped_tests);
    }
    printf("\n\n");
}

void tr_count_result(TrReport* report, TrResult result) {
    report->total++;
    switch (result) {
        case TR_OK:
            report->passed++;
            break;
        case TR_SKIP:
            report->skipped++;
            break;
        case TR_FAIL:
            report->failed++;
            break;
        case TR_ERROR:
            report->failed++;
            break; /* provisional */
        default:
            report->failed++;
            break; /* defensivo */
    }
}

TrResult tr_run_test(TrRunner* runner, TrResult (*test)(void),
                     void (*setup)(void), void (*teardown)(void),
                     const char* name) {
    TrResult result;

    tr_current = runner;
    runner->current_test = name;

    if (setup) setup();

    printf("Ejecutando... %s", name);
    result = test(); /* en Fase 3: if (setjmp(...) == 0) result = test(); */
    if (result == TR_OK) printf(GREEN("   OK\n"));

    tr_count_result(&runner->report, result);

    if (teardown) teardown(); /* debe ejecutarse pase o falle */

    tr_current = NULL;
    return result;
}

void run_single_test(void (*test)(void), void (*setUp)(void),
                     void (*tearDown)(void), const char* funcname) {
    setUp();
    printf("Ejecutando %s...", funcname);
    int failed_before = failed_tests;
    int skipped_before = skipped_tests;
    test();
    if (failed_before == failed_tests && skipped_before == skipped_tests) {
        printf(GREEN("  Pasa\n"));
    }
    ran_tests++;
    tearDown();
}
