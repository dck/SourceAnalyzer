#ifndef TESTNODE_H
#define TESTNODE_H

    #include <cppunit/Test.h>
    #include <cppunit/TestCase.h>
    #include <cppunit/TestFixture.h>
    #include <cppunit/extensions/HelperMacros.h>

    #include "node.h"
    #include "nodelocation.h"

    class TestNode : public CppUnit::TestFixture
    {

        CPPUNIT_TEST_SUITE( TestNode );
            CPPUNIT_TEST(testEquality);
            CPPUNIT_TEST(testNotEquality);
        CPPUNIT_TEST_SUITE_END();

        public:
            void setUp();
            void tearDown();
            void testEquality();
            void testNotEquality();

        private:
            INode         *node1, *node2, *node3, *node4;
            INodeLocation *nloc1, *nloc2;
    };

#endif // TESTNODE_H
