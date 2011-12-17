#ifndef IPATH_H
#define IPATH_H

    #include "common.h"
    #include "inode.h"

    class IPath {
        public:
            virtual void setRoot  ( const INode* node ) = 0;
            virtual void addSheet ( const INode* node ) = 0;
            
            virtual INode*   getRoot   () const = 0;
            virtual NodeList getSheets () const = 0;
    };

#endif // IPATH_H
