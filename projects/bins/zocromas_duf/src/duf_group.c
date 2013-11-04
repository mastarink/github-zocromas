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
#include "duf_group.h"
/* ###################################################################### */




/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_insert_group( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                      void *str_cb_udata )
{
  unsigned long long *pid;

  pid = ( unsigned long long * ) sel_cb_udata;
  *pid = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_group( const char *name )
{
  unsigned long long id = -1;
  int r = 0;

  r = duf_sql_c( DUF_TRACE_NO,"INSERT INTO duf_group (name,now) values ('%s',datetime())", DUF_CONSTRAINT_IGNORE_YES, name );
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_group, &id, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO, "SELECT id FROM duf_group WHERE name='%s'",
                        name );
  }
  else if ( !r /* assume SQLITE_OK */  )
    id = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );

  return id;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_insert_path_group( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                           void *str_cb_udata )
{
  unsigned long long *pid;

  pid = ( unsigned long long * ) sel_cb_udata;
  *pid = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_path_group( unsigned long long groupid, unsigned long long pathid )
{
  unsigned long long id = -1;
  int r = 0;

  r = duf_sql_c( DUF_TRACE_NO,"INSERT INTO duf_path_group (groupid,pathid,now) " " VALUES ('%lld','%lld',datetime())", DUF_CONSTRAINT_IGNORE_YES,
                 groupid, pathid );
  if ( r == duf_constraint )
    r = duf_sql_select( duf_sql_insert_path_group, &id, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT id FROM duf_path_group " " WHERE groupid='%lld' and  pathid='%lld'", groupid, pathid );
  else if ( !r /* assume SQLITE_OK */  )
    id = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );
  return id;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_group_to_groupid( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                          void *str_cb_udata )
{
  unsigned long long *pgroupid;

  pgroupid = ( unsigned long long * ) sel_cb_udata;
  ( *pgroupid ) = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_group_to_groupid( const char *group )
{
  unsigned long long groupid = 0;

  /* r = */
  duf_sql_select( duf_sql_group_to_groupid, &groupid, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                  "SELECT id FROM duf_group WHERE name='%s'", group );
  return groupid;
}
