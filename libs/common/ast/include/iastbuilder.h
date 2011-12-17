
#ifndef IAST_BUILDER_H
#define IAST_BUILDER_H

    #include "inode.h"
    #include "iast.h"

    class IASTBuilder {
        public:
            virtual void push ( INode* node ) = 0;
            virtual void buildNode () = 0;
            virtual AST* getAST    () = 0;
    };

#endif // IAST_BUILDER_H

