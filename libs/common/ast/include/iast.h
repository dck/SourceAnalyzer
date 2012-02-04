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
        class iterator;
        class null_iterator;

        virtual ~IAbstractSyntaxTree() {}

        virtual iterator getRoot() const = 0;
        virtual void     setRoot( INode* root ) = 0;
        virtual void     setRoot( const iterator& root ) = 0;

        virtual iterator getRBottom () const = 0;
        virtual iterator getLBottom () const = 0;

        static null_iterator null_it;
};

class IAbstractSyntaxTree::iterator
{
    public:
        iterator ( INode* node = NULL );
        iterator ( const iterator& iterator );

        virtual ~iterator() {}

        virtual iterator& left       ();
        virtual iterator& operator-- ();
        virtual iterator  operator-- (int);

        virtual iterator& right      ();
        virtual iterator& operator++ ();
        virtual iterator  operator++ (int);

        virtual iterator& up       () throw();
        virtual iterator& downToR  () throw();
        virtual iterator& downToL  () throw();

        virtual iterator& operator =  ( const iterator& iterator );
        friend  bool      operator == ( const iterator& it1, const iterator& it2 );
        friend  bool      operator != ( const iterator& it1, const iterator& it2 );

        friend  bool      operator == ( const iterator& it1, const IAbstractSyntaxTree::null_iterator& it2 );
        friend  bool      operator != ( const iterator& it1, const IAbstractSyntaxTree::null_iterator& it2 );

        friend  bool      operator == ( const IAbstractSyntaxTree::null_iterator& it1, const iterator& it2 );
        friend  bool      operator != ( const IAbstractSyntaxTree::null_iterator& it1, const iterator& it2 );

        virtual INode*    operator*  () const throw();
        virtual INode*    operator-> () const throw();

    protected:
        inline INode* getCurrentNode () const               { return _currentNode;        }
        inline void   setCurrentNode ( INode* currentNode ) { _currentNode = currentNode; }
        inline bool   isNull         () const               { return _isNull;             }
        inline void   setNull        ( bool isNull )        { _isNull = isNull;           }

    private:
        INode* _currentNode;
        bool   _isNull;
};

class IAbstractSyntaxTree::null_iterator : public IAbstractSyntaxTree::iterator
{
    public:
        null_iterator( INode* node = NULL )
            : iterator(node) { setNull(true); }

        virtual iterator& left       ()     throw();
        virtual iterator& operator-- ()     throw();
        virtual iterator  operator-- (int)  throw();

        virtual iterator& right      ()     throw();
        virtual iterator& operator++ ()     throw();
        virtual iterator  operator++ (int)  throw();

        virtual iterator& up       () throw();
        virtual iterator& downToR  () throw();
        virtual iterator& downToL  () throw();

        //virtual iterator& operator =  ( const iterator& iterator );

        friend  bool      operator == ( const null_iterator& it1, const iterator& it2 );
        friend  bool      operator != ( const null_iterator& it1, const iterator& it2 );

        friend  bool      operator == ( const null_iterator& it1, const null_iterator& it2 );
        friend  bool      operator != ( const null_iterator& it1, const null_iterator& it2 );

        friend  bool      operator == ( const iterator& it1, const null_iterator& it2 );
        friend  bool      operator != ( const iterator& it1, const null_iterator& it2 );

        virtual INode*    operator*  () const throw();
        virtual INode*    operator-> () const throw();
};

typedef IAbstractSyntaxTree IAST;

#endif // IABSTRACT_SYNTAX_TREE_H
