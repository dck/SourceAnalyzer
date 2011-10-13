%{
///////////////////////////////////////////////////////////////////////////////
// This file is the part of SourceAnalyzer source codes.                     //
// SourceAnalyzer is a library that search out a call-graph of               //
// given source code. See <http://trac-hg.assembla.com/SourceAnalyzer>       //
// Copyright (C) 2008-2010 SourceAnalyzer contributors                       //
//                                                                           //
// This library is free software: you can redistribute it and/or modify      //
// it under the terms of the GNU Lesser General Public License as published  //
// by the Free Software Foundation, either version 3 of the License,         //
// any later version.                                                        //
//                                                                           //
// This library is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU Lesser General Public License for more details.                       //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with this library.  If not, see <http://www.gnu.org/licenses/>.     //
///////////////////////////////////////////////////////////////////////////////
    #include <string>
    using namespace std;
    
    #define YYSTYPE string
    #include "csyntaxer.hh"

    extern size_t line;
    extern bool isInFunc;
    extern bool isInteractive;

    size_t braceLevel = 0;

    bool isSameLine = false;
    
    #ifdef YY_NO_UNISTD_H
        #include <io.h> 
    #endif // YY_NO_UNISTD_H
%}

%%
auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volitile|while yylval = yytext; return DEFINED;

"sourceanalyzerinputend"                  if( true == isInteractive ) return TERM; yylval = yytext; return UNDEFINED;

[_a-zA-Z][_a-zA-Z0-9]*   yylval = yytext; return UNDEFINED;

"*"                      yylval = yytext; if( isInFunc == true ) return OTHER; return STAR;

"# "|"#line"             isSameLine = true; return SHARP;

"("                      yylval = yytext; return OBRACET;
")"                      yylval = yytext; return EBRACET;
                       
";"                      if( isInFunc == true ) return OTHER; return SEMICOLON;

[{] yylval = yytext; braceLevel++; if( braceLevel == 1 ) return OBRACE; return OTHER;
[}] yylval = yytext; braceLevel--; if( braceLevel == 0 ) return EBRACE; return OTHER;

[0-9]+                    yylval = yytext; return OTHER;
\"[ -._a-zA-Z0-9\/\\:]*\" yylval = yytext; return OTHER;

[ \t]+                    /* ignore whitespace */
\n\r|\r\n|\n|\r if( isSameLine == true ) { isSameLine = false; return NEWLINE; } line++;

.                         yylval = yytext; return OTHER;
%%
