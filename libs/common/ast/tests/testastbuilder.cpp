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

using std::vector;

CPPUNIT_TEST_SUITE_REGISTRATION( TestASTBuilder );

void TreeBuilderMethod::build(IASTBuilder *astBuilder)
{
    try
    {
        size_t i = 0;
        INode* _node = NULL;
        INodeLocation* _nloc = new NodeLocation();

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(1); _nloc->setLine(1); _nloc->setFile("test.cpp");
        _node->setNodeLocation( *_nloc );
        astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(5);
        _node->setNodeLocation( *_nloc );
        astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(9);
        _node->setNodeLocation( *_nloc );
        astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(7);
        _node->setNodeLocation( *_nloc );
        astBuilder->buildNode(_node, 2);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(13);
        _node->setNodeLocation( *_nloc );
        astBuilder->pushNode(_node);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(11);
        _node->setNodeLocation( *_nloc );
        astBuilder->buildNode(_node, 2);

        _node = new Node();
        _node->setInstrType(INode::realInstr);
        _node->setValue(symbols[i++]);
        _nloc->setPos(3);
        _node->setNodeLocation( *_nloc );
        astBuilder->buildNode(_node, 2);

        delete _nloc;
        _nloc = NULL;
    }
    catch ( IException& exception )
    {
        std::cout << exception.getMsg() << std::endl;
    }
}

void TreeBuilderMethod::initSymbols()
{
    symbols.push_back("c");
    symbols.push_back("8");
    symbols.push_back("5");
    symbols.push_back("*");
    symbols.push_back("1");
    symbols.push_back("+");
    symbols.push_back("=");
}

void TestASTBuilder::setUp()
{
    initResSymbols();
}

void TestASTBuilder::tearDown()
{
}

void TestASTBuilder::testASTBuilder()
{
    CPPUNIT_ASSERT( 0 == testR2LBuilder() );
    CPPUNIT_ASSERT( 0 == testL2RBuilder() );
}

int TestASTBuilder::testR2LBuilder()
{
    int i = 0;
    int ret_code = 0;
    IASTBuilder* astBuilder = new ASTR2LBuilder();
    build( astBuilder );
    IAST* ast = astBuilder->getAST();

    IAST::iterator it = ast->getRoot();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;
    it.downToL();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;
    it.up().downToR();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;
    it.downToL();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;
    it.downToL();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;
    it.up().downToR();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;
    it.up().up().downToR();
    if( it->getValue().compare(_sr2lres[i++]) != 0 )
        ret_code++;

    delete ast;
    delete astBuilder;
    return ret_code;
}

int TestASTBuilder::testL2RBuilder()
{
    int i = 0;
    int ret_code = 0;

    IASTBuilder* astBuilder = new ASTL2RBuilder();
    build( astBuilder );
    IAST* ast = astBuilder->getAST();

    IAST::iterator it = ast->getRoot();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;
    it.downToL();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;
    it.downToL();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;
    it.up().downToR();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;
    it.downToL();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;
    it.up().downToR();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;
    it.up().up().up().downToR();
    if( it->getValue().compare(_sl2rres[i++]) != 0 )
        ret_code++;

    delete ast;
    delete astBuilder;
    return 0;
}

void TestASTBuilder::initResSymbols()
{
    _sr2lres.push_back("=");
    _sr2lres.push_back("c");
    _sr2lres.push_back("+");
    _sr2lres.push_back("*");
    _sr2lres.push_back("8");
    _sr2lres.push_back("5");
    _sr2lres.push_back("1");

    _sl2rres.push_back("=");
    _sl2rres.push_back("+");
    _sl2rres.push_back("1");
    _sl2rres.push_back("*");
    _sl2rres.push_back("5");
    _sl2rres.push_back("8");
    _sl2rres.push_back("c");
}
