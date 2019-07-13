#if !defined(__TESTS_H__)
#define __TESTS_H__

#include <chrono>
#include <cstdio>
#include <exception>
#include <future>
#include <string>
#include <thread>

#define TEST_ASSERTION_FAILED (0)
#define TEST_SUCCESS (1)
#define TEST_EXCEPTION (2)
#define TEST_TIMEOUT (3)

#define TIMEOUT_SECONDS (20)

std::string testContext;

const char *test_result[] = {
        "assertion failed",
        "success",
        "exception",
        "timeout"
};

#define assert(pred)\
    if (!(pred)) {\
        fprintf(stderr,\
                "%s:%d assertion failed\n",\
                __func__,\
                __LINE__\
                );\
        return TEST_ASSERTION_FAILED;\
    }

#define assertNoException(prog)\
    try {\
        prog;\
    } catch (const std::exception &e) {\
        fprintf(stderr,\
                "%s:%d noexcept assertion failed: %s\n",\
                __func__,\
                __LINE__,\
                e.what()\
                );\
        return TEST_EXCEPTION;\
    }

#define testInfoLog(stream, msg)\
    if (testContext == __func__) {\
        fprintf((stream), "\t%s\n",  (msg));\
        fflush((stream));\
    } else {\
        fprintf((stream), "%s: %s\n",  __func__, (msg));\
        fflush((stream));\
        testContext = __func__;\
    }

#define testStatusLog(stream, func) {\
        auto __future = std::async(std::launch::async, (func));\
        size_t __result;\
        if (__future.wait_for(std::chrono::seconds(TIMEOUT_SECONDS)) != std::future_status::ready)\
            __result = TEST_TIMEOUT;\
        else\
            __result = __future.get();\
        fprintf((stream), "\t%s test result: %s\n\n",  testContext.c_str(), test_result[__result]);\
    }

#endif // __TESTS_H__