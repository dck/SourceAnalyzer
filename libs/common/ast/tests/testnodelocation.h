#ifndef TESTNODELOCATION_H
#define TESTNODELOCATION_H

#include <cppunit/Test.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nodelocation.h"

class TestNodeLocation : public CppUnit::TestCase
{

public:
    TestNodeLocation();
    TestNodeLocation( std::string name ) : CppUnit::TestCase( name ) {}

    void runTest() {
      NodeLocation node1, node2;
      node1.setPos(10);
      node1.setFile("/src/test.cpp");
      node2.setPos(12);

      CPPUNIT_ASSERT( node1.getFile() == "/src/test.cpp" );
      CPPUNIT_ASSERT( node2.getPos() == 12 );

      // Expected fail
      CPPUNIT_ASSERT( node1.getPos() == 15 );
    }

};

#endif // TESTNODELOCATION_H
