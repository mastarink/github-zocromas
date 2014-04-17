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



/* duf_sel_cb_items:
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
int
duf_sel_cb_items( duf_record_t * precord, va_list args, void *sel_cb_udata,
                  duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
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
    if ( duf_config->cli.dbg.verbose > 1 )
    {
      /* printf( "%s:%s='%s' -- r[%d]='%s' / %llu\n", __func__, precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord ), */
      /*         duf_sql_str_by_name( "parentid", precord ), duf_sql_ull_by_name( "parentid", precord ) );                                     */
      fprintf( stderr, "        _____ %s::", __func__ );
      for ( int i = 0; i < precord->ncolumns; i++ )
      {
        fprintf( stderr, "%s;", precord->pnames[i] );
      }
      fprintf( stderr, " [ dirid=%llu ]\n", pdi->levinfo[pdi->depth].dirid );
    }
    DUF_TRACE( scan, 0, "dirid:%llu", pdi->levinfo[pdi->depth].dirid );

    duf_dbgfunc( DBG_STEPULL, __func__, __LINE__, pdi->levinfo[pdi->depth].dirid );
    if ( 1 || pdi->levinfo[pdi->depth].dirid )
    {
      /* int rs = 0; */
      /* struct stat st_dir; */
      /* char *path; */

      /* path = duf_pathid_to_path( dirid ); */

      /* rs = stat( path, &st_dir ); */

      if ( str_cb )
      {
        if ( duf_config->cli.dbg.verbose > 1 )
        {
          const char *name2 = NULL;

          if ( str_cb == duf_uni_scan_dir )
            name2 = "duf_uni_scan_dir";

          fprintf( stderr, "call str_cb> %p:%s\n", ( void * ) ( unsigned long long ) str_cb, name2 ? name2 : "-" );
        }
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
        if ( !pdi || ( ( !pdi->u.maxseq || pdi->seq <= pdi->u.maxseq ) 
	      && ( !pdi->u.maxitems.files || (pdi->items.files) < pdi->u.maxitems.files )
	      && ( !pdi->u.maxitems.dirs || (pdi->items.dirs) < pdi->u.maxitems.dirs )
	      && ( !pdi->u.maxitems.total || (pdi->items.total) < pdi->u.maxitems.total )
	      ) )
        {
          r = ( *str_cb ) ( str_cb_udata, pdi, sccb, precord );
        }
        else
        {
          r = 1;
          /* printf( "@ %llu of %llu :: %s (%d)\n", pdi->seq, pdi->u.maxseq, name, pdi->seq <= pdi->u.maxseq ); */
        }
      }
      else
      {
        r = 0;
      }
      /* mas_free( path ); */
    }
    else
    {
      printf( "ERROR %s : %d\n", __func__, r );
      if ( duf_config->cli.dbg.verbose > 1 )
        fprintf( stderr, " ?????????????????\n" );
    }
  }
  else
  {
    printf( "ERROR %s : %d\n", __func__, r );
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  /* printf( "\t>>>>Y> [%-30s] DEPTH=%u - 1 %llu - %llu\n", __func__, pdi->depth, pdi->levinfo[pdi->depth].ndirs, pdi->levinfo[pdi->depth-1].ndirs ); */
  pdi->depth--;
  if ( pdi->depth < 0 )
  {
    fprintf( stderr, "@@@@@@@@@@@@@@@@@@@@@@@@@ Error\n" );
    exit( 4 );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* 
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
static int
duf_scan_vitems_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi,
                     duf_scan_callbacks_t * sccb, const char *sql, va_list args )
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
  r = duf_sql_vselect( duf_sel_cb_items /* sel_cb */ , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, sql, args );

  DUF_TRACE( scan, 0, "L%u <duf_sql_vselect", pdi ? pdi->depth : 0 );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
int
duf_scan_items_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi,
                    duf_scan_callbacks_t * sccb, const char *sql, ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, sql );

  DUF_TRACE( scan, 0, "%s L%u >duf_scan_vitems_sql str_cb=%p",
             ( node_type == DUF_NODE_LEAF ? "LEAF" : ( node_type == DUF_NODE_NODE ? "NODE" : "?" ) ), pdi ? pdi->depth : 0,
             ( void * ) ( unsigned long long ) str_cb );

  r = duf_scan_vitems_sql( node_type, str_cb, str_cb_udata, pdi, sccb, sql, args );
  /* r = duf_scan_vfiles_sql( str_cb, str_cb_udata, pdi, sccb, sql, args ); */



  DUF_TRACE( scan, 0, "%s L%u <duf_scan_vitems_sql",
             ( node_type == DUF_NODE_LEAF ? "LEAF" : ( node_type == DUF_NODE_NODE ? "NODE" : "?" ) ), pdi ? pdi->depth : 0 );

  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * this is callback of type: duf_scan_callback_file_t (second range): 
 * */
/* int                                                                                                                                          */
/* duf_str_cb_scan_print_file( unsigned long long pathid, unsigned long long filenameid, const char *name,                                      */
/*                             void *str_cb_udata, duf_dirinfo_t * pdi1, duf_scan_callbacks_t * cb, duf_record_t * precord )                    */
/* {                                                                                                                                            */
/*   duf_dirinfo_t *pdi;                                                                                                                        */
/*                                                                                                                                              */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                                              */
/*   pdi = ( duf_dirinfo_t * ) str_cb_udata;                                                                                                    */
/*                                                                                                                                              */
/*   if ( pdi )                                                                                                                                 */
/*   {                                                                                                                                          */
/*     if ( ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && ( pdi->depth < pdi->u.maxdepth ) )                                                */
/*     {                                                                                                                                        */
/*       unsigned long long mdpathid;                                                                                                           */
/*                                                                                                                                              */
/*       mdpathid = precord->presult[3] ? strtoll( precord->presult[3], NULL, 10 ) : 0;                                                         */
/*                                                                                                                                              */
/*       (* unsigned long long mdpath1 = strtoll( precord->presult[4], NULL, 10 ); *)                                                           */
/*       (* unsigned long long mdpath2 = strtoll( precord->presult[5], NULL, 10 ); *)                                                           */
/*                                                                                                                                              */
/* #if  0                                                                                                                                       */
/*       {                                                                                                                                      */
/*         char *path;                                                                                                                          */
/*                                                                                                                                              */
/*         path = duf_pathid_to_path( pathid );                                                                                                 */
/*         (* printf( "%c %7llu: %-20s %lld:%016llx:%016llx @ %7llu: %s/%s\n", st ? '+' : '-', filenameid, name, mdpathid, mdpath1, mdpath2, *) */
/*         (*         pathid, path, name );                                                                                                  *) */
/*                                                                                                                                              */
/*         printf( "(%d) {%d} %c %7llu: %-20s %lld @ %7llu: %s/%s\n", pdi ? pdi->seq : 0, pdi ? pdi->depth : 0, st ? '+' : '-', filenameid,     */
/*                 name, mdpathid, pathid, path, name );                                                                                        */
/*         mas_free( path );                                                                                                                    */
/*       }                                                                                                                                      */
/* #endif                                                                                                                                       */
/*       {                                                                                                                                      */
/*         int r = 0;                                                                                                                           */
/*                                                                                                                                              */
/*         {                                                                                                                                    */
/*           char *filepath = NULL;                                                                                                             */
/*           struct stat st;                                                                                                                    */
/*                                                                                                                                              */
/*           filepath = filenameid_to_filepath( filenameid );                                                                                   */
/*                                                                                                                                              */
/*           (* fprintf( stderr, "************************* >>> [%llu:%llu] %s\n", pathid, filenameid, filepath ); *)                           */
/*           if ( filepath )                                                                                                                    */
/*             r = stat( filepath, &st );                                                                                                       */
/*           mas_free( filepath );                                                                                                              */
/*         }                                                                                                                                    */
/*         printf( "%-5llu>%2d %c %7llu: %2s %20s\n", pdi ? pdi->seq : 0, pdi ? pdi->depth : 0, r == 0 ? ' ' : '-', filenameid,                 */
/*                 mdpathid ? "md" : "", name );                                                                                                */
/*       }                                                                                                                                      */
/*     }                                                                                                                                        */
/*   }                                                                                                                                          */
/*   duf_dbgfunc( DBG_END, __func__, __LINE__ );                                                                                                */
/*   return 0;                                                                                                                                  */
/* }                                                                                                                                            */
