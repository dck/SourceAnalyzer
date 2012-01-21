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

#include "astbuilder.h"

///////////// public /////////////

ASTBuilder::ASTBuilder()
{
    setINodeStack( new INodeStack );
}

ASTBuilder::~ASTBuilder()
{
    delete getINodeStack();
}

void ASTBuilder::pushNode( INode* node )
{
    getINodeStack()->push(node);
}

void ASTBuilder::buildNode( INode* node, const size_t childrenNodeNumber )
{
    if( getINodeStack()->size() < childrenNodeNumber )
    {
        // we must to throw exception here
        // can't to build node
    }
    for( size_t i = 0; i<childrenNodeNumber; i++ )
    {
        node->getChildren().push_back(getINodeStack()->top());
        getINodeStack()->pop();
    }
    getINodeStack()->push(node);
}

IAST* ASTBuilder::getAST ()
{
    if( getINodeStack()->size() != 1 )
    {
        // we have to throw exception here
        // can't get ast
    }
    IAST* ast = new AbstractSyntaxTree(getINodeStack()->top());
    getINodeStack()->pop();
    return ast;
}
