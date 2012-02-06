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

#ifndef ASTDEPTHWALKER_H
#define ASTDEPTHWALKER_H

#include "iastwalker.h"

class ASTDepthWalker : public IASTWalker
{
    public:
    //    ASTDepthWalker( IASTWalker& astWalker )
       //     : _it(astWalker.getIt()), _eot(false) {}
        ASTDepthWalker( const IAST::iterator& root )
            : _it(root), _eot(true) {}
        virtual ~ASTDepthWalker(){}

        virtual IASTWalker& next ();

        inline INode*           element ()       { return *_it; }
        inline bool             isNext  () const { return _eot; }
        inline IAST::iterator&  getIt   ()       { return _it;  }
        inline void             setIt   ( const IAST::iterator& root ) { _it = root; _eot = true; }

    private:
        IAST::iterator _it;
        bool           _eot;
};

#endif // ASTDEPTHWALKER_H
