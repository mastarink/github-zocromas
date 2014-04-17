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
#include "duf_filename.h"
/* ###################################################################### */



/* unsigned long long                                                                                                     */
/* duf_insert_filename( const char *fname, unsigned long long dir_id, unsigned long long resd )                           */
/* {                                                                                                                      */
/*   unsigned long long resf = -1;                                                                                        */
/*   int r;                                                                                                               */
/*   char *qfname;                                                                                                        */
/*                                                                                                                        */
/*   qfname = duf_single_quotes_2( fname );                                                                               */
/*   r = duf_sql_c( "INSERT INTO duf_filenames (pathid, dataid, name, ucnt, now) values ('%lu','%lu','%s',0,datetime())", */
/*                  DUF_CONSTRAINT_IGNORE_YES, dir_id, resd, qfname ? qfname : fname );                                   */
/*   if ( !r (* assume SQLITE_OK *)  )                                                                                    */
/*   {                                                                                                                    */
/*     resf = duf_last_insert_rowid(  );                                                                                  */
/*     (* fprintf( stderr, "INSERT INTO duf_filenames :: %llu. [%s]\x1b[K\n", resf, qfname ? qfname : fname ); *)         */
/*   }                                                                                                                    */
/*   else if ( r != duf_constraint )                                                                                      */
/*     fprintf( stderr, "ERROR %s %d\n", __func__, r );                                                                   */
/*                                                                                                                        */
/*   mas_free( qfname );                                                                                                  */
/*   return resf;                                                                                                         */
/* }                                                                                                                      */
