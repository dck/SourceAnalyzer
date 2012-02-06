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

#ifndef TESTASTBUILDER_H
#define TESTASTBUILDER_H

#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "astbuilder.h"
#include "iexception.h"

class TreeBuilderMethod
{
    public:
        TreeBuilderMethod() { initSymbols(); }
        //         c = 8 * 5 + 1;
        //             |=|
        //         |c|     |+|
        //             |*|     |1|
        //          |8|   |5|
        virtual void build(  IASTBuilder* astBuilder );

    protected:
        virtual void initSymbols ();

    private:
        std::vector<std::string> symbols;
};

class TestASTBuilder : public CppUnit::TestFixture
        , protected TreeBuilderMethod
{
    CPPUNIT_TEST_SUITE( TestASTBuilder );
        CPPUNIT_TEST(testASTBuilder);
    CPPUNIT_TEST_SUITE_END();

    public:
        virtual void setUp      ();
        virtual void tearDown   ();
        void testASTBuilder     ();

    protected:
        virtual void initResSymbols ();

    private:
        int  testR2LBuilder ();
        int  testL2RBuilder ();

        std::vector<std::string> _sr2lres;
        std::vector<std::string> _sl2rres;
};

#endif // TESTASTBUILDER_H
