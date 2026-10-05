#include "minitest.h"

int ran_tests = 0;
int failed_tests = 0;
int skipped_tests = 0;

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
