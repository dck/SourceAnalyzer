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

#include "nodelocation.h"

using std::string;

INodeLocation& NodeLocation::operator = ( const INodeLocation& nodeLocation )
{
    this->setFile ( nodeLocation.getFile () );
    this->setLine ( nodeLocation.getLine () );
    this->setPos  ( nodeLocation.getPos  () );
    return *this;
}

bool operator == ( const INodeLocation& nodeLocation1, const INodeLocation& nodeLocation2 )
{
    return ( ( nodeLocation1.getFile () == nodeLocation2.getFile () ) &&
             ( nodeLocation1.getLine () == nodeLocation2.getLine () ) &&
             ( nodeLocation1.getPos  () == nodeLocation2.getPos  () )  );
}

bool operator != ( const INodeLocation& nodeLocation1, const INodeLocation& nodeLocation2 )
{
    return !(nodeLocation1 == nodeLocation2);
}
