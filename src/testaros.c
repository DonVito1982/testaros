#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "testaros.h"
#include "testaros_portability.h"

#define YELLOW(text) "\033[1;33m" text "\033[0m"
#define GREEN(text) "\033[1;32m" text "\033[0m"
#define RED(text) "\033[1;31m" text "\033[0m"

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

void tr_report_print(const TrReport* report) {
    printf("Total: %u, ", report->total);
    printf(GREEN("Pasadas") ": %u", report->passed);
    if (report->failed != 0) {
        printf(", " RED("Falladas") ": %u", report->failed);
    }
    if (report->skipped != 0) {
        printf(", " YELLOW("Saltadas") ": %u.", report->skipped);
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

typedef struct {
    char expression[48];
    bool active;
} TrFilter;

static void tr_run_test(TrRunner* runner, const TrTest* test, char* prefix,
                 const TrFilter* filter) {
    char compound_prefix[200];
    strcpy(compound_prefix, prefix);
    strcat(compound_prefix, test->name);
    if (!filter->active || strstr(compound_prefix, filter->expression) != NULL) {
        TrResult result;

        tr_current = runner;
        runner->current_test = test->name;

        if (test->setup) test->setup();

        printf("Ejecutando... %s%s", prefix, test->name);
        result = test->test(); /* en Fase 3: if (setjmp(...) == 0) result =
                                  test(); */
        if (result == TR_OK) printf(GREEN("   OK\n"));

        tr_count_result(&runner->report, result);

        if (test->teardown) test->teardown(); /* debe ejecutarse pase o falle */

        tr_current = NULL;
    }
}

static void display_test(const TrTest* test, char* prefix, const TrFilter* filter) {
    char compound_prefix[200];
    strcpy(compound_prefix, prefix);
    strcat(compound_prefix, test->name);
    if (!filter->active || strstr(compound_prefix, filter->expression) != NULL) {
        printf("%s\n", compound_prefix);
    }
}

static void run_suite(TrRunner* runner, const TrSuite* suite, char* prefix,
                      const TrFilter* filter) {
    char compound_prefix[200];
    strcpy(compound_prefix, prefix);
    strcat(compound_prefix, suite->prefix);
    strcat(compound_prefix, "/");
    unsigned iterations = suite->iterations == 0 ? 1 : suite->iterations;
    for (unsigned i = 0; i < iterations; ++i) {
        for (const TrTest* some_test = suite->tests;
             some_test != NULL && some_test->test != NULL; ++some_test) {
            tr_run_test(runner, some_test, compound_prefix, filter);
        }
    }
    for (const TrSuite* const* child = suite->suites;
         child != NULL && *child != NULL; ++child) {
        run_suite(runner, *child, compound_prefix, filter);
    }
    return;
}

static void list_names(const TrSuite* suite, const char* prefix,
                const TrFilter* filter) {
    char compound_prefix[200];
    strcpy(compound_prefix, prefix);
    strcat(compound_prefix, suite->prefix);
    strcat(compound_prefix, "/");
    if (!filter->active || strstr(compound_prefix, filter->expression) != NULL) {
        printf("%s\n", compound_prefix);
    }
    for (const TrTest* some_test = suite->tests;
         some_test != NULL && some_test->test != NULL; ++some_test) {
        display_test(some_test, compound_prefix, filter);
    }
    for (const TrSuite* const* child = suite->suites;
         child != NULL && *child != NULL; ++child) {
        list_names(*child, compound_prefix, filter);
    }
}

static void print_help(void) {
    printf("Ayuda de testaros:\n\n");
    printf("--help            Muestra este mensaje\n");
    printf("--list            Muestra las pruebas disponibles\n");
    printf("--filter <filter> Ejecuta las pruebas cuyo mensaje contenga a <filter>\n");
}

int tr_run_suites(const TrSuite* suite, int argc, char** argv) {
    TrFilter status;
    status.active = false;
    bool name_listing = false;
    int arg_index = 1;
    if (argc == 2 &&
        (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_help();
        return 0;
    }
    while (arg_index < argc) {
        if (strcmp(argv[arg_index], "--list") == 0) {
            name_listing = true;
            arg_index++;
        } else if (strcmp(argv[arg_index], "--filter") == 0) {
            status.active = true;
            arg_index++;
            if (arg_index == argc) {
                printf("Falta argumento de filtro\n");
                return 1;
            }
            strcpy(status.expression, argv[arg_index]);
            arg_index++;
        } else {
            printf("Argumento no reconocido\n");
            return 1;
        }
    }
    int result = 0;
    if (name_listing) {
        list_names(suite, "", &status);
    } else {
        TrRunner runner;
        tr_runner_init(&runner);
        run_suite(&runner, suite, "", &status);
        tr_report_print(&(runner.report));

        result = tr_report_status(&(runner.report));
    }
    return result;
}
