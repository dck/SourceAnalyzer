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

#include "testdepthwalker.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestDepthWalker );

void TestDepthWalker::setUp()
{
    _builder     = new ASTR2LBuilder();
    build(_builder);
    _ast = _builder->getAST();
    initResSymbols();
}

void TestDepthWalker::tearDown()
{
    if( _builder!= NULL )
        delete _builder;
    _builder = NULL;
    if( _ast!= NULL )
        delete _ast;
    _ast = NULL;
}

void TestDepthWalker::testDepthWalker()
{
    IAST::iterator it = _ast->getRoot();
    IASTWalker* astwalker = new ASTDepthWalker(it);
    int i = 0;
    while( astwalker->isNext() == true )
    {
        CPPUNIT_ASSERT( (_res[i++] == (astwalker->element()->getValue())) );
        astwalker->next();
    }
    delete astwalker;
}

void TestDepthWalker::initResSymbols()
{
    _res.push_back("=");
    _res.push_back("c");
    _res.push_back("+");
    _res.push_back("*");
    _res.push_back("8");
    _res.push_back("5");
    _res.push_back("1");
}
