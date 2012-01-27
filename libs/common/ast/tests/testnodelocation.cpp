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

#include "testnodelocation.h"

void TestNodeLocation::setUp()
{
    this->_nloc1 = new NodeLocation( 1, 2, "hello" );
    this->_nloc2 = new NodeLocation( 2, 2, "hello" );
    this->_nloc3 = new NodeLocation( 1, 2, "foo" );
}

void TestNodeLocation::tearDown()
{
    delete this->_nloc1;
    delete this->_nloc2;
    delete this->_nloc3;
}

void TestNodeLocation::testEquality()
{
    CPPUNIT_ASSERT(   *_nloc1 == *_nloc1  );
    CPPUNIT_ASSERT( !(*_nloc1 == *_nloc2) );
    CPPUNIT_ASSERT( !(*_nloc1 == *_nloc3) );
}

void TestNodeLocation::testNotEquality()
{
    CPPUNIT_ASSERT( !( *_nloc1 != *_nloc1 ) );
    CPPUNIT_ASSERT(    *_nloc1 != *_nloc2   );
    CPPUNIT_ASSERT(    *_nloc1 != *_nloc3   );
}
