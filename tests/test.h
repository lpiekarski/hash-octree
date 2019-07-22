#if !defined(__TEST_H__)
#define __TEST_H__

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
#define TEST_NOEXCEPTION (4)

#define TIMEOUT_SECONDS (20)

std::string testContext;

const char *test_result[] = {
        "assertion failed",
        "success",
        "exception",
        "timeout",
        "no exception"
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

#define assertEqual(expr, val)\
    if (expr != val) {\
        fprintf(stderr,\
                "%s:%d equal assertion failed",\
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

#define assertException(prog, Exception)\
    try {\
        prog;\
        fprintf(stderr, "%s:%d except assertion failed\n", __func__, __LINE__);\
        return TEST_NOEXCEPTION;\
    } catch (const Exception &e) {\
    } catch (...) {\
        fprintf(stderr, "%s:%d except assertion failed\n", __func__, __LINE__);\
        return TEST_NOEXCEPTION;\
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

#define testStatusLog(stream, func) \
        auto __future = std::async(std::launch::async, (func));\
        size_t __result;\
        if (__future.wait_for(std::chrono::seconds(TIMEOUT_SECONDS)) != std::future_status::ready)\
            __result = TEST_TIMEOUT;\
        else\
            __result = __future.get();\
        fprintf((stream), "\t%s test result: %s\n\n",  testContext.c_str(), test_result[__result]);\

#define TEST_MAIN \
int main() {\
    int ret = 0;\
\
    for (auto test_func : tests) {\
        testStatusLog(stdout, test_func);\
        if (__result != TEST_SUCCESS)\
            ret = 1;\
    }\
\
    return ret;\
}\

#endif // __TEST_H__