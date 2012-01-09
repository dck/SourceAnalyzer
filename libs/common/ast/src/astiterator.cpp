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

#include "astiterator.h"

///////////// public /////////////

ASTIterator::ASTIterator(IAST *ast)
{
    setAST(ast);
    setCurrentNode(getAST()->getRoot());
}

ASTIterator::ASTIterator(const ASTIterator& _ASTIterator)
{
    setCurrentNode  (_ASTIterator.getCurrentNode());
    setAST          (_ASTIterator.getAST());
}

IASTIterator& ASTIterator::root()
{
    return *this;
}

IASTIterator& ASTIterator::up()
{
    // here must be thrown exception instead of returnign NULL
    if( getCurrentNode()->isParentNull() == true )
        return *this;
    setCurrentNode( getCurrentNode()->getParent() );
    return *this;
}

IASTIterator& ASTIterator::downToL()
{
    // here must be thrown exception instead of returnign NULL
    if( getCurrentNode()->isChildrenEmpty() == true )
        return *this;
    setCurrentNode( getCurrentNode()->getLeftChild() );
    return *this;
}

IASTIterator& ASTIterator::downToR()
{
    // here must be thrown exception instead of returnign NULL
    if( getCurrentNode()->isChildrenEmpty() == true )
        return *this;
    setCurrentNode( getCurrentNode()->getRightChild() );
    return *this;
}

IASTIterator& ASTIterator::left()
{
    INode* leftNeighbor = getCurrentNode()->getLeftNeighbor();
    if( leftNeighbor != NULL )
        setCurrentNode( leftNeighbor );
    return *this;
}

IASTIterator& ASTIterator::operator-- ()
{
    return left();
}

ASTIterator ASTIterator::operator-- (int)
{
    ASTIterator it_copy(*this);
    left();
    return it_copy;
}

IASTIterator& ASTIterator::right()
{
    INode* rightNeighbor = getCurrentNode()->getRightChild();
    if( rightNeighbor != NULL )
        setCurrentNode( rightNeighbor );
    return *this;
}

IASTIterator& ASTIterator::operator++ ()
{
    return right();
}

ASTIterator ASTIterator::operator++ (int)
{
    ASTIterator it_copy(*this);
    right();
    return it_copy;
}

IASTIterator& ASTIterator::operator= ( const ASTIterator& _ASTIterator )
{
    setAST(_ASTIterator.getAST());
    setCurrentNode(_ASTIterator.getCurrentNode());
    return *this;
}

///////////// protected /////////////

INode* ASTIterator::getCurrentNode() const
{
    return this->_currentNode;
}

IAST* ASTIterator::getAST() const
{
    return this->_ast;
}

void ASTIterator::setCurrentNode(INode *currentNode)
{
    this->_currentNode = currentNode;
}

void ASTIterator::setAST(IAST *ast)
{
    this->_ast = ast;
}
