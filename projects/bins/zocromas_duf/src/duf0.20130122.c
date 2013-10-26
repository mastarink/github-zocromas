#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <openssl/md5.h>


#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-byte-order.h>
#include <libexif/exif-data-type.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-log.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-content.h>
#include <libexif/exif-mnote-data.h>
#include <libexif/exif-mem.h>
#include <libexif/exif-log.h>
#include <libexif/exif-mem.h>



/*
./sh/run_duf.sh /mnt/new_media/media/Photo/ /mnt/new_media/media/Pictures.R.20120207.164339/ /mnt/miscn/video/Pictures/ /home/mastar/a/to-attach/ /home/mastar/a/zaurus/sd/Documents/image/ /home/mastar/.local/share/Trash/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/tall/htclegend/ /mnt/tall/atalk/backup/

./sh/run_duf.sh /mnt/new_media/media/down/

*/

/*
 * mkdir m4
 * autoreconf --install
 * ./configure 
 */
#include <sqlite3.h>

sqlite3 *pDb = NULL;
char *errmsg = NULL;

int
check_tables( void )
{
  int r;
  char *errmsg;

  /* char **presult=NULL; */

  /* int row, column; */

  /* r = sqlite3_get_table( pDb, "select name from sqlite_master  where type='table'", (* SQL to be evaluated *) */
  /*                        &presult, (* Results of the query *)                                                 */
  /*                        &row,  (* Number of result rows written here *)                                      */
  /*                        &column, (* Number of result columns written here *)                                 */
  /*                        &errmsg (* Error msg written here *)                                                 */
  /*        );                                                                                                   */
  /* for ( int ir = 1; ir <= row; ir+=column )                                                                   */
  /* {                                                                                                           */
  /*   char *prow;                                                                                               */
  /*                                                                                                             */
  /*   prow = presult[ir];                                                                                       */
  /*   if ( 0 == strcmp( prow, "files" ) )                                                                       */
  /*     table_files_ok = 1;                                                                                     */
  /*   else if ( 0 == strcmp( prow, "paths" ) )                                                                  */
  /*     table_paths_ok = 1;                                                                                     */
  /*   else if ( 0 == strcmp( prow, "md5" ) )                                                                    */
  /*     table_md5_ok = 1;                                                                                       */
  /*   fprintf( stderr, "%s\n", prow );                                                                          */
  /* }                                                                                                           */
  /* sqlite3_free_table( presult );                                                                              */
  /* presult = NULL; */

  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS "
                    " filedatas (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, size INTEGER, md5id INTEGER, now REAL)",
                    NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS filedatas_md5id ON filedatas (md5id)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS filedatas_size ON filedatas (size)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS filedatas_uniq ON filedatas (dev,inode)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );


  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS "
                    " filenames (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, name TEXT, pathid INTEGER, now REAL)",
                    NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS filenames_filename ON filenames (name)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS filenames_filename ON filenames (dataid)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS filenames_pathid ON filenames (pathid)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS filenames_uniq ON filenames (pathid,name)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );




  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS "
                    " paths (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, dirname text, parentid INTEGER, now REAL)",
                    NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_dirname ON paths (dirname)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS paths_uniq ON paths (dev,inode)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_inode ON paths (inode)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_parentid ON paths (parentid)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS "
                    " md5 (id INTEGER PRIMARY KEY autoincrement, md5sum1 INTEGER, md5sum2 INTEGER, now REAL)", NULL, NULL,
                    &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS md5_md5sum ON md5 (md5sum1,md5sum2)", NULL, NULL, &errmsg );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );


  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS " " exif (id INTEGER PRIMARY KEY autoincrement, now REAL)", NULL, NULL, &errmsg );


  return 0;
}

int
direntry_filter( const struct dirent *de )
{
  int r = 0;

  switch ( de->d_type )
  {
  case DT_UNKNOWN:
    break;
  case DT_FIFO:
    break;
  case DT_CHR:
    break;
  case DT_DIR:
    r = 1;
    break;
  case DT_BLK:
    break;
  case DT_REG:
    /* fprintf( stderr, ">>>> %u : %lu : %s\n", de->d_type, de->d_ino, de->d_name ); */
    r = 1;
    break;
  case DT_LNK:
    break;
  case DT_SOCK:
    break;
  case DT_WHT:
    break;
  }
  return r;
}

char *
single_quotes_2( char *s )
{
  char *r = NULL;

  if ( strchr( s, '\'' ) )
  {
    size_t len;
    char *ip, *op;

    len = strlen( s );
    r = malloc( 2 * ( len + 1 ) );
    /* fprintf( stderr, "Q:[%s] (%lu)\n", s, len ); */
    ip = s;
    op = r;
    while ( ip && *ip && op )
    {
      if ( *ip == '\'' )
        *op++ = '\'';
      *op++ = *ip;
      ip++;
    }
    *op++ = 0;
  }
  return r;
}

sqlite3_int64
update_path( char *path, sqlite3_int64 rs_up, int recurse, int dofiles )
{
  sqlite3_int64 res = 0;
  int r = 0;
  char rpath[PATH_MAX];
  char *dir_name;
  char *base_name;
  struct stat st_dir;

  ( void ) realpath( path, rpath );
  if ( *rpath && strlen( rpath ) > 1 )
  {
    base_name = basename( rpath );
    dir_name = dirname( rpath );
    if ( !*base_name && *dir_name && 0 == strcmp( dir_name, "/" ) )
    {
      base_name = path + 1;
      /* fprintf( stderr, "CPATH: %s : %s -- %s\n", rpath, path, base_name ); */
    }
    r = stat( path, &st_dir );
    if ( !r )
    {
      char *sql;
      char *qbase_name;

      qbase_name = single_quotes_2( base_name );
      if ( rs_up < 0 )
        rs_up = update_path( dir_name, -1, 0, 0 );
      sql = sqlite3_mprintf( "INSERT INTO paths (dev,inode,dirname,parentid,now) values ('%lu','%lu','%s','%lu',datetime())",
                             st_dir.st_dev, st_dir.st_ino, qbase_name ? qbase_name : base_name, rs_up );
      free( qbase_name );
      /* fprintf( stderr, "'%s' : '%s' : '%s' :: { %lu : %lu : %s } : %s\n", path, dir_name, base_name, st_dir.st_dev, */
      /*          st_dir.st_ino, dir_name, sql );                                                                      */
      r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );
      sqlite3_free( sql );
      if ( r == SQLITE_CONSTRAINT )
      {
        char *sql;
        int row, column;
        char **presult = NULL;

        sql = sqlite3_mprintf( "SELECT id FROM paths WHERE dev='%lu' and inode='%lu'", st_dir.st_dev, st_dir.st_ino );
        r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
        if ( r == SQLITE_OK )
        {
          if ( row )
            for ( int ir = column; ir <= column * row; ir += column )
            {
              char *prow;

              prow = presult[ir];
              /* fprintf( stderr, ">>>>>>>>>>>>>>>>> %s\n", prow ); */
              res = strtol( prow, NULL, 10 );
            }
        }
        else
          fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
        sqlite3_free_table( presult );
        sqlite3_free( sql );
      }
      else if ( r == SQLITE_OK )
        res = sqlite3_last_insert_rowid( pDb );
      else
        fprintf( stderr, "%d r=%d '%s' [%s]\n", __LINE__, r, errmsg ? errmsg : "-", sql );

      /* fprintf( stderr, "last_insert_rowid %lld\n", res ); */
      /* fprintf( stderr, "path %s\n", path );          */
    }

    if ( res > 0 )
    {
      char wpath[PATH_MAX];

      *wpath = 0;
      if ( dir_name[0] && dir_name[1] )
        strcat( wpath, dir_name );
      strcat( wpath, "/" );
      strcat( wpath, base_name );
      /* fprintf( stderr, "%s '%s/%s' (%s)\n", wpath, dir_name, base_name, path ); */
      {
        struct dirent **list = NULL;
        int nlist;

        nlist = scandir( wpath, &list, direntry_filter, alphasort );
        if ( nlist < 0 )
        {
          fprintf( stderr, "%s: nlist = %d\n", wpath, nlist );
          sleep( 3 );
        }
        while ( nlist > 0 && nlist-- )
        {
          if ( recurse && list[nlist]->d_type == DT_DIR
               && !( list[nlist]->d_name[0] == '.' && ( list[nlist]->d_name[1] == '.' || list[nlist]->d_name[1] == 0 ) ) )
          {
            char recpath[PATH_MAX];

            *recpath = 0;
            strcat( recpath, wpath );
            strcat( recpath, "/" );
            strcat( recpath, list[nlist]->d_name );
            /* fprintf( stderr, "=R===== %s : %s\n", recpath, list[nlist]->d_name ); */

            update_path( recpath, res, recurse, dofiles );
          }
          else if ( dofiles && list[nlist]->d_type == DT_REG )
          {
            sqlite3_int64 resd = 0;
            char *sql;
            struct stat st_file;
            size_t fsz = 0;

            {
              char *fpath;
              const char *fname;

              fname = list[nlist]->d_name;
              fpath = malloc( strlen( path ) + strlen( fname ) + 2 );
              strcpy( fpath, path );
              strcat( fpath, "/" );
              strcat( fpath, fname );

              r = stat( fpath, &st_file );
              if ( !r )
              {
                fsz = st_file.st_size;
                /* fprintf( stderr, ">>> %lu : %s\n", fsz, path ); */
              }
              free( fpath );
            }
            sql = sqlite3_mprintf( "INSERT INTO filedatas (dev,inode,size,md5id,now) values ('%lu','%lu','%lu','%lu',datetime())",
                                   st_dir.st_dev, list[nlist]->d_ino, fsz, 0 );
            r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );
            sqlite3_free( sql );
            if ( r == SQLITE_CONSTRAINT )
            {
              char *sql;
              int row, column;
              char **presult = NULL;

              sql = sqlite3_mprintf( "SELECT id FROM filedatas WHERE dev='%lu' and inode='%lu'", st_dir.st_dev,
                                     list[nlist]->d_ino );
              r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
              if ( r == SQLITE_OK )
              {
                if ( row )
                  for ( int ir = column; ir <= column * row; ir += column )
                  {
                    char *prow;

                    prow = presult[ir];
                    /* fprintf( stderr, ">>>>>>>>>>>>>>>>> %s\n", prow ); */
                    resd = strtol( prow, NULL, 10 );
                  }
              }
              else
                fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
              sqlite3_free_table( presult );
              sqlite3_free( sql );
            }
            else if ( r == SQLITE_OK )
              resd = sqlite3_last_insert_rowid( pDb );
            else
              fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

            {
              char *fname;

              fname = single_quotes_2( list[nlist]->d_name );
              sql = sqlite3_mprintf( "INSERT INTO filenames (pathid,dataid,name,now) values ('%lu','%lu','%s',datetime())",
                                     res, resd, fname ? fname : list[nlist]->d_name );
              r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );

              if ( r == SQLITE_OK )
              {
                sqlite3_int64 resf = -1;

                resf = sqlite3_last_insert_rowid( pDb );
                fprintf( stderr, "%llu. [%s]\x1b[K\r", resf, fname ? fname : list[nlist]->d_name );
              }
              else if ( r == SQLITE_CONSTRAINT )
              {
              }
              else
                fprintf( stderr, "%d r=%d '%s' - [%s]\n", __LINE__, r, errmsg ? errmsg : "-", sql );
              if ( fname )
                free( fname );
            }
            sqlite3_free( sql );
          }

          free( list[nlist] );
        }
        free( list );
      }
    }
  }
  return res;
}

char *
filepath( sqlite3_int64 pathid )
{
  char *path = NULL;
  char *name = NULL;
  sqlite3_int64 parentid = -1;

  /* fprintf( stderr, "((( %llu )))\n", pathid ); */
  {
    int r;
    char *sql;
    int row, column;
    char **presult = NULL;

    sql = sqlite3_mprintf( "SELECT parentid, dirname FROM paths WHERE id=%llu", pathid );
    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row == 1 )
      {
        name = strdup( presult[3] );
        parentid = strtol( presult[2], NULL, 10 );
        /* fprintf( stderr, "%llu - %s\n", parentid, name ); */
      }
    }
    else
      fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
  }
  if ( name && parentid >= 0 )
  {
    char *parent = NULL;

    parent = filepath( parentid );

    path = malloc( strlen( name ) + ( parent ? strlen( parent ) : 0 ) + 2 );
    if ( parent )
      strcpy( path, parent );
    strcat( path, "/" );
    strcat( path, name );
    if ( parent )
      free( parent );
  }
  if ( name )
    free( name );
  return path;
}

void
update_md5( void )
{
  char *sql;
  int row, column;
  char **presult = NULL;
  int r;

  sql = sqlite3_mprintf( "SELECT filedatas.id, filedatas.dev, filedatas.inode, filenames.pathid, filenames.name "
                         " FROM filedatas " " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) " " WHERE md5id=0" );
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  /* fprintf( stderr, "%ux%u : %s\n", row, column, sql ); */
  if ( r == SQLITE_OK )
  {
    if ( row )
    {
      char *path = NULL;
      sqlite3_int64 pathid = -1, old_pathid = -1;

      for ( int ir = column; ir <= column * row; ir += column )
      {
        char *prow;
        char *fname;
        sqlite3_int64 filedataid;

        prow = presult[ir];
        /* fprintf( stderr, ">>>>>> %10s %15s %5s %5s [%s]\n", presult[ir], presult[ir + 1], presult[ir + 2], presult[ir + 3], */
        /*          presult[ir + 4] );                                                                                         */
        fname = presult[ir + 4];
        filedataid = strtoll( presult[ir], NULL, 10 );
        {
          MD5_CTX ctx;
          unsigned char md[MD5_DIGEST_LENGTH];

          /* char smd[33]; */
          if ( presult[ir + 3] )
            pathid = strtoll( presult[ir + 3], NULL, 10 );
          else
          {
            fprintf( stderr, "No pathid for '%s'\n", presult[ir + 4] );
            fprintf( stderr, ">>>>>> %10s %15s %5s %5s [%s]\n", presult[ir], presult[ir + 1], presult[ir + 2], presult[ir + 3],
                     presult[ir + 4] );
            return;
          }
          if ( old_pathid != pathid || !path )
          {
            if ( path )
              free( path );
            path = filepath( pathid );
          }
          {
            char *fpath;

            fpath = malloc( strlen( path ) + strlen( fname ) + 2 );
            strcpy( fpath, path );
            strcat( fpath, "/" );
            strcat( fpath, fname );
            {
              FILE *f;

              f = fopen( fpath, "r" );
              if ( f )
              {
                sqlite3_int64 resmd = -1;
                char *buffer;
                size_t bufsz = 1024 * 1024 * 10;

                buffer = malloc( bufsz );
                /* fprintf( stderr, "Opened fpath: '%s'\n", fpath ); */
                fprintf( stderr, "%u. %s\x1b[K\r", ir / column, fpath );
                MD5_Init( &ctx );
                while ( !feof( f ) )
                {
                  int r;

                  r = fread( buffer, 1, bufsz, f );
                  /* fprintf( stderr, "READ %d\n", r ); */
                  /* fprintf( stderr, "Update %d\n", r ); */
                  MD5_Update( &ctx, buffer, r );
                }
                fclose( f );
                free( buffer );
                MD5_Final( md, &ctx );
                /* {                                             */
                /*   for ( int is = 0; is < sizeof( md ); is++ ) */
                /*   {                                           */
                /*     unsigned char c;                          */
                /*                                               */
                /*     c = md[is];                               */
                /*     (* fprintf( stderr, "%02x\n", c ); *)     */
                /*     snprintf( &smd[is * 2], 3, "%02x", c );   */
                /*   }                                           */
                /* }                                             */
                /* fprintf( stderr, "Got md5 sum: %s\n", smd ); */
                {
                  unsigned long long *md64;

                  md64 = ( ( unsigned long long * ) &md[0] );
                  /* fprintf( stderr, "%016llx - %016llx\n", md64[1], md64[0] ); */
                  {
                    char *sql;
                    int r;

                    sql = sqlite3_mprintf( "INSERT INTO md5 (md5sum1,md5sum2,now) values ('%llu','%llu',datetime())", md64[1],
                                           md64[0] );
                    r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );

                    if ( r == SQLITE_CONSTRAINT )
                    {
                      char *sql;
                      int row, column;
                      char **presult = NULL;

                      sql = sqlite3_mprintf( "SELECT id FROM md5 WHERE md5sum1='%llu' and md5sum2='%llu'", md64[1], md64[0] );
                      r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
                      if ( r == SQLITE_OK && row )
                        for ( int ir = column; ir <= column * row; ir += column )
                        {
                          char *prow;

                          prow = presult[ir];
                          /* fprintf( stderr, ">>>>>>>>>>>>>>>>> %s\n", prow ); */
                          resmd = strtol( prow, NULL, 10 );
                        }
                      else
                        fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
                      sqlite3_free_table( presult );
                      sqlite3_free( sql );
                    }
                    else if ( r == SQLITE_OK )
                      resmd = sqlite3_last_insert_rowid( pDb );
                    else
                      fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

                    sqlite3_free( sql );
                  }
                }
                fprintf( stderr, "%u. resmd: %llu : %s\x1b[K\r", ir / column, resmd, fpath );
                {
                  char *sql;

                  sql = sqlite3_mprintf( "UPDATE filedatas SET md5id='%llu' WHERE id='%lld'", resmd, filedataid );
                  /* fprintf( stderr, "%d '%s'\n", __LINE__, sql ); */
                  r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );
                  if ( r != SQLITE_OK )
                    fprintf( stderr, "%d r=%d '%s' - %s\n", __LINE__, r, errmsg ? errmsg : "-", sql );
                  sqlite3_free( sql );
                }
              }
              else
              {
                fprintf( stderr, "ERROR open file : %s\n", fpath );
              }
            }
            free( fpath );
          }
          old_pathid = pathid;
        }
      }
      if ( path )
        free( path );
    }
  }
  else
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
  sqlite3_free_table( presult );
  sqlite3_free( sql );
}

void
update_exif( void )
{

  char *sql;
  int row, column;
  char **presult = NULL;
  int r;

  sql = sqlite3_mprintf( "SELECT filedatas.id, filedatas.dev, filedatas.inode, filenames.pathid, filenames.name "
                         " FROM filedatas " " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) "
                         " WHERE filenames.name LIKE '%%.JPG' OR filenames.name LIKE '%%.JPEG' " " LIMIT 100000" );
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  /* fprintf( stderr, "%ux%u : %s\n", row, column, sql ); */
  if ( r == SQLITE_OK )
  {
    if ( row )
    {

      for ( int ir = column; ir <= column * row; ir += column )
      {
        char *prow;
        char *fname;
        char *path = NULL;
        sqlite3_int64 pathid;

        prow = presult[ir];
        /* fprintf( stderr, ">>>>>> %10s %15s %5s %5s [%s]\n", presult[ir], presult[ir + 1], presult[ir + 2], presult[ir + 3], */
        /*          presult[ir + 4] );                                                                                         */

        pathid = strtoll( presult[ir + 3], NULL, 10 );
        path = filepath( pathid );
        fname = presult[ir + 4];
        /* fprintf( stderr, "fname:[%s]...\n", fname ); */
        if ( fname && path )
        {
          char *fpath;

          fpath = malloc( strlen( path ) + strlen( fname ) + 2 );
          strcpy( fpath, path );
          strcat( fpath, "/" );
          strcat( fpath, fname );
          fprintf( stderr, "%s\x1b[K\r", fpath );
          printf( "%s\n", fpath );
          {
            ExifLoader *xl;
            ExifData *xd;

            xl = exif_loader_new(  );
            exif_loader_write_file( xl, fpath );

            xd = exif_loader_get_data( xl );
            exif_data_dump( xd );

            exif_data_free( xd );
            exif_loader_reset( xl );
            exif_loader_unref( xl );
          }
          free( fpath );
        }
        free( path );
      }
    }
  }
}

/* file:///usr/share/doc/sqlite-3.7.15.2/html/docs.html */
int
main( int argc, char **argv )
{
  int r;

  /* char *s; */
  /* char rpath[PATH_MAX]; */

  /* s = realpath( ".", rpath ); */
  /* fprintf( stderr, ">>> %s\n", rpath ); */

  r = sqlite3_initialize(  );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  r = sqlite3_open( "duf20130122.sqlite3", &pDb );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );

  {
    char *sql;

    sql = sqlite3_mprintf( "PRAGMA synchronous = OFF" );
    r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );
    if ( r != SQLITE_OK )
      fprintf( stderr, "%d r=%d '%s' - %s\n", __LINE__, r, errmsg ? errmsg : "-", sql );
    sqlite3_free( sql );
  }

  check_tables(  );
  for ( int ia = 1; ia < argc; ia++ )
  {
    update_path( argv[ia], -1, 1, 1 );
  }
  update_md5(  );

  /* update_exif(  ); */


  {
    char *sql;
    int row, column;
    char **presult = NULL;

    sql = sqlite3_mprintf( "SELECT " " filedatas.md5id, COUNT(*) as cnt "
                           " FROM md5 " " LEFT JOIN filedatas ON (md5.id=filedatas.md5id) "
                           " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) "
                           " LEFT JOIN paths ON (filenames.pathid=paths.id) "
                           " WHERE filenames.name LIKE '%%.jpg' OR filenames.name " " LIKE '%%.jpeg' "
                           " GROUP BY md5sum1,md5sum2 " " HAVING cnt>1 " " ORDER BY cnt DESC" );
    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row )
        for ( int ir = column; ir <= column * row; ir += column )
        {
          int r2;
          sqlite3_int64 md5id;
          char *sqlg;
          int row2, column2;
          char **presult2 = NULL;

          md5id = strtoll( presult[ir], NULL, 10 );
          fprintf( stderr, "md5id: %llu ========================================================\n", md5id );
          sqlg = sqlite3_mprintf( "SELECT filenames.pathid, filedatas.inode, filedatas.md5id, filedatas.size, filenames.name "
                                  " FROM md5 " " LEFT JOIN filedatas ON (md5.id=filedatas.md5id) "
                                  " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) "
                                  " LEFT JOIN paths ON (filenames.pathid=paths.id) " " WHERE " "filedatas.md5id='%llu'", md5id );

          /* fprintf( stderr, "%s\n", sqlg ); */
          r2 = sqlite3_get_table( pDb, sqlg, &presult2, &row2, &column2, &errmsg );
          /* fprintf( stderr, "%u\n", row2 ); */
          if ( r2 == SQLITE_OK )
          {
            if ( row2 )
              for ( int ir2 = column2; ir2 <= column2 * row2; ir2 += column2 )
              {
                char *path;
                char *fname;
                size_t fsize;

                fname = presult2[ir2 + 4];
                fsize = strtol( presult2[ir2 + 3], NULL, 10 );
                path = filepath( strtoll( presult2[ir2], NULL, 10 ) );
                /*
                   fprintf( stderr, "ir2: %u - %s - %s\n", ir2, fname, path ); */
                if ( fname && path )
                {
                  char *fpath;

                  fpath = malloc( strlen( path ) + strlen( fname ) + 2 );
                  strcpy( fpath, path );
                  strcat( fpath, "/" );
                  strcat( fpath, fname );

                  fprintf( stderr, "pathid: %5s inode: %9s md5id: %5s size:%lu fpath: %s\n", presult2[ir2], presult2[ir2 + 1],
                           presult2[ir2 + 2], fsize, fpath );
                  free( fpath );
                }
                if ( path )
                  free( path );
              }
          }
          else
            fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
          sqlite3_free_table( presult2 );
          sqlite3_free( sqlg );
        }
    }
    else
      fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
  }



  r = sqlite3_close( pDb );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );


  r = sqlite3_shutdown(  );
  if ( r != SQLITE_OK )
    fprintf( stderr, "%d r=%d '%s'\n", __LINE__, r, errmsg ? errmsg : "-" );
  return 0;
}
