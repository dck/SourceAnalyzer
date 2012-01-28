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

CPPUNIT_TEST_SUITE_REGISTRATION( TestNode );

void TestNode::setUp()
{
    this->_node1 = new Node();
    this->_node2 = new Node();
    this->_node3 = new Node();
    this->_node4 = new Node();

    this->_nloc1 = new NodeLocation( 4, 12, "size.h");
    this->_nloc2 = new NodeLocation( 3, 2,  "size.h");

    _node1->setInstrType(Node::realInstr);
    _node1->setValue("int");
    _node1->setNodeLocation(*_nloc1);

    _node2->setInstrType(Node::realInstr);
    _node2->setValue("int");
    _node2->setNodeLocation(*_nloc1);

    _node3->setInstrType(Node::virtInstr);
    _node3->setValue("int");
    _node3->setNodeLocation(*_nloc1);

    _node4->setInstrType(Node::realInstr);
    _node4->setValue("int");
    _node4->setNodeLocation(*_nloc2);

}

void TestNode::tearDown()
{
    delete _node1;
    delete _node2;
    delete _node3;
    delete _node4;

    delete _nloc1;
    delete _nloc2;
}

void TestNode::testEquality()
{
    CPPUNIT_ASSERT(   *_node1 == *_node2  );
    CPPUNIT_ASSERT( !(*_node1 == *_node3) );
    CPPUNIT_ASSERT( !(*_node1 == *_node4) );
}

void TestNode::testNotEquality()
{
    CPPUNIT_ASSERT( !(*_node1 != *_node2) );
    CPPUNIT_ASSERT(   *_node1 != *_node3  );
    CPPUNIT_ASSERT(   *_node1 != *_node4  );
}
