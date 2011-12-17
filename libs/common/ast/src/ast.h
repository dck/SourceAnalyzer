
#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

    #include "inode.h"

    class AbstractSyntaxTree : public IAbstractSyntaxTree {
        public:
            virtual INode* getRoot() const;

        private:
            INode* root;
    };

#endif // ABSTRACT_SYNTAX_TREE_H
