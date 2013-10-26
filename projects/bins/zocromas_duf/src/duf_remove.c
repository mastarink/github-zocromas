#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


#include <sqlite3.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>



#include "duf_def.h"
#include "duf_create.h"
#include "duf_utils.h"
#include "duf_md5.h"
#include "duf_finddup.h"
#include "duf_update.h"

#include "duf_remove.h"

int
files_by_md5id( char *path_ids, int targc, char ***ptargv, sqlite3_int64 md5id )
{
  char *sql;

  sql = sqlite3_mprintf( "SELECT pathid, name FROM filenames "
                         " LEFT JOIN filedatas on (filenames.dataid=filedatas.id) "
                         " LEFT JOIN md5 on (md5.id=filedatas.md5id) "
                         " WHERE md5.id='%llu' AND pathid in (%s)", md5id, path_ids );
  {
    int r;
    int row, column;
    char **presult = NULL;

    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row )
        for ( int ir = column; ir <= column * row; ir += column )
        {

          char *fpath;

          fpath = path_id_to_path( strtoll( presult[ir + 0], NULL, 10 ) );
          fpath = join_path( fpath, presult[ir + 1] );
          fprintf( stderr, "%s\n", fpath );
        }
    }
    else
      SQL_ERR( r );
    sqlite3_free_table( presult );
  }
  fprintf( stderr, "....................................\n" );
  {
    char buffer[2048];
    char *s;

    s = gets( buffer );
  }
  sqlite3_free( sql );
  return targc;
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
          /* "SELECT * FROM keydata WHERE " */
        }
        mas_free( fpath );
      }
    }
    closedir( d );
    /* break; */
  }
}
