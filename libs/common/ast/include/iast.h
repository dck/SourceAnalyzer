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

#ifndef IABSTRACT_SYNTAX_TREE_H
#define IABSTRACT_SYNTAX_TREE_H

    #include "inode.h"

    class IAbstractSyntaxTree
    {
        public:
            virtual ~IAbstractSyntaxTree() = 0;

            virtual INode* getRoot() const = 0;
            virtual void   setRoot( INode* root ) = 0;

            virtual INode* getRBottom () const = 0;
            virtual INode* getLBottom () const = 0;

            class iterator
            {
                public:
                    iterator ( IAbstractSyntaxTree* ast = NULL );
                    iterator ( const iterator& _iterator );

                    virtual iterator& root       ();

                    virtual iterator& left       ();
                    virtual iterator& operator-- ();
                    virtual iterator  operator-- (int);

                    virtual iterator& right      ();
                    virtual iterator& operator++ ();
                    virtual iterator  operator++ (int);

                    virtual iterator& up       () throw();
                    virtual iterator& downToR  () throw();
                    virtual iterator& downToL  () throw();

                    virtual iterator& operator= ( const iterator& _iterator );

                    virtual INode*        operator*  () const throw();
                    virtual INode*        operator-> () const throw();

                protected:
                    virtual INode*                getCurrentNode () const;
                    virtual IAbstractSyntaxTree*  getAST         () const;

                    virtual void setCurrentNode ( INode* currentNode );
                    virtual void setAST         ( IAbstractSyntaxTree* ast );

                private:
                    IAbstractSyntaxTree*   _ast;
                    INode*                 _currentNode;
            };

    };

    typedef IAbstractSyntaxTree IAST;

#endif // IABSTRACT_SYNTAX_TREE_H
