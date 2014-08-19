#define DUF_SQL_PDI_STMT

#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>              /* Definition of AT_* constants */
#include <sys/stat.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_path.h"
/* ###################################################################### */


char *
duf_join_path( const char *path, const char *fname )
{
  char *fpath = NULL;

  DEBUG_START(  );
  if ( fname )
  {
    if ( path )
      fpath = mas_strdup( path );
    if ( *fname != '/' && !( fpath && *fpath == '/' && fpath[1] == 0 ) )
      fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, fname );
  }
  DEBUG_ENDS( fpath );
  return fpath;
}


static char *
duf_pathid_to_path2_in( duf_sqlite_stmt_t * pstmt, unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;

  DEBUG_START(  );
  if ( dirid > 0 )
  {
/* get parentid for dirid */

    if ( pstmt )
    {
      duf_sql_reset( pstmt );
      if ( r >= 0 )
        r = duf_sql_bind_long_long( pstmt, ":dirid", dirid );
      if ( r >= 0 )
        r = duf_sql_step( pstmt );
      if ( r == DUF_SQL_ROW )
      {
        char *tail;

        DUF_UFIELD2( parentid );
        DUF_SFIELD2( dirname );
        tail = mas_strdup( dirname );
        if ( parentid >= 0 )
        {
          char *parent = NULL;

          parent = duf_pathid_to_path2_in( pstmt, parentid, pdi, &r ); /* open!! */
          DUF_TEST_R( r );
          path = duf_join_path( parent, tail );
          /* DUF_ERROR( "%llu (%s): %s @ %llu => %s", dirid, parent, tail, parentid, path ); */

          if ( tail )
            mas_free( tail );
          if ( parent )
            mas_free( parent );
        }
      }
      DUF_TEST_R( r );
      DEBUG_ENDS( path );
    }
  }
  if ( pr )
    *pr = r;
  return path;
}

char *
duf_pathid_to_path2( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;
  duf_sqlite_stmt_t *pstmt = NULL;

  {
    const char *sql = "SELECT parentid, dirname FROM " DUF_DBPREF "paths WHERE id=:dirid";

    if ( r >= 0 && sql )
      r = duf_sql_prepare( sql, &pstmt );
  }
  path = duf_pathid_to_path2_in( pstmt, dirid, pdi, &r );
  path = mas_strcat_x( path, "/" );

  {
    int rf = duf_sql_finalize( pstmt );

    pstmt = NULL;
    DUF_TEST_R( rf );
    DUF_TRACE( action, 4, "FINALIZE %s;", rf < 0 ? "FAIL" : "" );

    if ( r >= 0 || r == DUF_SQL_DONE )
      r = rf;
    DUF_TEST_R( r );
  }
  if ( pr )
    *pr = r;
  return path;
}

const char *
duf_pathid_to_path2_tmp( unsigned long long dirid, const duf_depthinfo_t * pdi )
{
  mas_free( duf_config->tmp->path );
  duf_config->tmp->path = duf_pathid_to_path2( dirid, pdi, NULL );
  return duf_config->tmp->path;
}

/* insert path into db; return id */
unsigned long long
duf_insert_path_uni2( duf_depthinfo_t * pdi, const char *dename, int ifadd, duf_levinfo_t * pli, dev_t dev_id, ino_t dir_ino,
                      unsigned long long parentid, int need_id, int *pchanges, int *pr )
{
  unsigned long long dirid = 0;
  int r = 0;

  DEBUG_START(  );
  /* unsigned char c1 = ( unsigned char ) ( dename ? *dename : 0 ); */
  if ( dename /* && dev_id && dir_ino */  )
  {
    int changes = 0;

    if ( ifadd && !duf_config->cli.disable.insert )
    {
      static const char *sql =
            "INSERT OR IGNORE INTO " DUF_DBPREF "paths ( dev, inode, dirname, parentid) VALUES (:dev, :inode, :dirname, :parentid )";
      if ( pdi )
      {
        DUF_SQL_START_STMT( pdi, insert_path, sql, r, pstmt );
        DUF_TRACE( insert, 0, "S:%s (%lu,%lu,'%s',%llu)", sql, dev_id, dir_ino, dename, parentid );
        /* DUF_ERROR( "insert_path_index:%d", insert_path_index ); */
        DUF_SQL_BIND_LL( dev, dev_id, r, pstmt );
        DUF_SQL_BIND_LL( inode, dir_ino, r, pstmt );
        DUF_SQL_BIND_S( dirname, dename, r, pstmt );
        DUF_SQL_BIND_LL( parentid, parentid, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );

        DUF_SQL_END_STMT( r, pstmt );
      }
      else
      {
        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_BIND_LL( dev, dev_id, r, pstmt );
        DUF_SQL_BIND_LL( inode, dir_ino, r, pstmt );
        DUF_SQL_BIND_S( dirname, dename, r, pstmt );
        DUF_SQL_BIND_LL( parentid, parentid, r, pstmt );
        DUF_SQL_STEP( r, pstmt );

        DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );

        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
    }
    /* sql = NULL; */
    DUF_TRACE( current, 0, "<changes> : %d", changes );
    /* if ( r == DUF_SQL_CONSTRAINT ) */
    if ( need_id )
    {
      if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
      {
        /* duf_sqlite_stmt_t *pstmt = NULL; */
        const char *sql = "SELECT " DUF_DBPREF "paths.id AS dirid, " DUF_DBPREF "paths.dirname " /*      */
              ", tf.numfiles AS nfiles, td.numdirs AS ndirs " /*      */
              " FROM " DUF_DBPREF "paths LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.pathid=" DUF_DBPREF "paths.id) " /*      */
              " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.pathid=" DUF_DBPREF "paths.id) " /*      */
              " WHERE " DUF_DBPREF "paths.parentid=:dirid AND dirname=:dirname";

        if ( pdi )
        {
          DUF_SQL_START_STMT( pdi, select_path, sql, r, pstmt );
          DUF_TRACE( select, 0, "S:%s (%llu,'%s')", sql, parentid, dename );
          /* DUF_ERROR( "select_path_index:%d", select_path_index ); */
          DUF_SQL_BIND_LL( dirid, parentid, r, pstmt );
          DUF_SQL_BIND_S( dirname, dename, r, pstmt );
          DUF_SQL_STEP( r, pstmt );
          if ( r == DUF_SQL_ROW )
          {
            DUF_TRACE( current, 0, "<selected>" );
            dirid = duf_sql_column_long_long( pstmt, 0 );
            if ( need_id && !dirid )
            {
              DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
            }
            else
            {
              DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", dename, dirid, parentid );
            }
            assert( !need_id || dirid );
            if ( pli )
            {
              pli->dirid = dirid;
              /* pli->itemname = mas_strdup( duf_sql_column_string( pstmt, 1 ) ); */
              pli->numfile = duf_sql_column_long_long( pstmt, 2 );
              pli->numdir = duf_sql_column_long_long( pstmt, 3 );
            }
            r = 0;
          }
          else
          {
            DUF_TEST_R( r );
            DUF_TRACE( current, 0, "<NOT selected> (%d)", r );
          }
          DUF_SQL_END_STMT( r, pstmt );
        }
        else
        {
          DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
          DUF_SQL_BIND_LL( dirid, parentid, r, pstmt );
          DUF_SQL_BIND_S( dirname, dename, r, pstmt );
          DUF_SQL_STEP( r, pstmt );

          if ( r == DUF_SQL_ROW )
          {
            DUF_TRACE( current, 0, "<selected>" );
            dirid = duf_sql_column_long_long( pstmt, 0 );
            if ( need_id && !dirid )
            {
              DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
            }
            else
            {
              DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", dename, dirid, parentid );
            }
            assert( !need_id || dirid );
            if ( pli )
            {
              pli->dirid = dirid;
              pli->itemname = mas_strdup( duf_sql_column_string( pstmt, 1 ) );
              pli->numfile = duf_sql_column_long_long( pstmt, 2 );
              pli->numdir = duf_sql_column_long_long( pstmt, 3 );
            }
            r = 0;
          }
          else
          {
            DUF_TEST_R( r );
            DUF_TRACE( current, 0, "<NOT selected> (%d)", r );
          }
          DUF_SQL_END_STMT_NOPDI( r, pstmt );
        }
        DUF_TEST_R( r );
        DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, dename );
      }
      else if ( !r /* assume SQLITE_OK */  && changes )
      {
        dirid = duf_sql_last_insert_rowid(  );
        if ( need_id && !dirid )
        {
          DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
        }
        else
        {
          DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", dename, dirid, parentid );
        }
        assert( !need_id || dirid );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, dename );
      }
      DUF_TEST_R( r );
    }
    if ( pchanges )
      *pchanges = changes;
  }
  else
  {
    DUF_ERROR( "Wrong data dename(%s) OR dev_id(%lu) OR dir_ino(%lu)", dename, dev_id, dir_ino );
    r = DUF_ERROR_DATA;
  }
  if ( pr )
    *pr = r;
  if ( need_id && !dirid )
  {
    DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
  }
  DUF_TEST_R( r );
  /* assert( !need_id || dirid ); */
  DEBUG_ENDULL( dirid );
  return dirid;
}

int
duf_real_path2db( duf_depthinfo_t * pdi, const char *rpath, int ifadd )
{
  unsigned long long parentid = 0;
  char *real_path = mas_strdup( rpath );
  int r = 0;

  DUF_TRACE( explain, 0, "real_path: ≪%s≫", real_path );

  {
    int upfd = 0;
    char *dir = real_path;

    DUF_TRACE( path, 0, "%s PATHID for %s", ifadd ? "ADD" : "GET", real_path );
    while ( r >= 0 && dir && *dir )
    {
      char *edir = dir;
      const char *insdir;
      struct stat st_dir;

      DUF_TRACE( path, 0, "prepare [%s] under %llu", dir, parentid );
      while ( edir && *edir && *edir != '/' )
        edir++;
      if ( edir && *edir == '/' )
        *edir++ = 0;
      DUF_TRACE( path, 0, "prepared [%s] up to [%s] fd:%d", dir, edir, upfd );

      insdir = dir;
      if ( pdi )
      {
        if ( r >= 0 )
          r = duf_levinfo_down( pdi, 0, insdir, 0 /* ndirs */ , 0 /* nfiles */ , 0 /* is_leaf */  );
        if ( r >= 0 )
          r = duf_levinfo_openat_dh( pdi );
      }
      if ( pdi )
      {
        upfd = duf_levinfo_dfd( pdi );
        DUF_TRACE( explain, 4, "already opened (at) ≪%s≫ upfd:%d", insdir, upfd );
      }
      else                      /* never used !!?? */
      {
        const char *opendir;
        int fd = 0;

        if ( *insdir )
          opendir = insdir;
        else
          opendir = "/";
        fd = upfd;
        r = fstatat( fd, opendir, &st_dir, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
        upfd = openat( fd, opendir, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
        DUF_TRACE( explain, 0, "opened (at) ≪%s≫ upfd:%d", insdir, upfd );
        if ( fd )
          close( fd );
        fd = 0;
      }
      if ( r )
      {
        DUF_ERROR( "No such entry %s\n", real_path );
        break;
      }
      else
      {
        int changes;

        changes = 0;
        DUF_TRACE( path, 0, "to insert [%s]", insdir ? insdir : "/" );
        parentid = duf_insert_path_uni2( pdi, insdir, ifadd, duf_levinfo( pdi ), st_dir.st_dev, st_dir.st_ino, parentid, 1 /*need_id */ ,
                                         &changes, &r );
        if ( changes )
        {
          DUF_TRACE( explain, 0, "added id: %llu for ≪%s≫", parentid, insdir );
        }
        else
        {
          DUF_TRACE( explain, 1, "already in db id: %llu for ≪%s≫", parentid, insdir );
        }
        /* assert( parentid ); */

        if ( pdi )
          duf_levinfo_set_dirid( pdi, parentid );
        DUF_TRACE( path, 0, "inserted [%s] AS %llu", insdir, parentid );
        DUF_TRACE( path, 0, "id %llu for insdir ≪%s≫", parentid, insdir );
      }
      dir = edir;
      DUF_TRACE( path, 0, "next [%s] under %llu", dir, parentid );
    }
    if ( !pdi && upfd )
      close( upfd );
    upfd = 0;
  }
  duf_pdi_set_topdepth( pdi );
  mas_free( real_path );
  if ( r >= 0 && !parentid )
    r = DUF_ERROR_NOT_IN_DB;
  DUF_TEST_R( r );

  return r;
}

unsigned long long
duf_insert_path_uni( duf_depthinfo_t * pdi, const char *dename, dev_t dev_id, ino_t dir_ino, unsigned long long parentid, int need_id,
                     int *pchanges, int *pr )
{
  return duf_insert_path_uni2( pdi, dename, 1, ( duf_levinfo_t * ) NULL, dev_id, dir_ino, parentid, need_id, pchanges, pr );
}
