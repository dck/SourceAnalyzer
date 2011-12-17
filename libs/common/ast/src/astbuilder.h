
#ifndef AST_BUILDER_H
#define AST_BUILDER_H

    #include "iastbuilder.h"

    class ASTBuilder : public IASTBuilder {
        public:
            virtual void push ( INode* node );
            virtual void buildNode ();
            virtual AST* getAST    ();

        private:
            AST* ast;
    };

#endif // AST_BUILDER_H

