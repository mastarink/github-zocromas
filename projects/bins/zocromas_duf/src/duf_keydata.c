#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"


/* ###################################################################### */
#include "duf_keydata.h"
/* ###################################################################### */


/* unsigned long long                                                                                                                  */
/* duf_insert_keydata( unsigned long long pathid, unsigned long long filenameid, ino_t inode, unsigned long long resmd )               */
/* {                                                                                                                                   */
/*   int r;                                                                                                                            */
/*   unsigned long long resp = 0;                                                                                                      */
/*                                                                                                                                     */
/*   r = duf_sql_c( "INSERT INTO duf_keydata (pathid, filenameid, md5id, ucnt, now) " " VALUES ('%llu', '%llu', '%llu',0,datetime())", */
/*                  DUF_CONSTRAINT_IGNORE_YES, pathid, filenameid, resmd );                                                            */
/*   if ( r == duf_constraint )                                                                                                        */
/*   {                                                                                                                                 */
/*   }                                                                                                                                 */
/*   else if ( !r (* assume SQLITE_OK *)  )                                                                                            */
/*     resp = duf_last_insert_rowid(  );                                                                                               */
/*   else                                                                                                                              */
/*     fprintf( stderr, "error duf_insert_keydata %d\n", r );                                                                          */
/*   return resp;                                                                                                                      */
/* }                                                                                                                                   */
