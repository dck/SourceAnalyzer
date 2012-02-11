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

#include "iast.h"
#include "iexception.h"

IAST::iterator& IAST::null_iterator::up()
{
    throw BadPointer("null_iterator: up");
    return *this;
}

IAST::iterator& IAST::null_iterator::downToL()
{
    throw BadPointer("null_iterator: downToL");
    return *this;
}

IAST::iterator& IAST::null_iterator::downToR()
{
    throw BadPointer("null_iterator: downToR");
    return *this;
}

IAST::iterator& IAST::null_iterator::left()
{
    throw BadPointer("null_iterator: left");
    return *this;
}

IAST::iterator& IAST::null_iterator::right()
{
    throw BadPointer("null_iterator: right");
    return *this;
}

bool operator == ( const IAST::null_iterator& it1, const IAST::iterator& it2 )
{
    return ( it1.isNull() == it2.isNull() );
}

bool operator != ( const IAST::null_iterator& it1, const IAST::iterator& it2 )
{
    return !( it1 == it2 );
}

bool operator == ( const IAST::null_iterator& it1, const IAST::null_iterator& it2 )
{
    return ( it1.isNull() == it2.isNull() );
}

bool operator != ( const IAST::null_iterator& it1, const IAST::null_iterator& it2 )
{
    return !( it1 == it2 );
}

bool operator == ( const IAST::iterator& it1, const IAST::null_iterator& it2 )
{
    return ( it2 == it1 );
}

bool operator != ( const IAST::iterator& it1, const IAST::null_iterator& it2 )
{
    return !( it2 == it1 );
}

INode* IAST::null_iterator::operator* () const
{
    throw BadPointer("null_iterator: operator*");
    return NULL;
}

INode* IAST::null_iterator::operator-> () const
{
    throw BadPointer("null_iterator: operator->");
    return NULL;
}
