
#ifndef IABSTRACT_SYNTAX_TREE_H
#define IABSTRACT_SYNTAX_TREE_H

    #include "inode.h"

    class IAbstractSyntaxTree {
        public:
            virtual INode* getRoot() const = 0;
    };

    typedef IAbstractSyntaxTree AST;

#endif // IABSTRACT_SYNTAX_TREE_H
