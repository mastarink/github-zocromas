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

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"


#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_file.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
/* #include "duf_file.h" */

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_scan.h"
/* ###################################################################### */



/* duf_sel_cb_items:
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
/* will be static! */
int
duf_sel_cb_items( duf_record_t * precord, va_list args, void *sel_cb_udata,
                  duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                  duf_dirhandle_t * pdhu )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  DUF_TRACE( current, 0, "pdhu : %d", pdhu ? ( pdhu->dfd ? 2 : 1 ) : 0 );
  if ( 1 || str_cb )
  {
    pdi->depth++;
    if ( pdi->node_type == DUF_NODE_NODE )
    {
      DUF_UFIELD( dirid );
      DUF_UFIELD( ndirs );
      DUF_UFIELD( nfiles );

      /* fprintf( stderr, "\t>>>>X> [%-30s] DEPTH=%u + 1\n", __func__, pdi->depth ); */
      memset( &pdi->levinfo[pdi->depth], 0, sizeof( pdi->levinfo[pdi->depth] ) );
      pdi->levinfo[pdi->depth].dirid = dirid;
      pdi->levinfo[pdi->depth].ndirs = ndirs;
      pdi->levinfo[pdi->depth].nfiles = nfiles;
      /* fprintf( stderr, "\t>>>>B> [%-30s] %5llu NDIRS[%u]=%llu\n", __func__, pdi->levinfo[pdi->depth].dirid, pdi->depth, ndirs ); */
    }
    if ( DUF_IF_VERBOSEN( 1 ) )
    {
      /* printf( "%s:%s='%s' -- r[%d]='%s' / %llu\n", __func__, precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord ), */
      /*         duf_sql_str_by_name( "parentid", precord ), duf_sql_ull_by_name( "parentid", precord ) );                                     */
      DUF_VERBOSE( 1, ":_____ %s::", __func__ );
      for ( int i = 0; i < precord->ncolumns; i++ )
      {
        DUF_VERBOSE( 1, ".%s;", precord->pnames[i] );
      }
      DUF_VERBOSE( 1, "; [ dirid=%llu ]", pdi->levinfo[pdi->depth].dirid );
    }
    else
      DUF_TRACE( scan, 0, "dirid:%llu", pdi->levinfo[pdi->depth].dirid );

    duf_dbgfunc( DBG_STEPULL, __func__, __LINE__, pdi->levinfo[pdi->depth].dirid );
    if ( 1 || pdi->levinfo[pdi->depth].dirid )
    {
      /* int rs = 0; */
      /* struct stat st_dir; */
      /* char *path; */

      /* path = duf_pathid_to_path( dirid ); */

      /* rs = stat( path, &st_dir ); */

      /* if ( str_cb ) */
      {
        DUF_VERBOSE( 1, "call str_cb> %p", ( void * ) ( unsigned long long ) str_cb );
/*
 * 4. call function str_cb
 * */
        if ( pdi )
        {
          if ( pdi->node_type == DUF_NODE_LEAF )
            pdi->seq_leaf++;
          else if ( pdi->node_type == DUF_NODE_NODE )
          {
            pdi->seq_node++;
            if ( pdi->depth > 0 )
            {
              /* printf( "\t>>>>U> [%-30s] %5llu NDIRS[%u - 1]=%llu - 1\n", __func__, pdi->levinfo[pdi->depth].dirid, pdi->depth, */
              /*         pdi->levinfo[pdi->depth - 1].ndirs );                                                                    */
              pdi->levinfo[pdi->depth - 1].ndirs--;
            }
          }
          pdi->seq++;
        }
        /* printf( "@ %llu of %llu :: %s L %u of %u\n", pdi->seq, pdi->u.maxseq, name, pdi->depth, pdi->u.maxdepth ); */
        /* called both for leaves (files) and nodes (dirs) */
        /* {                                                                                                                                 */
        /*   printf( "\t>>>>O> [%-30s] %5llu NDIRS[%u - 2]=%llu NDIRS[%u - 1]=%llu\n", __func__, pdi->levinfo[pdi->depth].dirid, pdi->depth, */
        /*           pdi->levinfo[pdi->depth - 2].ndirs, pdi->depth, pdi->levinfo[pdi->depth - 1].ndirs );                                   */
        /* }                                                                                                                                 */
        if ( str_cb )
        {
          if ( !pdi || ( ( !pdi->u.maxseq || pdi->seq <= pdi->u.maxseq )
                         && ( !pdi->u.maxitems.files || ( pdi->items.files ) < pdi->u.maxitems.files )
                         && ( !pdi->u.maxitems.dirs || ( pdi->items.dirs ) < pdi->u.maxitems.dirs )
                         && ( !pdi->u.maxitems.total || ( pdi->items.total ) < pdi->u.maxitems.total ) ) )
          {
            DUF_TRACE( current, 0, "> pdhu : %d; CALL str_cb:%s", pdhu ? ( pdhu->dfd ? 2 : 1 ) : 0,
                       duf_dbg_funname( ( duf_anyhook_t ) str_cb ) );
            r = ( *str_cb ) ( str_cb_udata, pdi, sccb, precord, pdhu );
            DUF_TRACE( current, 0, "<pdhu : %d", pdhu ? ( pdhu->dfd ? 2 : 1 ) : 0 );
          }
          else
          {
            r = DUF_ERROR_MAX_REACHED;
            /* printf( "@ %llu of %llu :: %s (%d)\n", pdi->seq, pdi->u.maxseq, name, pdi->seq <= pdi->u.maxseq ); */
          }
        }
        else
        {
          DUF_TRACE( error, 0, "str_cb not set" );
          r = 0;
        }
      }
      /* mas_free( path ); */
    }
    else
    {
      DUF_ERROR( "dirid missing" );
      r = DUF_ERROR_NO_DIRID;
    }
  }
  else
  {
    DUF_ERROR( "str_cb missing" );
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    r = DUF_ERROR_NO_STR_CB;
  }
  pdi->depth--;
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* 
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
static int
duf_scan_vitems_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                     duf_scan_callbacks_t * sccb, duf_dirhandle_t * pdhu, const char *sql, va_list args )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* duf_sel_cb_items:
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
  DUF_TRACE( scan, 0, "L%u >duf_sql_vselect with sel_cb=duf_sel_cb_items(%p); str_cb=%p", pdi ? pdi->depth : 0,
             ( void * ) ( unsigned long long ) duf_sel_cb_items, ( void * ) ( unsigned long long ) str_cb );

  pdi->node_type = node_type;
  DUF_TRACE( current, 0, "pdhu : %d", pdhu ? ( pdhu->dfd ? 2 : 1 ) : 0 );
  r = duf_sql_vselect( duf_sel_cb_items /* sel_cb */ , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, pdhu, sql,
                       args );

  DUF_TRACE( scan, 0, "L%u <duf_sql_vselect", pdi ? pdi->depth : 0 );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
int
duf_scan_items_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                    duf_scan_callbacks_t * sccb, duf_dirhandle_t * pdhu, const char *sql, ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, sql );

  DUF_TRACE( scan, 0, "%s L%u >duf_scan_vitems_sql str_cb=%p",
             ( node_type == DUF_NODE_LEAF ? "LEAF" : ( node_type == DUF_NODE_NODE ? "NODE" : "?" ) ), pdi ? pdi->depth : 0,
             ( void * ) ( unsigned long long ) str_cb );

  r = duf_scan_vitems_sql( node_type, str_cb, str_cb_udata, pdi, sccb, pdhu, sql, args );
  /* r = duf_scan_vfiles_sql( str_cb, str_cb_udata, pdi, sccb, sql, args ); */



  DUF_TRACE( scan, 0, "%s L%u <duf_scan_vitems_sql",
             ( node_type == DUF_NODE_LEAF ? "LEAF" : ( node_type == DUF_NODE_NODE ? "NODE" : "?" ) ), pdi ? pdi->depth : 0 );

  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
