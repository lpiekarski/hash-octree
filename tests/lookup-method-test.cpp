#include <type_traits>
#include <vector>

#include "test.h"
#include "../include/lookup-methods/lookup-methods.h"
#include "../include/lookup-methods/unordered-map-lookup-method.h"

#define TESTS_NUM 4

std::vector<HashOctree::LookupMethod *> lms;

void clear_lms() {
    for (const auto &lm : lms)
        delete lm;
    lms.clear();
}

void setup_lms() {
    if (!lms.empty())
        clear_lms();
    lms.emplace_back(new HashOctree::UnorderedMapLookupMethod());
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

    for (const auto &key : testKeys) {
        for (auto &lm : lms) {
            testInfoLog(stdout, (std::string(typeid(lm).name()) + ".contains() test").c_str());
            assert(!lm->contains(key));
        }
    }

    for (const auto &key : testKeys) {
        for (auto &lm : lms) {
            testInfoLog(stdout, (std::string(typeid(lm).name()) + ".erase() test").c_str());
            assertNoException(lm->erase(key));
        }
    }

    for (const auto &key : testKeys) {
        for (const auto &lm : lms) {
            testInfoLog(stdout, (std::string(typeid(lm).name()) + ".lookup() const test").c_str());
            const HashOctree::LookupMethod &lm_const = *lm;
            assertException(lm_const.lookup(key), std::out_of_range);
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