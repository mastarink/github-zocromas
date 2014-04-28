#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* dirname, basename */
#include <libgen.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"

#include "duf_sql_def.h"
#include "duf_sql_field.h"
#include "duf_sql.h"

#include "duf_dbg.h"

#include "duf_group.h"

/* ###################################################################### */
#include "duf_add.h"
/* ###################################################################### */



/* insert path into db; return id */
unsigned long long
duf_insert_path_uni( const char *dename, dev_t dev_id, ino_t dir_ino, unsigned long long parentid, int need_id, int *pr )
{
  unsigned long long pathid = 0;
  int r;

  /* unsigned char c1 = ( unsigned char ) ( dename ? *dename : 0 ); */

  if ( dename && dev_id && dir_ino )
  {
    int changes = 0;

    {
      const char *qdirname;
      char *qbase_name;

      duf_dbgfunc( DBG_START, __func__, __LINE__ );
      qbase_name = duf_single_quotes_2( dename );
      qdirname = qbase_name ? qbase_name : dename;
      DUF_TRACE( current, 0, "insert [%s] @ %llu", qdirname, parentid );
      {
        r = duf_sql( "INSERT OR IGNORE INTO duf_paths (dev, inode, dirname, parentid) " " VALUES ('%lu','%lu','%s','%llu')", &changes,
                     dev_id, dir_ino, qdirname, parentid );
      }
      DUF_TRACE( collect, 1, "INSERT: r=%d; dev_id=%lu; dir_ino=%lu; dirname=%s; parentid=%llu", r, dev_id, dir_ino, qdirname, parentid );

      mas_free( qbase_name );
    }
    /* sql = NULL; */
    DUF_TRACE( current, 0, "<changes> : %d", changes );
    /* if ( r == DUF_SQL_CONSTRAINT ) */
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        /* unsigned long long pathid1; */
        duf_scan_callbacks_t sccb = {.fieldset = "pathid" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &pathid, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            &sccb, ( const duf_dirhandle_t * ) NULL,
                            "SELECT id AS pathid " " FROM duf_paths " " WHERE dev='%lu' AND inode='%lu'", dev_id, dir_ino );
        DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) pathid=%llu:'%s'", pathid, dename );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id )
      {
        pathid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) pathid=%llu:'%s'", pathid, dename );
      }
    }
    else
      DUF_ERROR( "insert path [%s] %d", dename, r );
  }
  else
  {
    DUF_ERROR( "Wrong data" );
    r = DUF_ERROR_DATA;
  }
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}

/* insert full path into db; return id 
 * /a/b/c/d/e/f
 * '/a/b/c/d/e/f' -> db
 * */
unsigned long long
duf_add_real_path_uni( const char *real_path, const char *group, int up, int need_id, int *pr )
{
  int r = 0;
  unsigned long long pathid = 0;
  char *dir_name;
  const char *base_name = NULL;
  char *rpath = mas_strdup( real_path );

  duf_dbgfunc( DBG_START, __func__, __LINE__ );


  base_name = basename( rpath );
  dir_name = dirname( rpath );
  if ( !base_name || !*base_name )
  {
    DUF_TRACE( current, 0, "real_path:[%s] / [%s]", real_path, rpath );
    base_name = real_path + 1;
    dir_name = NULL;
    DUF_TRACE( current, 0, "real_path:[%s] / [%s] => [%s]", real_path, rpath, base_name );
  }
  if ( base_name && *base_name )
  {
    struct stat st_dir;

    /* stat needless here ? */
    r = stat( real_path, &st_dir );
    if ( r )
    {
/* no such entry */
      DUF_ERROR( "No such entry %s\n", real_path );
    }
    else
    {
      unsigned long long parentid = 0;

      /* 
       * insert path into db (recursively); return id 
       * /a/b/c/d/e/f
       * '/a/b/c/d/e' -> db
       * */
      if ( ( dir_name && *dir_name ) && up )
      {
        DUF_TRACE( current, 0, "update parent %s", dir_name );
        parentid = duf_add_real_path_uni( dir_name, NULL /* group */ , DUF_TRUE /* up */ , 1 /*need_id */ , &r );
      }
      DUF_TRACE( current, 0, "updated parent %s AS %llu", dir_name, parentid );

      /* 
       * insert name into db; return id 
       * /a/b/c/d/e/f
       * 'f' -> db
       * */
      if ( r >= 0 )
      {
        char *name = mas_strdup( base_name );

        DUF_TRACE( current, 0, "to insert [%s]", base_name );
        pathid = duf_insert_path_uni( name, st_dir.st_dev, st_dir.st_ino, parentid, ( need_id || group ), &r );
        DUF_TRACE( current, 0, "inserted [%s] AS %llu", base_name, pathid );
        mas_free( name );
      }
  DUF_TRACE( action, 0, "#%llu : added real path %s",  pathid, real_path);

      if ( r >= 0 && group )
        duf_pathid_group( group, pathid, +1 );
    }
  }
  mas_free( rpath );
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}

unsigned long long
duf_add_path_uni( const char *path, const char *group, int need_id, int *pr )
{
  int r = 0;
  unsigned long long pathid = 0;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  {
    char *real_path = NULL;

    {
      char *rp = realpath( path, NULL );

      if ( rp )
      {
        real_path = mas_strdup( rp );
        free( rp );
      }
    }
    DUF_TRACE( action, 0, "real_path:%s", real_path );
    if (  /* strlen( real_path ) > 1 && */ !( real_path && *real_path == '/' && real_path[1] == 0 ) )
    {
      DUF_TRACE( current, 0, "Update parent %s", real_path );
      pathid = duf_add_real_path_uni( real_path, group, DUF_TRUE, need_id, &r );
      DUF_TRACE( current, 0, "Updated parent %s AS %llu", real_path, pathid );
    }
    /* DUF_TRACE( action, 0, "Added path %s as %llu;  group: %s", path, pathid, group ); */
    mas_free( real_path );
  }
  if ( pr )
    *pr = r;
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return pathid;
}
