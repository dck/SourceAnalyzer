#ifndef INODE_H
#define INODE_H

    #include "inodelocation.h"

    class std::string;    

    class INode {
        public:
            enum InstrType {
                realInstr = 0,
                virtInstr = 1
            };

            virtual INode*         getParent()       const = 0;
            virtual InstrType      getInstrType()    const = 0;
            virtual std::string    getValue()        const = 0;
            virtual INodeLocation* getNodeLocation() const = 0;

            //TODO: implement getChildren function
            
            virtual void setParent       ( const INode* parent ) = 0;
            virtual void setInstrType    ( const InstrType instrType ) = 0;
            virtual void setNodeLocation ( const INodeLocation* nodeLocation ) = 0;
            virtual void setValue        ( const std::string& value ) = 0;
            virtual void addChild        ( const INode* child ) = 0;
            
            //TODO: implement removeChild function
            //TODO: implement operator=
    };
    
#endif //INODE_H
