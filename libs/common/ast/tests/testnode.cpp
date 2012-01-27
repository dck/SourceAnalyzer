#include "testnode.h"

void TestNode::setUp()
{
    this->node1 = new Node();
    this->node2 = new Node();
    this->node3 = new Node();
    this->node4 = new Node();

    this->nloc1 = new NodeLocation( 4, 12, "size.h");
    this->nloc2 = new NodeLocation( 3, 2,  "size.h");

    node1->setInstrType(Node::realInstr);
    node1->setValue("int");
    node1->setNodeLocation(nloc1);

    node2->setInstrType(Node::realInstr);
    node2->setValue("int");
    node2->setNodeLocation(nloc1);

    node3->setInstrType(Node::virtInstr);
    node3->setValue("int");
    node3->setNodeLocation(nloc1);

    node4->setInstrType(Node::realInstr);
    node4->setValue("int");
    node4->setNodeLocation(nloc2);

}

void TestNode::tearDown()
{
    delete node1;
    delete node2;
    delete node3;
    delete node4;

    delete nloc1;
    delete nloc2;
}

void TestNode::testEquality()
{
    CPPUNIT_ASSERT(   *node1 == *node2  );
    CPPUNIT_ASSERT( !(*node1 == *node3) );
}

void TestNode::testNotEquality()
{
    CPPUNIT_ASSERT(   *node1 != *node3  );
    CPPUNIT_ASSERT( !(*node1 != *node4) );
}
