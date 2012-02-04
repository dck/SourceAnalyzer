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

#ifndef INODE_H
#define INODE_H

#include <list>
#include <stack>

#include "inodelocation.h"

class string;
class INode;

typedef std::list<INode*>   INodeList;
typedef std::stack<INode*>  INodeStack;
typedef INodeList::iterator INodeIterator;

class INode
{
    public:
        enum InstrType
        {
            unknown   = -1,
            realInstr = 0,
            virtInstr = 1
        };

        virtual ~INode(){}
        virtual INode*         getParent()        const = 0;
        virtual InstrType      getInstrType()     const = 0;
        virtual std::string    getValue()         const = 0;
        virtual INodeLocation* getNodeLocation()  const = 0;
        virtual INodeList      getChildren()      const = 0;
        virtual INode*         getLeftChild()     const = 0;
        virtual INode*         getRightChild()    const = 0;
        virtual INode*         getLeftNeighbor()  const = 0;
        virtual INode*         getRightNeighbor() const = 0;

        virtual void setParent        ( INode* parent )                = 0;
        virtual void setInstrType     ( const InstrType instrType )    = 0;
        virtual void setValue         ( const std::string& value )     = 0;
        virtual void setLeftNeighbor  ( INode* lneighbor )             = 0;
        virtual void setRightNeighbor ( INode* rneighbor )             = 0;
        virtual void setNodeLocation  ( INodeLocation& nodeLocation )  = 0;
        virtual void addChildFront    ( INode* child )                 = 0;
        virtual void addChildBack     ( INode* child )                 = 0;

        virtual bool isNull          () const = 0;
        virtual bool isParentNull    () const = 0;
        virtual bool isChildrenEmpty () const = 0;

        friend bool operator == ( const INode& node1, const INode& node2 );
        friend bool operator != ( const INode& node1, const INode& node2 );

    protected:
        virtual void setNodeLocation ( INodeLocation* nodeLocation ) = 0;

        /*
            virtual IASTIterator& hbegin () const = 0;
            virtual IASTIterator& hend   () const = 0;

            virtual IASTIterator& lchild () const = 0;
            virtual IASTIterator& rchild () const = 0;
        */

        //TODO: implement removeChild function
        //TODO: implement operator=
};

#endif //INODE_H
