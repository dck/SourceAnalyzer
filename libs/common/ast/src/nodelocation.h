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

#ifndef NODELOCATION_H
#define NODELOCATION_H

#include <string>

#include "inodelocation.h"

class NodeLocation : public INodeLocation
{
    public:
        NodeLocation () {}
        NodeLocation ( const size_t pos,
                       const size_t line,
                       const std::string& file )
            : _pos(pos), _line(line), _file(file){}

        virtual size_t      getPos  () const;
        virtual size_t      getLine () const;
        virtual std::string getFile () const;

        virtual void setPos  ( const size_t pos  );
        virtual void setLine ( const size_t line );
        virtual void setFile ( const std::string& file );

       // friend bool operator == ( const INodeLocation& nodeLocation1, const INodeLocation& nodeLocation2 );
       // friend bool operator != ( const INodeLocation& nodeLocation1, const INodeLocation& nodeLocation2 );

    private:
        size_t       _pos;
        size_t       _line;
        std::string  _file;
};

#endif //NODELOCATION_H
