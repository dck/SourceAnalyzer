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

#include "testastiterator.h"
#include "iexception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestASTIterator );

void TestASTIterator::setUp()
{
    _builder = new ASTR2LBuilder();
    build(_builder);
    _ast = _builder->getAST();
}

void TestASTIterator::tearDown()
{
    if( _builder!= NULL )
        delete _builder;
    _builder = NULL;
    if( _ast!= NULL )
        delete _ast;
    _ast = NULL;
}

void TestASTIterator::testNullIt()
{
    IAST::iterator it = _ast->getRoot();
    CPPUNIT_ASSERT( ((it.up())      == IAST::null_it) );
    CPPUNIT_ASSERT( ((it.downToL()) != IAST::null_it) );
}

void TestASTIterator::testNullItForException()
{
    IAST::iterator it;
    CPPUNIT_ASSERT_THROW( *it,          BadPointer );
    CPPUNIT_ASSERT_THROW( it.downToL(), BadPointer );
    CPPUNIT_ASSERT_THROW( it.downToR(), BadPointer );
    CPPUNIT_ASSERT_THROW( it.up(),      BadPointer );
    CPPUNIT_ASSERT_THROW( ++it,         BadPointer );
    CPPUNIT_ASSERT_THROW( --it,         BadPointer );
    CPPUNIT_ASSERT_THROW( it++,         BadPointer );
    CPPUNIT_ASSERT_THROW( it--,         BadPointer );
    CPPUNIT_ASSERT_THROW( IAST::null_it.downToL(), BadPointer );
}

void TestASTIterator::testOperatorEqual()
{
    IAST::iterator it1 = _ast->getRoot();
    IAST::iterator it2 = _ast->getRoot();
    it1.downToL();
    CPPUNIT_ASSERT( it1 != it2 );
    it2 = it1;
    CPPUNIT_ASSERT( it1 == it2 );
    it1.up();
    CPPUNIT_ASSERT( it1 != it2 );
}
