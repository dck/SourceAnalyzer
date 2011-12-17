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

    // #define DEBUG
    #include <cstdio>
    #include <string>
    #include <iostream>
    #ifdef DEBUG
        #define DEBUGMSG( msg ) std::cerr << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl
    #endif
    /* #include "callgraph.h" */
    extern "C" {
    #include "sa3parsers.h"
    }

    using namespace std;
    #define YYSTYPE string
    #define YYERROR_VERBOSE 1

    bool isInteractive;
    int  wrapRet = 1;

    int yylex(void);
    extern "C" {
        int yywrap( void ) {
            return wrapRet;
        }
    }
    
    size_t line = 1;
    string file = "noname";
    
    void yyerror(const char *str) {
        #ifdef DEBUG
            DEBUGMSG( file << ":" << line << ": SA C Parser: " << str );
        #endif
    }
    
    extern FILE* yyin;
    
    bool   isInFunc   = false;
    string funcName   = "";
    string funcParams = "";
    sa3cg cg = NULL;
    
    string delQuotes( const string& strWithQuotes ) {
        string result = strWithQuotes;
        if( result.empty() == false ) {
            string symbol = result.substr( 0, 1 ); // first symbol
            if( symbol == "\"" || symbol == "\'" )
                result.erase( 0, 1 );
            symbol = result.substr( result.length() - 1, 1 ); // last symbol
            if( symbol == "\"" || symbol == "\'" )
                result.erase( result.length() - 1, 1 );
        }
        return result;
    }
%}

%token DEFINED UNDEFINED STAR SHARP OTHER OBRACET EBRACET SEMICOLON OBRACE EBRACE NEWLINE TERM 

%%

input: /* empty */
     | input input_instance
     | input TERM
     {
         return 0;
     }
     | input error
     {
         #ifdef DEBUG
             DEBUGMSG( file << ":" << line << ": SA C Parser: error occur; parser will continue" );
         #endif
     }
;
     
input_instance: place
              | body
              | func_definition
;

place: SHARP OTHER OTHER flags NEWLINE
     {
         line = atoi( $2.c_str() );
         file = delQuotes( $3 );
         #ifdef DEBUG
             DEBUGMSG( "Place: " << line << ":" << file );
         #endif
     }
;

flags: /* empty */
     | flags OTHER
;

func_definition: type call attribute SEMICOLON
               {
                   #ifdef DEBUG
                       DEBUGMSG( "Func definition: " << funcName << ", (" << funcParams
                              << "), " << $1 << ", " << line << ", " << file );
                   #endif
		   /* callGraph->addDefinition( funcName, funcParams, $1, line,
file, isInteractive ); */
			{
				int ret = sa3adddecl (cg, (char*) $1.c_str(), (char*) funcName.c_str(), (char*) funcParams.c_str());
				if (0 != ret) {
					cerr << "ERROR: sa3adddecl failed with " << ret << endl;
				}
			}
               }
;

func_implementation: type call attribute OBRACE
                   {
                       #ifdef DEBUG
                           DEBUGMSG( "Func implementaton: " << funcName << ", (" << funcParams
                                     << "), " << $1 << ", " << line << ", " << file );
                       #endif
		       /*callGraph->addImplementation( funcName, funcParams, $1, line, file );*/
		       {
			       int ret = sa3adddecl(cg, (char*) $1.c_str(), (char*) funcName.c_str(), (char*) funcParams.c_str());
			       if (0 != ret) {
				       cerr << "ERROR: sa3adddecl failed with " << ret << endl;
			       }
		       }
                       isInFunc = true;
                   }
;

attribute: /* empty */
         | attribute OBRACET
         | attribute param_token
         | attribute EBRACET
;

body: func_implementation EBRACE
    {
        isInFunc = false;
        #ifdef DEBUG
            DEBUGMSG( "Body (empty)" );
        #endif
    }
    | func_tokens EBRACE
    {
        isInFunc = false;
        #ifdef DEBUG
            DEBUGMSG( "Body" );
        #endif
    }
;

func_tokens: func_implementation func_token
           | func_implementation call
           | func_implementation params
           | func_implementation place
           | func_tokens func_token
           | func_tokens call
           | func_tokens params
           | func_tokens place
;

call: UNDEFINED params
    {
        $$ = $1 + " ( " + $2 + " )";
        if( isInFunc == true ) {
            #ifdef DEBUG
                DEBUGMSG( "Call: " << funcName << ", " << $1 << ", (" << $2
                          << "), " << line << ", " << file );
            #endif
	    /*callGraph->addCall( funcName, $1, $2, line, file );*/
	    {
		    int ret = sa3addcall (cg, (char*)funcName.c_str(), (char*)$1.c_str());
		    if (0 != ret) {
			    cerr << "ERROR: sa3adddecl failed with " << ret << endl;
		    }
	    }
        } else {
            funcName   = $1;
            funcParams = $2;
            #ifdef DEBUG
                DEBUGMSG( "Func definition/implementation:" );
                DEBUGMSG( "\tfuncName   = " << funcName );
                DEBUGMSG( "\tfuncParams = " << funcParams );
            #endif
        }
    }
;

params: param_tokens EBRACET
      {
          $$ = $1;
          #ifdef DEBUG
              DEBUGMSG( "Parameters: " << $$ );
          #endif
      }
      | OBRACET EBRACET
      {
          $$ = "";
          #ifdef DEBUG
              DEBUGMSG( "Empty parameters" );
          #endif
      }
;

param_tokens: OBRACET param_token
            {
                $$ = $2;
            }
            | OBRACET call
            {
                $$ = $2;
            }
            | OBRACET params // NEW
            {
                $$ = "( " + $2 + " )";
            }
            | param_tokens param_token
            {
                $$ = $1 + " " + $2;
            }
            | param_tokens call
            {
                $$ = $1 + " " + $2;
            }
            | param_tokens params // NEW
            {
                $$ = $1 + " ( " + $2 + " )";
            }
;

param_token: DEFINED
           | UNDEFINED
           | STAR
           | OTHER
;

func_token: DEFINED
          | UNDEFINED
          | OTHER
;

type: type_token
    {
        #ifdef DEBUG
            DEBUGMSG( "Type (token): " << $$ );
        #endif
    }
    | type type_token
    {
        $$ = $1 + " " + $2;
        #ifdef DEBUG
            DEBUGMSG( "Type: " << $$ );
        #endif
    }
;

type_token: DEFINED
          | UNDEFINED
          | STAR
          | OBRACET
          | EBRACET
;
%%

int cparse (sa3cg _cg) {
    int returnCode = 0;
    cg = _cg;
    yyin = stdin;
    returnCode = yyparse();
    return returnCode;
}
