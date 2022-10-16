#pragma once
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

// Windows implementation
#if (  ( defined(__WIN32) || defined(_WIN32) || defined(_WIN64) ) )

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace colors
{
    const int white = 7;
    const int green = 2;
    const int red = 4;
    const int yellow = 6;
    const int blue = 1;
}

template <typename T>
void print(const T& msg, int color = colors::white)
{
    HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ::SetConsoleTextAttribute(console, color);
    std::cout << msg;
    ::SetConsoleTextAttribute(console, colors::white);
}

#else 

// unused color codes to avoid compilation issues
namespace colors
{
    const int white = 39;
    const int green = 32;
    const int red = 31;
    const int yellow = 33;
    const int blue = 34;
}

template <typename T>
void print(const T& msg, int)
{
    std::cout << msg;
}

#endif

namespace testing {
    class Test
    {
    public:
        typedef void (*function_type)();

        Test(function_type fn, const std::string& nm, const std::string& desc = "")
            : func(fn)
            , test_name(nm)
            , test_description(desc)
        {}

        void run() const { func(); }
        const std::string& description() const { return test_description; }
        const std::string& name() const { return test_name; }

    private:
        function_type func;
        std::string test_name;
        std::string test_description;

    }; // Test


    struct AssertionFailed
    {
        explicit AssertionFailed(const std::string& msg)
            : reason(msg) {}

        std::string reason;

    }; // AssertionFailed

    class TestFailed
    {
    public:
        TestFailed(const Test& test, const std::string& msg)
            : failed_test(test)
            , message(msg)
        {}

        const std::string& which() const { return failed_test.name(); }
        const std::string& why() const { return message; }
        const std::string& tip() const { return failed_test.description(); }

    private:
        const Test& failed_test;
        std::string message;

    }; // TestFailed

    void run(const Test& test)
    {
        try
        {
            std::cout << "Running test: " << test.name() << "\n";

            test.run();

        }
        catch (const AssertionFailed & af) {
            throw TestFailed(test, af.reason);
        }

        std::ostringstream oss;
        oss << "--> Test completed successfully\n";
        print(oss.str(), colors::green);
    }

    class TestSuite
    {
    public:
        static bool register_test(Test::function_type fn, const char* name, const char* description = "")
        {
            registered_tests.push_back(Test(fn, name, description));
            return true;
        }

        static void run_all()
        {
            std::for_each(registered_tests.begin(), registered_tests.end(), ::testing::run);
        }

        static void run_test(std::size_t index)
        {
            ::testing::run(registered_tests.at(index));
        }

    private:
        static std::vector<Test> registered_tests;

    };

    namespace {

        void assertion_fail(const char* expression, const char*, unsigned int line) {
            std::stringstream ss;
            ss << "The following assertion failed at "
                << "line " << line << ": " << expression;
            throw AssertionFailed(ss.str());
        }

        void fail(const char* function_name, unsigned int line) {
            (void)fail; // suppress the unused function warning
            std::stringstream ss;
            ss << "This point should not be reached: "
                << function_name
                << ", line " << line;
            throw AssertionFailed(ss.str());
        }
    }
} // namespace testing


#define ASSERT_THAT(expression)                                         \
    if( !(expression) )                                                 \
        assertion_fail( #expression, __FUNCTION__, __LINE__ );


#define SUCCEED()                                                       \
    return;

#define FAIL()                                                          \
    std::ostringstream ss;                                              \
    ss << "This point should not be reached: "                          \
       << __FUNCTION__                                           \
       << ", line " << __LINE__;                                        \
    throw AssertionFailed( ss.str() );

#define CONCAT_IMPL(x,y) x ## y
#define CONCAT(x,y) CONCAT_IMPL(x,y)

#define REGISTER_TEST_IMPL( fn_name, var_name, name, description )      \
    static void fn_name();                                              \
    static volatile bool var_name = testing::TestSuite::register_test(fn_name, name, description); \
    void fn_name()

#define REGISTER_TEST( name, description, line )                        \
    REGISTER_TEST_IMPL( CONCAT(test_fn_, line), CONCAT(sink_, line), name, description )

#define TEST( name, description )                                       \
    REGISTER_TEST( name, description, __LINE__ )