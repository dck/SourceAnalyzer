#include <iostream>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/ui/text/TestRunner.h>

#include "testnode.h"

int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestResult result;

    runner.addTest( TestNode::suite() );
    runner.run();

    return 0;
}
