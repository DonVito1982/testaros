#include "../src/testaros.h"
#include <stddef.h>

static TrResult test_suma_pass(void) {
    int a = 3;
    int b = 4;
    ASSERT_EQ_INT(7, a + b);
    return TR_OK;
}

static TrResult test_suma_fails(void) {
    int a = 3;
    int b = 4;
    ASSERT_EQ_INT(6, a + b);
    return TR_OK;
}

static TrResult test_suma_skip(void) {
    SKIP_TEST();
    return TR_OK;
}

static const TrTest tests[] = {
    {"suma-pass", test_suma_pass, NULL, NULL},
    {"suma-fails", test_suma_fails, NULL, NULL},
    {"suma-skips", test_suma_skip, NULL, NULL},
    {NULL, NULL, NULL, NULL}
};

const TrSuite suma_suite = { "suma", tests, NULL, 1};

int main(int argc, char **argv) {
    return tr_run_suites(&suma_suite, argc, argv);
}
