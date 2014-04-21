#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql_def.h"
#include "duf_sql.h"

#include "duf_utils.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_group.h"
/* ###################################################################### */




/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sel_cb_insert_group( duf_record_t * precord, va_list args, void *sel_cb_udata,
                         duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                         const duf_dirhandle_t * pdhu )
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
  r = duf_sql_c( "INSERT INTO duf_group (name,now) values ('%s',datetime())", DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, name );
  if ( r == DUF_SQL_CONSTRAINT )
  {
    r = duf_sql_select( duf_sel_cb_insert_group, &id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /* sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT id as groupid FROM duf_group WHERE name='%s'", name );
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
duf_sql_insert_path_group( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                           duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
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
  r = duf_sql_c( "INSERT INTO duf_path_group (groupid,pathid,now) " " VALUES ('%lld','%lld',datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, groupid, pathid );
  if ( r == DUF_SQL_CONSTRAINT )
    r = duf_sql_select( duf_sql_insert_path_group, &id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT id as groupid FROM duf_path_group " " WHERE groupid='%lld' and  pathid='%lld'", groupid, pathid );
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
duf_sql_group_to_groupid( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                          duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
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
                  ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                  "SELECT id as groupid FROM duf_group WHERE name='%s'", group );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, groupid );
  return groupid;
}
