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

#include "node.h"

using std::string;

///////////// public /////////////

Node::Node()
{
    setParent       ( NULL );
    setNodeLocation ( new NodeLocation() );
    setRightNeighbor( NULL );
    setLeftNeighbor ( NULL );
    setInstrType    ( Node::unknown );
}

Node::~Node()
{
    if( NULL != _nodeLocation ) {
        delete _nodeLocation;
        _nodeLocation = NULL;
    }
    for( INodeIterator it = _children.begin(); it!=_children.end(); ++it )
    {
        INode* node = *it;
        delete node;
        node = NULL;
    }
}

INode* Node::getLeftChild() const
{
    if( !getChildren().empty() )
        return getChildren().front();
    // have to throw exception
    return NULL;
}

INode* Node::getRightChild() const
{
    if( !getChildren().empty() )
        return getChildren().back();
    // have to throw exception
    return NULL;
}

bool operator == ( const INode& node1, const INode& node2 )
{
    return ( (  node1.getValue()        == node2.getValue()          ) &&
             (  node1.getInstrType()    == node2.getInstrType()      ) &&
             ( *node1.getNodeLocation() == *node2.getNodeLocation()  ) );
}

bool operator != ( const INode& node1, const INode& node2 )
{
    return !(node1 == node2);
}

INode& Node::operator =  ( const INode& node )
{
    this->setInstrType     ( node.getInstrType () );

    this->setLeftNeighbor  ( node.getLeftNeighbor  () );
    this->setRightNeighbor ( node.getRightNeighbor () );

    this->setParent        ( node.getParent () );
    this->setValue         ( node.getValue  () );

    *(this->getNodeLocation()) = *(node.getNodeLocation());

    for( INodeIterator it = node.getChildren().begin(); it!=node.getChildren().end(); ++it )
    {
        _children.push_back(*it);
    }
}
