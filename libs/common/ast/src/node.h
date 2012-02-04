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

#include <iostream>

#include "inode.h"
#include "nodelocation.h"

class Node : public INode
{
    public:
        Node ();
        virtual ~Node();

        inline void setParent        ( INode* parent )               { _parent = parent;              }
        inline void setInstrType     ( const InstrType instrType )   { _instrType = instrType;        }
        inline void setValue         ( const std::string& value )    { _value = value;                }
        inline void setNodeLocation  ( INodeLocation& nodeLocation ) { *_nodeLocation = nodeLocation; }
        inline void setLeftNeighbor  ( INode* lneighbor )            { _lneighbor = lneighbor;        }
        inline void setRightNeighbor ( INode* rneighbor )            { _rneighbor = rneighbor;        }
        inline void addChildFront    ( INode* child )                { _children.push_front(child);   }
        inline void addChildBack     ( INode* child )                { _children.push_back(child);    }

        inline INodeList        getChildren()      const { return _children;      }
        inline INode*           getParent()        const { return _parent;        }
        inline INode*           getLeftNeighbor()  const { return _lneighbor;     }
        inline INode*           getRightNeighbor() const { return _rneighbor;     }
        inline InstrType        getInstrType()     const { return _instrType;     }
        inline std::string      getValue()         const { return _value;         }
        inline INodeLocation*   getNodeLocation()  const { return _nodeLocation; }

        virtual INode*          getLeftChild()     const;
        virtual INode*          getRightChild()    const;

        inline bool isNull          () const { return (this == NULL);    }
        inline bool isParentNull    () const { return (_parent == NULL); }
        inline bool isChildrenEmpty () const { return _children.empty(); }

        // friend bool operator == ( const INode& node1, const INode& node2 );
        // friend bool operator != ( const INode& node1, const INode& node2 );

    protected:
        virtual void setNodeLocation  ( INodeLocation* nodeLocation ) { _nodeLocation = nodeLocation; }

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
