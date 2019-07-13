#include "tests.h"
#include <fstream>
#include "../src/hash-octree.h"

#define TESTS_NUM 3

int test_01() {
    testInfoLog(stdout, "Exporter test");
    HashOctree::HashOctree ho;
    ho.addDataPoint(3, 3, 3, 2, 1, 1, (void*)123123);
    std::ofstream p;
    p.open("octree.json");
    p << HashOctree::Exporter::toJson(ho, 4, true);
    p.close();
    return TEST_SUCCESS;
}

int test_02() {
    testInfoLog(stdout, "Importer test")
    return TEST_SUCCESS;
}

int test_03() {
    testInfoLog(stdout, "HashOctree::addDataPoint test");
    HashOctree::HashOctree ho;
    assertNoException(ho.addDataPoint(10, 10, 10, 1, 1, 1, (void*)5));
    return TEST_SUCCESS;
}

int (*tests[TESTS_NUM])(void) = {
        test_01,
        test_02,
        test_03,
};

int main() {
    for (auto test_func : tests)
        testStatusLog(stdout, test_func);
    return 0;
}