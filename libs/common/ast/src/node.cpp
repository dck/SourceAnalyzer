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
    setNodeLocation ( NULL );
    setRightNeighbor( NULL );
    setLeftNeighbor ( NULL );
    setInstrType    ( Node::unknown );
    getValue().clear();
}

Node::~Node()
{
    delete this->_nodeLocation;
    for( INodeIterator it = getChildren().begin(); it!=getChildren().end(); it++ )
        delete *it;
}

void Node::setParent ( INode* parent )
{
    this->_parent = parent;
}

void Node::setInstrType ( const InstrType instrType )
{
    this->_instrType = instrType;
}

void Node::setNodeLocation ( INodeLocation* nodeLocation )
{
    this->_nodeLocation = nodeLocation;
}

void Node::setValue ( const string& value )
{
    this->_value = value;
}

void Node::setLeftNeighbor(INode *lneighbor)
{
    this->_lneighbor = lneighbor;
}

void Node::setRightNeighbor(INode *rneighbor)
{
    this->_rneighbor;
}

void Node::addChild ( INode* child )
{
    this->getChildren().push_back(child);
}

INodeList Node::getChildren() const
{
    return this->_children;
}

INode* Node::getLeftChild() const
{
    return *(getChildren().begin());
}

INode* Node::getRightChild() const
{
    return *(getChildren().end());
}

INode* Node::getParent () const
{
    return this->_parent;
}

INode::InstrType Node::getInstrType () const
{
    return this->_instrType;
}

string Node::getValue () const
{
    return this->_value;
}

INodeLocation* Node::getNodeLocation () const
{
    return this->_nodeLocation;
}

INode* Node::getLeftNeighbor() const
{
    return this->_lneighbor;
}

INode* Node::getRightNeighbor() const
{
    return this->_rneighbor;
}

bool Node::isNull()
{
    if( this == NULL )
        return true;
    return false;
}

bool Node::isParentNull()
{
    if( getParent() == NULL )
        return true;
    return false;
}

bool Node::isChildrenEmpty()
{
    if( getChildren().size() == 0 )
        return true;
    return false;
}

bool Node::operator ==( const INode& node ) const
{
    if ( this->getValue         () == node.getValue         () &&
         this->getInstrType     () == node.getInstrType     () &&
         this->getNodeLocation  () == node.getNodeLocation  ()  );
}

bool Node::operator !=( const INode& node ) const
{
    return !(*this == node);
}
