#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"


/* #include "duf_sql.h" */
/* #include "duf_create.h" */
/* #include "duf_path.h" */
/* #include "duf_file.h" */
/* #include "duf_file_md5id.h" */
/* #include "duf_file_pathid.h" */
/* #include "duf_finddup.h" */
/* #include "duf_update.h" */


/* ###################################################################### */
#include "duf_remove.h"
/* ###################################################################### */


/* void                                                                                                                             */
/* remove_same_files_from_same_dirs( char *path_ids, int targc, char **targv )                                                      */
/* {                                                                                                                                */
/*   int ia;                                                                                                                        */
/*                                                                                                                                  */
/*   (* for ( int ia = 0; ia < targc; ia++ ) *)                                                                                     */
/*   ia = 0;                                                                                                                        */
/*                                                                                                                                  */
/*   {                                                                                                                              */
/*     DIR *d;                                                                                                                      */
/*                                                                                                                                  */
/*     (* fprintf( stderr, "%u. [%s]\n", 0, targv[ia] ); *)                                                                         */
/*     d = opendir( targv[ia] );                                                                                                    */
/*     {                                                                                                                            */
/*       struct stat st;                                                                                                            */
/*       struct dirent *de;                                                                                                         */
/*                                                                                                                                  */
/*       while ( ( de = readdir( d ) ) )                                                                                            */
/*       {                                                                                                                          */
/*         char *fpath;                                                                                                             */
/*                                                                                                                                  */
/*         fpath = join_path( targv[ia], de->d_name );                                                                              */
/*         {                                                                                                                        */
/*           stat( fpath, &st );                                                                                                    */
/*           if ( S_ISREG( st.st_mode ) )                                                                                           */
/*           {                                                                                                                      */
/*             (* fprintf( stderr, "[%8lu %o %5lu %4u:%4u %9lu] %s\n", st.st_ino, st.st_mode, st.st_nlink, st.st_uid, st.st_gid, *) */
/*             (*          st.st_size, de->d_name );                                                                             *) */
/*             (* fprintf( stderr, "[%8lu %9lu] %s", st.st_ino, st.st_size, de->d_name ); *)                                        */
/*             (* fprintf( stderr, ". ..... ... compare each file with siblings..." );    *)                                        */
/*             (* fprintf( stderr, "\n" );                                                *)                                        */
/*             {                                                                                                                    */
/*               {                                                                                                                  */
/*                 int same_filesc = 0;                                                                                             */
/*                 char **same_filesv = NULL;                                                                                       */
/*                                                                                                                                  */
/*                 same_filesc = files_by_md5id( path_ids, same_filesc, &same_filesv, md5id_of_file( targv[ia], de->d_name ) );     */
/*               }                                                                                                                  */
/*               (* fprintf( stderr, "%s\n", fpath ); *)                                                                            */
/*             }                                                                                                                    */
/*           }                                                                                                                      */
/*           (* "SELECT * FROM duf_keydata WHERE " *)                                                                               */
/*         }                                                                                                                        */
/*         mas_free( fpath );                                                                                                       */
/*       }                                                                                                                          */
/*     }                                                                                                                            */
/*     closedir( d );                                                                                                               */
/*     (* break; *)                                                                                                                 */
/*   }                                                                                                                              */
/* }                                                                                                                                */
