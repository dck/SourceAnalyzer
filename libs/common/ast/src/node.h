#ifndef NODE_H
#define NODE_H

    #include "inode.h"
    #include "common.h"

    class Node : public INode{
        public:
            virtual Node*            getParent()       const;
            virtual InstrType        getInstrType()    const;
            virtual std::string      getValue()        const;
            virtual INodeLocation*   getNodeLocation() const;
            
            virtual void setParent       ( const INode* parent );
            virtual void setInstrType    ( const InstrType instrType );
            virtual void setNodeLocation ( const INodeLocation* nodeLocation );
            virtual void setValue        ( const std::string& value );
            virtual void addChild        ( const INode* child );

        private:
            NodeList       _children;
            INode*         _parent;
            std::string    _value;
            InstrType      _instrType;
            INodeLocation* _nodeLocation;
    };
    

#endif // NODE_H
