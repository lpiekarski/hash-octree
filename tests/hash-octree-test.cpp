#include <fstream>
#include "test.h"
#include "../include/hash-octree.h"
#include "../include/exporter.h"
#include "../include/lookup-methods/lookup-methods.h"

#define TESTS_NUM 3

int test_01() {
    testInfoLog(stdout, "Exporter test");
    HashOctree::HashOctree<> ho;
    ho.addDataPoint(HashOctree::NodeDims(10, 3, 23, 4, 5, 7), (void*)123123);
    ho.addDataPoint(HashOctree::NodeDims(-3, 5, 7, 1, 2, 3), (void*)1233);
    ho.addDataPoint(HashOctree::NodeDims(2, 5, 3, 10, 3, 3), (void*)555123);
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
    HashOctree::HashOctree<> ho;
    assertNoException(ho.addDataPoint(HashOctree::NodeDims(10, 10, 10, 1, 1, 1), (void*)5));
    return TEST_SUCCESS;
}

int (*tests[TESTS_NUM])(void) = {
        test_01,
        test_02,
        test_03,
};

TEST_MAIN