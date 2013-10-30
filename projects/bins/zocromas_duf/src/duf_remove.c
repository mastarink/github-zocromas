#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


/* #include <sqlite3.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>



/* #include "duf_def.h" */
#include "duf_sql.h"
#include "duf_create.h"
#include "duf_utils.h"
#include "duf_md5.h"
#include "duf_finddup.h"
#include "duf_update.h"

#include "duf_remove.h"

static int
duf_sql_files_by_md5id( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t  fun )
{
  int r = 0;
  char *fpath;

  fpath = path_id_to_path( strtoll( presult[0], NULL, 10 ) );
  fpath = join_path( fpath, presult[1] );
  fprintf( stderr, "%s\n", fpath );
  return r;
}

int
files_by_md5id( char *path_ids, int targc, char ***ptargv, unsigned long long md5id )
{
  int r = 0;

  /* char *sql; */

  r = duf_sql_select( duf_sql_files_by_md5id, NULL, NULL, 0, "SELECT pathid, name FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                      " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "
                      " WHERE duf_md5.id='%llu' AND pathid in (%s)", md5id, path_ids );
  /* {                                                                                               */
  /*   sql = sqlite3_mprintf( "SELECT pathid, name FROM duf_filenames "                              */
  /*                          " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) " */
  /*                          " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "              */
  /*                          " WHERE duf_md5.id='%llu' AND pathid in (%s)", md5id, path_ids );      */
  /*   {                                                                                             */
  /*     int r;                                                                                      */
  /*     int row, column;                                                                            */
  /*     char **presult = NULL;                                                                      */
  /*                                                                                                 */
  /*     r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                        */
  /*     if ( r == SQLITE_OK )                                                                       */
  /*     {                                                                                           */
  /*       if ( row )                                                                                */
  /*         for ( int ir = column; ir <= column * row; ir += column )                               */
  /*         {                                                                                       */
  /*                                                                                                 */
  /*           char *fpath;                                                                          */
  /*                                                                                                 */
  /*           fpath = path_id_to_path( strtoll( presult[ir + 0], NULL, 10 ) );                      */
  /*           fpath = join_path( fpath, presult[ir + 1] );                                          */
  /*           fprintf( stderr, "%s\n", fpath );                                                     */
  /*         }                                                                                       */
  /*     }                                                                                           */
  /*     else                                                                                        */
  /*       SQL_ERR( r );                                                                             */
  /*     sqlite3_free_table( presult );                                                              */
  /*   }                                                                                             */
  /*   (* {                     *)                                                                   */
  /*   (*   char buffer[2048];  *)                                                                   */
  /*   (*   char *s;            *)                                                                   */
  /*   (*                       *)                                                                   */
  /*   (*   s = gets( buffer ); *)                                                                   */
  /*   (* }                     *)                                                                   */
  /*   sqlite3_free( sql );                                                                          */
  /* }                                                                                               */
  fprintf( stderr, "....................................\n" );
  return r >= 0 ? targc : r;
}

void
remove_same_files_from_same_dirs( char *path_ids, int targc, char **targv )
{
  int ia;

  /* for ( int ia = 0; ia < targc; ia++ ) */
  ia = 0;

  {
    DIR *d;

    /* fprintf( stderr, "%u. [%s]\n", 0, targv[ia] ); */
    d = opendir( targv[ia] );
    {
      struct stat st;
      struct dirent *de;

      while ( ( de = readdir( d ) ) )
      {
        char *fpath;

        fpath = join_path( targv[ia], de->d_name );
        {
          stat( fpath, &st );
          if ( S_ISREG( st.st_mode ) )
          {
            /* fprintf( stderr, "[%8lu %o %5lu %4u:%4u %9lu] %s\n", st.st_ino, st.st_mode, st.st_nlink, st.st_uid, st.st_gid, */
            /*          st.st_size, de->d_name );                                                                             */
            /* fprintf( stderr, "[%8lu %9lu] %s", st.st_ino, st.st_size, de->d_name ); */
            /* fprintf( stderr, ". ..... ... compare each file with siblings..." );    */
            /* fprintf( stderr, "\n" );                                                */
            {
              {
                int same_filesc = 0;
                char **same_filesv = NULL;

                same_filesc = files_by_md5id( path_ids, same_filesc, &same_filesv, md5id_of_file( targv[ia], de->d_name ) );
              }
              /* fprintf( stderr, "%s\n", fpath ); */
            }
          }
          /* "SELECT * FROM duf_keydata WHERE " */
        }
        mas_free( fpath );
      }
    }
    closedir( d );
    /* break; */
  }
}
