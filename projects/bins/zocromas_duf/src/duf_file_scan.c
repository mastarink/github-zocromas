#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_types.h"
#include "duf_config.h"

#include "duf_sql.h"

#include "duf_file.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
/* #include "duf_file.h" */

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_scan.h"
/* ###################################################################### */



/* duf_sql_call_str_cb:
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
int
duf_sql_call_str_cb( duf_record_t * precord, va_list args, void *sel_cb_udata,
                     duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( 1 || str_cb )
  {
    unsigned long long pathid;
    unsigned long long filenameid;

/*
 * 1. get pathid     from db record
 * 2. get filenameid from db record (if possible)
 * */
    pathid = duf_sql_ull_by_name( "pathid", precord, 0 );
    filenameid = duf_sql_ull_by_name( "filenameid", precord, 1 );

    if ( duf_config->verbose > 1 )
    {
      /* printf( "%s:%s='%s' -- r[%d]='%s' / %llu\n", __func__, precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord ), */
      /*         duf_sql_str_by_name( "parentid", precord ), duf_sql_ull_by_name( "parentid", precord ) );                                     */
      fprintf( stderr, "        >>>>> %s::", __func__ );
      for ( int i = 0; i < precord->ncolumns; i++ )
      {
        fprintf( stderr, "%s;", precord->pnames[i] );
      }
      fprintf( stderr, " [ pathid=%llu ]\n", pathid );
    }
    if ( duf_config->scan_trace )
      fprintf( stderr, "[SCAN ] %20s: \n", __func__ );

    duf_dbgfunc( DBG_STEPULL, __func__, __LINE__, pathid );
    if ( pathid )
    {
      const char *name;

      /* name = precord->presult[2]; */
/*
 * 3. get filename from db record (if possible) 
 * */
      name = duf_sql_str_by_name( "filename", precord, 1 );
      duf_dbgfunc( DBG_STEPI2S, __func__, __LINE__, pathid, pathid, name );

      {
        /* int r = 0; */
        /* struct stat st_dir; */
        /* char *path; */

        /* path = duf_pathid_to_path( pathid ); */

        /* r = stat( path, &st_dir ); */

        if ( str_cb )
        {
          if ( duf_config->verbose > 1 )
          {
            const char *name2 = NULL;

            if ( str_cb == duf_sql_uni_scan_dir )
              name2 = "duf_sql_uni_scan_dir";

            fprintf( stderr, "call str_cb> %p:%s\n", ( void * ) ( unsigned long long ) str_cb, name2 ? name2 : "-" );
          }
/*
 * 4. call function str_cb
 * */
          ( *str_cb ) ( pathid, filenameid, name, str_cb_udata, pdi, sccb, precord );
        }
        else if ( duf_config->scan_trace > 1 )
          fprintf( stderr, "[SCAN ] %20s: No str_cb for name='%s'\n", __func__, name );
        /* mas_free( path ); */
      }
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    }
    else if ( duf_config->verbose > 1 )
      fprintf( stderr, " ?????????????????\n" );
  }
  else
  {
    if ( duf_config->scan_trace > 1 )
      fprintf( stderr, "[SCAN ] %20s: No str_cb\n", __func__ );
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* 
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
int
duf_scan_vitems_sql( duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                     va_list args )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* duf_sql_call_str_cb:
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
  if ( duf_config->scan_trace )
    fprintf( stderr, "[SCAN ] %20s: L%u >duf_sql_vselect with sel_cb=duf_sql_call_str_cb(%p); str_cb=%p\n", __func__, pdi->level,
             ( void * ) ( unsigned long long ) duf_sql_call_str_cb, ( void * ) ( unsigned long long ) str_cb );
  r = duf_sql_vselect( duf_sql_call_str_cb /* sel_cb */ , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, sql, args );
  if ( duf_config->scan_trace )
    fprintf( stderr, "[SCAN ] %20s: L%u <duf_sql_vselect\n", __func__, pdi->level );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
int
duf_scan_items_sql( duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                    ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, sql );

  if ( duf_config->scan_trace )
    fprintf( stderr, "[SCAN ] %20s: L%u >duf_scan_vitems_sql str_cb=%p\n", __func__, pdi->level, ( void * ) ( unsigned long long ) str_cb );
  r = duf_scan_vitems_sql( str_cb, str_cb_udata, pdi, sccb, sql, args );
  /* r = duf_scan_vfiles_sql( str_cb, str_cb_udata, pdi, sccb, sql, args ); */
  if ( duf_config->scan_trace )
    fprintf( stderr, "[SCAN ] %20s: L%u <duf_scan_vitems_sql\n", __func__, pdi->level );

  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * this is callback of type: duf_scan_callback_file_t (second range): 
 * */
int
duf_sql_scan_print_file( unsigned long long pathid, unsigned long long filenameid, const char *name,
                         void *str_cb_udata, duf_dirinfo_t * pdi1, duf_scan_callbacks_t * cb, duf_record_t * precord )
{
  duf_dirinfo_t *pdi;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pdi = ( duf_dirinfo_t * ) str_cb_udata;

  if ( pdi )
  {
    if ( ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && pdi->level < pdi->u.maxdepth )
    {
      unsigned long long mdpathid;

      mdpathid = precord->presult[3] ? strtoll( precord->presult[3], NULL, 10 ) : 0;

      /* unsigned long long mdpath1 = strtoll( precord->presult[4], NULL, 10 ); */
      /* unsigned long long mdpath2 = strtoll( precord->presult[5], NULL, 10 ); */

#if  0
      {
        char *path;

        path = duf_pathid_to_path( pathid );
        /* printf( "%c %7llu: %-20s %lld:%016llx:%016llx @ %7llu: %s/%s\n", st ? '+' : '-', filenameid, name, mdpathid, mdpath1, mdpath2, pathid, path, */
        /*         name );                                                                                                                              */

        printf( "(%d) {%d} %c %7llu: %-20s %lld @ %7llu: %s/%s\n", pdi ? pdi->seq : 0, pdi ? pdi->level : 0, st ? '+' : '-', filenameid,
                name, mdpathid, pathid, path, name );
        mas_free( path );
      }
#endif
      {
        int r = 0;

        {
          char *filepath = NULL;
          struct stat st;

          filepath = filenameid_to_filepath( filenameid );

          /* fprintf( stderr, "************************* >>> [%llu:%llu] %s\n", pathid, filenameid, filepath ); */
          if ( filepath )
            r = stat( filepath, &st );
          mas_free( filepath );
        }
        printf( "%-3d>%2d %c %7llu: %2s %20s\n", pdi ? pdi->seq : 0, pdi ? pdi->level : 0, r == 0 ? ' ' : '-', filenameid,
                mdpathid ? "md" : "", name );
      }
      pdi->seq++;
    }
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}
