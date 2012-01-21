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

#ifndef DB_H
#define DB_H

int create_db    (database_t** ppdb, const char* name);
int open_db      (database_t** ppdb, const char* name);
int close_db     (database_t** ppdb);
int put_ent      (database_t*  pdb, void* pPK, uint16 keylen, void* pdata, uint32 datalen, uint8 ID);
int get_ent      (database_t*  pdb, void* pFK, uint16 keylen, sa3list*  lst, uint8 ID);
int get_all_ent  (database_t*  pdb, sa3list* lst, uint8 ID);
int is_ent_exist (database_t*  pdb, void* pkey, uint16 keylen, void* pdata, uint32 datalen, uint8 ID, uint8* p_existance_flag);

// int get_ent_pk    (database_t* pdb, void* pFK,  uint16 keylen, void** ppdata, uint32* datalen);
// int getfirstbykey (database_t* pdb, void* key,  uint16 keylen, void** data,   uint32* datalen);

#endif // DB_H
