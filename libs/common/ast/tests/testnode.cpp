// Copyright 2008-2011 SourceAnalyzer team. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY SOURCEANALYZER TEAM ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SOURCEANALYZER TEAM OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of SourceAnalyzer team.

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
    CPPUNIT_ASSERT( !(*node1 != *node2) );
    CPPUNIT_ASSERT(   *node1 != *node3  );
    CPPUNIT_ASSERT(   *node1 != *node4  );
}
