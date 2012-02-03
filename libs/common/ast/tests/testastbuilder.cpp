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

#include "testastbuilder.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestASTBuilder );

void TestASTBuilder::setUp()
{
    _ast        = NULL;
    _astBuilder = new ASTR2LBuilder();
    _nloc       = new NodeLocation();
}

void TestASTBuilder::tearDown()
{
    if( _astBuilder != NULL )
        delete _astBuilder;
    _astBuilder = NULL;

    if( _ast != NULL )
        delete _ast;
    _ast = NULL;

    if( _nloc != NULL )
        delete _nloc;
    _nloc = NULL;
}

void TestASTBuilder::test()
{
    buildSimpleTree();
    _ast = _astBuilder->getAST();
}

void TestASTBuilder::buildSimpleTree()
{
    try
    {
        INode* _node = NULL;

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("c");
        _nloc->setPos(1); _nloc->setLine(1); _nloc->setFile("test.cpp");
        _node->setNodeLocation( *_nloc );
        _astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("8");
        _nloc->setPos(5);
        _node->setNodeLocation( *_nloc );
        _astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("5");
        _nloc->setPos(9);
        _node->setNodeLocation( *_nloc );
        _astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("*");
        _nloc->setPos(7);
        _node->setNodeLocation( *_nloc );
        _astBuilder->buildNode(_node, 2);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("1");
        _nloc->setPos(13);
        _node->setNodeLocation( *_nloc );
        _astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("+");
        _nloc->setPos(11);
        _node->setNodeLocation( *_nloc );
        _astBuilder->buildNode(_node, 2);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue("=");
        _nloc->setPos(3);
        _node->setNodeLocation( *_nloc );
        _astBuilder->buildNode(_node, 2);

    }
    catch ( IException& exception )
    {
        std::cout << exception.getMsg() << std::endl;
    }
}
