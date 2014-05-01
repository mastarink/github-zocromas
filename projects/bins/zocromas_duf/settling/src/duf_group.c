#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_sql_def.h"
#include "duf_sql_field.h"
#include "duf_sql.h"


#include "duf_dbg.h"

#include "duf_path.h"

/* ###################################################################### */
#include "duf_group.h"
/* ###################################################################### */


#ifdef DUF_COMPILE_EXPIRED

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sel_cb_insert_group( duf_record_t * precord, va_list args, void *sel_cb_udata,
                         duf_scan_callback_file_t str_cb_notused, void *str_cb_udata_notused, duf_depthinfo_t * pdi,
                         duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  unsigned long long *pid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pid = ( unsigned long long * ) sel_cb_udata;
  *pid = strtoll( precord->presult[0], NULL, 10 );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

unsigned long long
duf_insert_group( const char *name )
{
  unsigned long long id = -1;
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_c( "INSERT INTO duf_group (name) VALUES ('%s')", DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, name );
  if ( r == DUF_SQL_CONSTRAINT )
  {
    r = duf_sql_select( duf_sel_cb_insert_group, &id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /* sccb *//*, ( const duf_dirhandle_t * ) NULL off */ ,
                        "SELECT id AS groupid FROM duf_group WHERE name='%s'", name );
  }
  else if ( !r /* assume SQLITE_OK */  )
    id = duf_sql_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );

  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, id );
  return id;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_insert_path_group( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb_notused,
                           void *str_cb_udata_notused, duf_depthinfo_t * pdi,
                           duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  unsigned long long *pid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pid = ( unsigned long long * ) sel_cb_udata;
  *pid = strtoll( precord->presult[0], NULL, 10 );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

unsigned long long
duf_insert_path_group( unsigned long long groupid, unsigned long long pathid )
{
  unsigned long long id = -1;
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_c( "INSERT INTO duf_path_group (groupid,pathid) VALUES ('%lld','%lld')",
                 DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, groupid, pathid );
  if ( r == DUF_SQL_CONSTRAINT )
    r = duf_sql_select( duf_sql_insert_path_group, &id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ /* , ( const duf_dirhandle_t * ) NULL off */,
                        "SELECT id AS groupid FROM duf_path_group WHERE groupid='%lld' AND  pathid='%lld'", groupid, pathid );
  else if ( !r /* assume SQLITE_OK */  )
    id = duf_sql_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, id );
  return id;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_group_to_groupid( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb_notused,
                          void *str_cb_udata_notused, duf_depthinfo_t * pdi,
                          duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  unsigned long long *pgroupid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pgroupid = ( unsigned long long * ) sel_cb_udata;
  ( *pgroupid ) = strtoll( precord->presult[0], NULL, 10 );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

unsigned long long
duf_group_to_groupid( const char *group )
{
  unsigned long long groupid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* r = */
  duf_sql_select( duf_sql_group_to_groupid, &groupid, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                  ( duf_scan_callbacks_t * ) NULL /*  sccb */ /*, ( const duf_dirhandle_t * ) NULL off */,
                  "SELECT id AS groupid FROM duf_group WHERE name='%s'", group );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, groupid );
  return groupid;
}



/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sel_cb_delete_path_by_groupid( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb_notused,
                                   void *str_cb_udata_notused, duf_depthinfo_t * pdi,
                                   duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  int r = 0;
  unsigned long long pathid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config->cli.dbg.verbose > 1 )
  {
    fprintf( stderr, "%s='%s' -- r[%d]='%s' / %llu\n", precord->pnames[0], precord->presult[0],
             ( __duf_sql_pos_by_name( "id", precord, NULL, 0 ) ), ( __duf_sql_str_by_name( "id", precord, NULL, 0 ) ),
             ( __duf_sql_ull_by_name( "id", precord, NULL, 0 ) ) );
  }
  pathid = strtoll( precord->presult[0], NULL, 10 );

  r = duf_sql( "DELETE FROM duf_path_group WHERE id='%lld'", ( int * ) NULL, pathid );
  /* fprintf( stderr, ">> DELETE FROM duf_path_group WHERE id='%lld' -- %d\n", pathid, r ); */
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_delete_path_by_groupid( unsigned long long groupid, unsigned long long pathid )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_select( duf_sel_cb_delete_path_by_groupid, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF,
                      ( duf_depthinfo_t * ) NULL, ( duf_scan_callbacks_t * ) NULL /*  sccb */ /*, ( const duf_dirhandle_t * ) NULL off*/,
                      "SELECT duf_path_group.id AS groupid " " FROM duf_paths "
                      " LEFT JOIN duf_path_group ON (duf_paths.id=duf_path_group.pathid) "
                      " WHERE duf_path_group.groupid='%lld' AND duf_paths.id='%lld'", groupid, pathid );
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
  int r = 0;
  unsigned long long pathid;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pathid = duf_path_to_pathid( path, ( duf_depthinfo_t * ) NULL, &r );
  if ( pathid )
    duf_pathid_group( group, pathid, add_remove );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
}
#endif
