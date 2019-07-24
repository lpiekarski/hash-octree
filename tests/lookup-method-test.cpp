#include <type_traits>
#include <vector>

#include "test.h"
#include "../include/lookup-methods/lookup-methods.h"
#include "../include/lookup-methods/unordered-map-lookup-method.h"
#include "../include/lookup-methods/map-lookup-method.h"

#define TESTS_NUM 4

std::vector<HashOctree::LookupMethod *> lms;
std::vector<std::string> labels;

void clear_lms() {
    for (const auto &lm : lms)
        delete lm;
    lms.clear();
    labels.clear();
}

void setup_lms() {
    if (!lms.empty())
        clear_lms();
    lms.emplace_back(new HashOctree::UnorderedMapLookupMethod());
    labels.emplace_back("UnorderedMapLookupMethod");

    lms.emplace_back(new HashOctree::MapLookupMethod());
    labels.emplace_back("MapLookupMethod");
}

int test_01() {
    setup_lms();
    testInfoLog(stdout, "Inheritance test");

    clear_lms();
    return TEST_SUCCESS;
}

int test_02() {
    setup_lms();
    testInfoLog(stdout, "Empty map lookup test");

    const HashOctree::key_t testKeys[] {
            (uint64_t)0,
            (uint64_t)UINT64_MAX,
            (uint64_t)1,
            (uint64_t)-1,
            (uint64_t)123123123123123,
            (uint64_t)1111111111111111
    };

    HashOctree::NodeControlBlock *ncb;

    for (const auto &key : testKeys) {
        for (int i = 0; i < lms.size(); i++) {
            testInfoLog(stdout, (labels[i] + ".contains() test").c_str());
            assert(!lms[i]->contains(key));
        }
    }

    for (const auto &key : testKeys) {
        for (int i = 0; i < lms.size(); i++) {
            testInfoLog(stdout, (labels[i] + ".erase() test").c_str());
            assertEqual(lms[i]->erase(key), HashOctree::OK);
        }
    }

    for (const auto &key : testKeys) {
        for (int i = 0; i < lms.size(); i++) {
            testInfoLog(stdout, (labels[i] + ".lookup() test").c_str());
            assertEqual(lms[i]->lookup(key, &ncb), HashOctree::NODE_DOESNT_EXIST);
        }
    }

    for (auto &lm : lms)
        assertEqual(lm->list().size(), 0);

    clear_lms();
    return TEST_SUCCESS;
}

int test_03() {
    testInfoLog(stdout, "Insert test");
    return TEST_SUCCESS;
}

int test_04() {
    testInfoLog(stdout, "Lookup test");
    return TEST_SUCCESS;
}

int (*tests[TESTS_NUM])(void) = {
        test_01,
        test_02,
        test_03,
        test_04,
};

TEST_MAIN