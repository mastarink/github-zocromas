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


/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with nrow1, nrows1, precord->presult from sql
 * str_cb + str_cb_udata to be called for each record
 * */

/* int                                                                                                                                               */
/* duf_sql_scan_files(  duf_record_t * precord, va_list args, void *sel_cb_udata,                                                */
/*                     duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )                       */
/* {                                                                                                                                                 */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                                                   */
/*   if ( str_cb )                                                                                                                                   */
/*   {                                                                                                                                               */
/*     unsigned long long pathid;                                                                                                                    */
/*     unsigned long long filenameid;                                                                                                                */
/*     const char *filename;                                                                                                                         */
/*                                                                                                                                                   */
/*     if ( duf_config->verbose > 1 )                                                                                                                */
/*     {                                                                                                                                             */
/*       (* printf( "%s:%s='%s' -- r[%d]='%s' / %llu\n", __func__, precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord ), *) */
/*       (*         duf_sql_str_by_name( "parentid", precord ), duf_sql_ull_by_name( "parentid", precord ) );                                     *) */
/*       fprintf( stderr, "        >>>>> %s::", __func__ );                                                                                          */
/*       for ( int i = 0; i < precord->ncolumns; i++ )                                                                                               */
/*       {                                                                                                                                           */
/*         fprintf( stderr, "%s;", precord->pnames[i] );                                                                                             */
/*       }                                                                                                                                           */
/*       fprintf( stderr, "\n" );                                                                                                                    */
/*     }                                                                                                                                             */
/*                                                                                                                                                   */
/*     (* pathid = strtoll( precord->presult[0], NULL, 10 );     *)                                                                                  */
/*     pathid = duf_sql_ull_by_name( "pathid", precord );                                                                                            */
/*     (* filenameid = strtoll( precord->presult[1], NULL, 10 ); *)                                                                                  */
/*     filenameid = duf_sql_ull_by_name( "filenameid", precord );                                                                                    */
/*     duf_dbgfunc( DBG_STEPULL, __func__, __LINE__, filenameid );                                                                                   */
/*     if ( filenameid )                                                                                                                             */
/*     {                                                                                                                                             */
/*       filename = precord->presult[2];                                                                                                             */
/*       duf_dbgfunc( DBG_STEPI2S, __func__, __LINE__, pathid, filenameid, filename );                                                               */
/*                                                                                                                                                   */
/*       {                                                                                                                                           */
/*         int r = 0;                                                                                                                                */
/*         struct stat st;                                                                                                                           */
/*         char *path;                                                                                                                               */
/*         char *filepath;                                                                                                                           */
/*                                                                                                                                                   */
/*         filepath = filenameid_to_filepath( filenameid );                                                                                          */
/*         path = duf_pathid_to_path( pathid );                                                                                                      */
/*                                                                                                                                                   */
/*         r = stat( filepath, &st );                                                                                                                */
/*                                                                                                                                                   */
/*         if ( duf_config->verbose > 1 )                                                                                                            */
/*         {                                                                                                                                         */
/*           const char *name2 = NULL;                                                                                                               */
/*                                                                                                                                                   */
/*           if ( str_cb == duf_sql_uni_scan_dir )                                                                                                   */
/*             name2 = "duf_sql_uni_scan_dir";                                                                                                       */
/*                                                                                                                                                   */
/*           fprintf( stderr, "call str_cb> %p:%s\n", ( void * ) ( unsigned long long ) str_cb, name2 ? name2 : "-" );                               */
/*         }                                                                                                                                         */
/*                                                                                                                                                   */
/*         ( *str_cb ) ( nrow1, nrows1, pathid, path, filenameid, filename, r == 0 ? &st : DUF_STAT_DEF, str_cb_udata, pdi, sccb, precord );           */
/*         mas_free( path );                                                                                                                         */
/*         mas_free( filepath );                                                                                                                     */
/*       }                                                                                                                                           */
/*       duf_dbgfunc( DBG_STEP, __func__, __LINE__ );                                                                                                */
/*       duf_dbgfunc( DBG_STEP, __func__, __LINE__ );                                                                                                */
/*     }                                                                                                                                             */
/*   }                                                                                                                                               */
/*   else                                                                                                                                            */
/*   {                                                                                                                                               */
/*     duf_dbgfunc( DBG_STEP, __func__, __LINE__ );                                                                                                  */
/*   }                                                                                                                                               */
/*   duf_dbgfunc( DBG_END, __func__, __LINE__ );                                                                                                     */
/*   return 0;                                                                                                                                       */
/* }                                                                                                                                                 */

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with nrow1, nrows1, precord->presult from sql
 * str_cb + str_cb_udata to be called for each record
 * */
int
duf_sql_scan_dirs( duf_record_t * precord, va_list args, void *sel_cb_udata,
                   duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( str_cb )
  {
    unsigned long long pathid;
    unsigned long long filenameid;
    const char *dirname;

    /* pathid = strtoll( precord->presult[0], NULL, 10 );     */
    pathid = duf_sql_ull_by_name( "pathid", precord ,0);
    filenameid = duf_sql_ull_by_name( "filenameid", precord ,1);

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

    duf_dbgfunc( DBG_STEPULL, __func__, __LINE__, pathid );
    if ( pathid )
    {
      dirname = precord->presult[2];
      duf_dbgfunc( DBG_STEPI2S, __func__, __LINE__, pathid, pathid, dirname );

      {
        /* int r = 0; */
        /* struct stat st_dir; */
        /* char *path; */

        /* path = duf_pathid_to_path( pathid ); */

        /* r = stat( path, &st_dir ); */

        if ( duf_config->verbose > 1 )
        {
          const char *name2 = NULL;

          if ( str_cb == duf_sql_uni_scan_dir )
            name2 = "duf_sql_uni_scan_dir";

          fprintf( stderr, "call str_cb> %p:%s\n", ( void * ) ( unsigned long long ) str_cb, name2 ? name2 : "-" );
        }
        ( *str_cb ) ( pathid,  filenameid  , dirname, str_cb_udata, pdi, sccb, precord );
        /* mas_free( path ); */
      }
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    }
    else if ( duf_config->verbose > 1 )
      fprintf( stderr, " ?????????????????\n" );
  }
  else
  {
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* 
 * called with nrow1, nrows1, precord->presult from sql
 * str_cb + str_cb_udata to be called for each record
 * sql + args must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * */

/* int                                                                                                                                          */
/* duf_scan_vfiles_sql( duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql, */
/*                      va_list args )                                                                                                          */
/* {                                                                                                                                            */
/*   int r = 0;                                                                                                                                 */
/*                                                                                                                                              */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                                              */
/*   if ( duf_config->verbose )                                                                                                                 */
/*     fprintf( stderr, "L%u vfiles>\n", pdi->level );                                                                                          */
/*   r = duf_sql_vselect( duf_sql_scan_files (* sel_cb *) , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, sql, args );                     */
/*   if ( duf_config->verbose )                                                                                                                 */
/*     fprintf( stderr, "L%u <vfiles\n", pdi->level );                                                                                          */
/*   duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );                                                                                            */
/*   return r;                                                                                                                                  */
/* }                                                                                                                                            */

/* 
 * called with nrow1, nrows1, precord->presult from sql
 * str_cb + str_cb_udata to be called for each record
 * sql + args must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * */
int
duf_scan_vdirs_sql( duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                    va_list args )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config->verbose )
    fprintf( stderr, "L%u vfiles>\n", pdi->level );
  r = duf_sql_vselect( duf_sql_scan_dirs /* sel_cb */ , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, sql, args );
  if ( duf_config->verbose )
    fprintf( stderr, "L%u <vfiles\n", pdi->level );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* 
 * str_cb + str_cb_udata to be called for each record
 * sql + ... must select pathid, ...
 * */
int
duf_scan_items_sql( duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                    ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, sql );

  r = duf_scan_vdirs_sql( str_cb, str_cb_udata, pdi, sccb, sql, args );
  /* r = duf_scan_vfiles_sql( str_cb, str_cb_udata, pdi, sccb, sql, args ); */

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
        printf( "%-3d>%2d %c %7llu: %2s %-20s\n", pdi ? pdi->seq : 0, pdi ? pdi->level : 0, r == 0 ? ' ' : '-', filenameid,
                mdpathid ? "md" : "", name );
      }
      pdi->seq++;
    }
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}
