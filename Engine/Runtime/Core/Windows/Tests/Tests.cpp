#include <Shared.h>
#include "Tests.h"
#include "Framework.h"
#include "UnitTests.h"

using namespace testing;

std::vector<Test> TestSuite::registered_tests;

void do_tests() {
    try
    {
        TestSuite::run_all();
        exit(0);
    }
    catch (const TestFailed & fail) { // if a running test fails, the program interrupts its execution
                                     // and a detailed message about the failure conditions is printed
        std::ostringstream oss;
        oss << "--> Test failed";
        print(oss.str(), colors::red);

        std::cout << "\n--> Reason: ";
        print(fail.why(), colors::yellow);

        std::cout << "\n--> Check the test description: ";
        print(fail.tip(), colors::yellow);

    }
    catch (const std::exception & ex) {
        std::ostringstream oss;
        oss << "Runtime error exception caught: \n";
        oss << ex.what() << std::endl << std::endl;
        print(oss.str(), colors::red);
    }
    catch (...) {
        print("An unknown exception was thrown. This should never happen\n", colors::red);
    }

    exit(1);
}