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

#ifndef NODE_H
#define NODE_H

    #include "inode.h"

    class Node : public INode
    {
        public:
            Node ();
            virtual ~Node();

            virtual void setParent        ( INode* parent );
            virtual void setInstrType     ( const InstrType instrType );
            virtual void setNodeLocation  ( INodeLocation* nodeLocation );
            virtual void setValue         ( const std::string& value );
            virtual void setLeftNeighbor  ( INode* lneighbor );
            virtual void setRightNeighbor ( INode* rneighbor );
            virtual void addChild         ( INode* child );

            virtual INodeList        getChildren()      const;
            virtual INode*           getLeftChild()     const;
            virtual INode*           getRightChild()    const;
            virtual INode*           getParent()        const;
            virtual INode*           getLeftNeighbor()  const;
            virtual INode*           getRightNeighbor() const;
            virtual InstrType        getInstrType()     const;
            virtual std::string      getValue()         const;
            virtual INodeLocation*   getNodeLocation()  const;

            virtual bool isNull          ();
            virtual bool isParentNull    ();
            virtual bool isChildrenEmpty ();

            friend bool operator == ( const INode& node1, const INode& node2 );
            friend bool operator != ( const INode& node1, const INode& node2 );

        private:
            INodeList      _children;
            INode*         _parent;
            INode*         _lneighbor;
            INode*         _rneighbor;
            std::string    _value;
            InstrType      _instrType;
            INodeLocation* _nodeLocation;
    };


#endif // NODE_H
