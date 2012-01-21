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

///////////// public /////////////

IAST::iterator::iterator(IAST *ast)
{
    setAST(ast);
    setCurrentNode(getAST()->getRoot());
}

IAST::iterator::iterator(const IAST::iterator& _iterator)
{
    setCurrentNode  (_iterator.getCurrentNode());
    setAST          (_iterator.getAST());
}

IAST::iterator& IAST::iterator::root()
{
    return *this;
}

IAST::iterator& IAST::iterator::up()
{
    // here must be thrown exception instead of returnign NULL
    if( getCurrentNode()->isParentNull() == true )
        return *this;
    setCurrentNode( getCurrentNode()->getParent() );
    return *this;
}

IAST::iterator& IAST::iterator::downToL()
{
    // here must be thrown exception instead of returnign NULL
    if( getCurrentNode()->isChildrenEmpty() == true )
        return *this;
    setCurrentNode( getCurrentNode()->getLeftChild() );
    return *this;
}

IAST::iterator& IAST::iterator::downToR()
{
    // here must be thrown exception instead of returnign NULL
    if( getCurrentNode()->isChildrenEmpty() == true )
        return *this;
    setCurrentNode( getCurrentNode()->getRightChild() );
    return *this;
}

IAST::iterator& IAST::iterator::left()
{
    INode* leftNeighbor = getCurrentNode()->getLeftNeighbor();
    if( leftNeighbor != NULL )
        setCurrentNode( leftNeighbor );
    return *this;
}

IAST::iterator& IAST::iterator::operator-- ()
{
    return left();
}

IAST::iterator IAST::iterator::operator-- (int)
{
    iterator it_copy(*this);
    left();
    return it_copy;
}

IAST::iterator& IAST::iterator::right()
{
    INode* rightNeighbor = getCurrentNode()->getRightChild();
    if( rightNeighbor != NULL )
        setCurrentNode( rightNeighbor );
    return *this;
}

IAST::iterator& IAST::iterator::operator++ ()
{
    return right();
}

IAST::iterator IAST::iterator::operator++ (int)
{
    iterator it_copy(*this);
    right();
    return it_copy;
}

IAST::iterator& IAST::iterator::operator= ( const iterator& _iterator )
{
    setAST(_iterator.getAST());
    setCurrentNode(_iterator.getCurrentNode());
    return *this;
}

INode* IAST::iterator::operator* () const
{
    return getCurrentNode();
}

INode* IAST::iterator::operator-> () const
{
    return getCurrentNode();
}

///////////// protected /////////////

INode* IAST::iterator::getCurrentNode() const
{
    return this->_currentNode;
}

IAST* IAST::iterator::getAST() const
{
    return this->_ast;
}

void IAST::iterator::setCurrentNode(INode *currentNode)
{
    this->_currentNode = currentNode;
}

void IAST::iterator::setAST(IAST *ast)
{
    this->_ast = ast;
}
