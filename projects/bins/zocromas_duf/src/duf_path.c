#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"
#include "duf_config.h"

#include "duf_sql.h"
#include "duf_insert.h"
#include "duf_group.h"
#include "duf_utils.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_path.h"
/* ###################################################################### */


char *
duf_join_path( const char *path, const char *fname )
{
  char *fpath = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( fname )
  {
    if ( path )
      fpath = mas_strdup( path );
    if ( *fname != '/' && !( fpath && *fpath == '/' && fpath[1] == 0 ) )
      fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, fname );
  }
  duf_dbgfunc( DBG_ENDS, __func__, __LINE__, fpath );
  return fpath;
}

typedef struct
{
  char *name;
  unsigned long long parentid;
} pathid_to_path_udata_t;

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_pathid_to_path( duf_record_t * precord, va_list args, void *sel_cb_udata,
                        duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * cb )
{
  pathid_to_path_udata_t *pud;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pud = ( pathid_to_path_udata_t * ) sel_cb_udata;
  if ( precord->nrow == 0 )
  {
    if ( duf_config->cli.dbg.verbose > 1 )
    {
      fprintf( stderr, "%s:%s='%s' -- r[%d]='%s' / %llu ; '%s'\n", __func__, precord->pnames[0], precord->presult[0],
               duf_sql_pos_by_name( "parentid", precord, 0 ), duf_sql_str_by_name( "parentid", precord, 0 ),
               duf_sql_ull_by_name( "parentid", precord, 0 ), duf_sql_str_by_name( "dirname", precord, 0 ) );
    }
    pud->parentid = duf_sql_ull_by_name( "parentid", precord, 0 );
    pud->name = mas_strdup( duf_sql_str_by_name( "dirname", precord, 0 ) );
    /* pud->parentid = strtoll( precord->presult[0], NULL, 10 ); */
    /* pud->name = mas_strdup( precord->presult[1] );            */
  }
  /* fprintf( stderr, "pud a:%d nrow:%d\n", precord->presult ? 1 : 0, nrow ); */
  /* fprintf( stderr, "pud b: %s :: %s\n", precord->presult[0], precord->presult[1] ); */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

char *
duf_pathid_to_path( unsigned long long pathid )
{
  int r;
  char *path = NULL;
  pathid_to_path_udata_t pathdef = {.name = NULL,.parentid = 0 };
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_select( duf_sql_pathid_to_path, &pathdef, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                      ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                      "SELECT parentid, dirname " " FROM duf_paths WHERE id=%llu", pathid );
  if ( r >= 0 && pathdef.name && pathdef.parentid >= 0 )
  {
    char *parent = NULL;

    /* fprintf( stderr, "parentid:%lld - %s\n", pathdef.parentid, pathdef.name ); */
    parent = duf_pathid_to_path( pathdef.parentid );
    path = duf_join_path( parent, pathdef.name );
    if ( parent )
      mas_free( parent );
  }
  if ( pathdef.name )
    mas_free( pathdef.name );
  duf_dbgfunc( DBG_ENDS, __func__, __LINE__, path );
  return path;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
int
duf_sql_path_to_pathid( duf_record_t * precord, va_list args, void *sel_cb_udata,
                        duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * cb )
{
  unsigned long long *ppathid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  ppathid = ( unsigned long long * ) sel_cb_udata;
  if ( duf_config->cli.dbg.verbose > 1 )
  {
    fprintf( stderr, "%s:%s='%s' -- r[%d]='%s' / %llu\n", __func__, precord->pnames[0], precord->presult[0],
             ( duf_sql_pos_by_name( "pathid", precord, 0 ) ), ( duf_sql_str_by_name( "pathid", precord, 0 ) ),
             ( duf_sql_ull_by_name( "pathid", precord, 0 ) ) );
  }
  ( *ppathid ) = duf_sql_ull_by_name( "pathid", precord, 0 ); /* strtoll( precord->presult[0], NULL, 10 ); */
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, ( *ppathid ) );
  return 0;
}

unsigned long long
duf_realpath_to_pathid_x( char *rpath, unsigned long long *pprevpathid, char **notfound )
{
  int r = 0;
  unsigned long long pathid_new = 0;
  unsigned long long prevpathid = 0;
  unsigned long long pathid = 0;
  char *bd = NULL;
  char *qname = NULL;
  char *cpath = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  bd = cpath = mas_strdup( rpath );

  while ( bd && *bd && *bd == '/' )
    bd++;
  while ( bd && *bd )
  {
    char *ed;

    prevpathid = pathid;
    qname = duf_single_quotes_2( bd );
    ed = bd;
    /* find next '/' */
    while ( ed && *ed && *ed != '/' )
      ed++;
    /* find next name */
    while ( ed && *ed && *ed == '/' )
      *ed++ = 0;
    pathid_new = 0;
    r = duf_sql_select( duf_sql_path_to_pathid, &pathid_new, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT id as pathid " " FROM duf_paths WHERE parentid='%llu' and dirname='%s'", pathid, qname ? qname : bd );
    if ( r < 0 )
      break;
    if ( !pathid_new )
    {
      if ( notfound && !*notfound )
        *notfound = mas_strdup( bd );
      break;
    }
    if ( 0 )
    {
      char *tpath = duf_pathid_to_path( pathid_new );

      fprintf( stderr, "%lld => %lld : %s : ed='%s'\n", pathid, pathid_new, tpath, ed );
      mas_free( tpath );
    }
    pathid = pathid_new;
    bd = ed;
    mas_free( qname );
    qname = NULL;
  }
  if ( pprevpathid )
    *pprevpathid = prevpathid;
  mas_free( cpath );
  cpath = NULL;
  mas_free( qname );
  qname = NULL;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid_new );
  return pathid_new;
}

unsigned long long
duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound )
{
  unsigned long long pathid_new = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( path )
  {
    char *rpath;
    char *real_path;

    rpath = mas_malloc( PATH_MAX );
    if ( rpath )
    {
      {
        ( void ) realpath( path, rpath );
        real_path = mas_strdup( rpath );
        mas_free( rpath );
      }
      pathid_new = duf_realpath_to_pathid_x( real_path, pprevpathid, notfound );
      mas_free( real_path );
    }
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid_new );
  return pathid_new;
}

unsigned long long
duf_path_to_pathid( const char *path )
{
  unsigned long long pathid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pathid = duf_path_to_pathid_x( path, NULL, NULL );

  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_delete_path_by_groupid( duf_record_t * precord, va_list args,
                                void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi,
                                duf_scan_callbacks_t * cb )
{
  int r = 0;
  unsigned long long pathid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config->cli.dbg.verbose > 1 )
  {
    fprintf( stderr, "%s='%s' -- r[%d]='%s' / %llu\n", precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord, 0 ),
             duf_sql_str_by_name( "id", precord, 0 ), duf_sql_ull_by_name( "id", precord, 0 ) );
  }
  pathid = strtoll( precord->presult[0], NULL, 10 );
  r = duf_sql( "DELETE FROM duf_path_group WHERE id='%lld'", pathid );
  /* fprintf( stderr, ">> DELETE FROM duf_path_group WHERE id='%lld' -- %d\n", pathid, r ); */
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_delete_path_by_groupid( unsigned long long groupid, unsigned long long pathid )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_select( duf_sql_delete_path_by_groupid, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF,
                      ( duf_dirinfo_t * ) NULL, ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                      "SELECT duf_path_group.id as groupid " " FROM duf_paths "
                      " LEFT JOIN duf_path_group ON (duf_paths.id=duf_path_group.pathid) "
                      " WHERE duf_path_group.groupid='%lld' AND duf_paths.id='%lld'", groupid, pathid );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}


/* 
 * duf_scan_callback_file_t:
 * */
static int
duf_sql_scan_print_path( unsigned long long pathid, unsigned long long filenameid,
                         const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  char *path;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  path = duf_pathid_to_path( pathid );
  printf( "%7llu: %s\n", pathid, path );
  mas_free( path );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_scan_paths( duf_record_t * precord, va_list args, void *sel_cb_udata,
                    duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  unsigned long long pathid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config->cli.dbg.verbose > 1 )
  {
    fprintf( stderr, "%s='%s' -- r[%d]='%s' / %llu\n", precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord, 0 ),
             duf_sql_str_by_name( "id", precord, 0 ), duf_sql_ull_by_name( "id", precord, 0 ) );
  }
  pathid = strtoll( precord->presult[0], NULL, 10 );
  {
    char *path;

    path = duf_pathid_to_path( pathid );
    if ( str_cb )
    {
/* 
 * duf_scan_callback_file_t:
 * */
      ( *str_cb ) ( pathid, 0 /* filenameid */ , NULL /* name */ , STR_CB_UDATA_DEF,
                    ( duf_dirinfo_t * ) NULL, sccb, precord );
    }
    mas_free( path );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_scan_paths( duf_scan_callback_file_t str_cb, void *str_cb_udata, const char *groupname )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( groupname )
  {
    if ( *groupname == '-' )
    {
      groupname++;
      r = duf_sql_select( duf_sql_scan_paths, str_cb_udata, str_cb, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                          ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                          "SELECT duf_paths.id as pathid FROM duf_paths "
                          " LEFT JOIN duf_path_group ON(duf_paths.id=duf_path_group.pathid) "
                          " LEFT JOIN duf_group ON (duf_path_group.groupid=duf_group.id) " " WHERE ( duf_group.name IS NULL OR "
                          " duf_group.name!='%s' ) " " ORDER BY duf_paths.id", groupname );
    }
    else
    {
      if ( *groupname == '+' )
        groupname++;
      r = duf_sql_select( duf_sql_scan_paths, str_cb_udata, str_cb, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                          ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                          "SELECT duf_paths.id as pathid FROM duf_paths "
                          " LEFT JOIN duf_path_group ON(duf_paths.id=duf_path_group.pathid) "
                          " LEFT JOIN duf_group ON (duf_path_group.groupid=duf_group.id) " " WHERE ( duf_group.name='%s' ) "
                          " ORDER BY duf_paths.id", groupname );
    }
  }
  else
    r = duf_sql_select( duf_sql_scan_paths, str_cb_udata, str_cb, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT  duf_paths.id as pathid FROM duf_paths " " ORDER BY duf_paths.id" );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_print_paths( const char *groupname )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  fprintf( stderr, "Group %s\n", groupname );
  r = duf_scan_paths( duf_sql_scan_print_path, STR_CB_UDATA_DEF, groupname );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

void
duf_pathid_group( const char *group, unsigned long long pathid, int add_remove )
{
  unsigned long long groupid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( group )
  {
    if ( add_remove > 0 )
    {
      groupid = duf_insert_group( group );
      /* gpid = */ duf_insert_path_group( groupid, pathid );
    }
    else if ( add_remove < 0 )
    {
      groupid = duf_group_to_groupid( group );
      if ( groupid )
      {
        fprintf( stderr, "G [%llu] group:%s; pathid:%llu {add_remove:%d}\n", groupid, group, pathid, add_remove );
        duf_delete_path_by_groupid( groupid, pathid );
      }
    }
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
}

void
duf_paths_group( const char *group, const char *path, int add_remove )
{
  unsigned long long pathid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pathid = duf_path_to_pathid( path );
  if ( pathid )
    duf_pathid_group( group, pathid, add_remove );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_insert_path( duf_record_t * precord, va_list args, void *sel_cb_udata,
                     duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * cb )
{
  unsigned long long *pdir_id;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* fprintf( stderr, "Constraint selected %d\n", nrow1 ); */
  pdir_id = ( unsigned long long * ) sel_cb_udata;
  if ( precord->nrow == 0 )
  {
    if ( duf_config->cli.dbg.verbose > 1 )
    {
      fprintf( stderr, "%s='%s' -- r[%d]='%s' / %llu\n", precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord, 0 ),
               duf_sql_str_by_name( "id", precord, 0 ), duf_sql_ull_by_name( "id", precord, 0 ) );
    }
    *pdir_id = strtoll( precord->presult[0], NULL, 10 );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* insert path into db; return id */
unsigned long long
duf_insert_path( const char *base_name, const struct stat *pst_dir, unsigned long long parentid )
{
  unsigned long long dir_id = 0;
  int r;

  char *qbase_name;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* fprintf( stderr, "Insert path %s\n", base_name ); */
  qbase_name = duf_single_quotes_2( base_name );
  r = duf_sql_c( "INSERT INTO duf_paths (dev, inode, dirname, parentid, ucnt, now) " " VALUES ('%lu','%lu','%s','%lu',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, pst_dir->st_dev, pst_dir->st_ino, qbase_name ? qbase_name : base_name, parentid );
  mas_free( qbase_name );
  /* sql = NULL; */
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_path, &dir_id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT id as pathid " " FROM duf_paths " " WHERE dev='%lu' and inode='%lu'", pst_dir->st_dev, pst_dir->st_ino );
  }
  else if ( !r /* assume SQLITE_OK */  )
  {
    dir_id = duf_last_insert_rowid(  );
    /* fprintf( stderr, "Inserted dir_id: %lld\n", dir_id ); */
  }
  else
    fprintf( stderr, "error duf_insert_path %d\n", r );
  /* fprintf( stderr, "Insert path %s done\n", base_name ); */
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, dir_id );
  return dir_id;
}
