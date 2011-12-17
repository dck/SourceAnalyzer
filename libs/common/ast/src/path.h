#ifndef PATH_H
#define PATH_H

    #include "ipath.h"

    class Path : public IPath {
        public:
            virtual void setRoot  ( const INode* node );
            virtual void addSheet ( const INode* node );
            
            virtual INode*   getRoot   () const;
            virtual NodeList getSheets () const;

        private:
            INode*   _root;
            NodeList _sheets;
    };

#endif // PATH_H
