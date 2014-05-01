#include <string.h>

#include <assert.h>
#include <limits.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_dh.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_sql_field.h"

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

#ifdef DUF_COMPILE_EXPIRED
typedef struct
{
  char *name;
  unsigned long long parentid;
} name_parid_udata_t;

/* will be static! */
/* 
 * this is callback of type:duf_sel_cb_t (second range; ; sel_cb)
 * */
int
duf_sel_cb_name_parid( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb_unused, void *str_cb_udata_unused,
                       duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb_unused )
{
  int r = 0;
  name_parid_udata_t *pud;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pud = ( name_parid_udata_t * ) sel_cb_udata;
  if ( precord->nrow == 0 )
  {
    DUF_UFIELD( parentid );
    DUF_SFIELD( dirname );
    pud->parentid = parentid;
    /* pud->parentid = duf_sql_ull_by_name( "parentid", precord, 0 ); */


    pud->name = mas_strdup( dirname );


    /* pud->parentid = strtoll( precord->presult[0], NULL, 10 ); */
    /* pud->name = mas_strdup( precord->presult[1] );            */
  }
  /* fprintf( stderr, "pud a:%d nrow:%d\n", precord->presult ? 1 : 0, nrow ); */
  /* fprintf( stderr, "pud b: %s :: %s\n", precord->presult[0], precord->presult[1] ); */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

char *
duf_pathid_to_path_dh( unsigned long long dirid, duf_dirhandle_t * pdh_pathid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;
  int openat = !duf_config->cli.noopenat && pdh_pathid;

  int test_o = duf_config->nopen;
  int test_c = duf_config->nclose;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  duf_check_dh( "Before" );


  if ( dirid == 0 )
  {
    duf_dirhandle_t dh = {.dfd = 0 };
    path = NULL;
    if ( openat )
    {
      dh.dirid = dirid;
      r = duf_open_dh( &dh, "/" );
      DUF_TEST_R( r );
      if ( r >= 0 )
        *pdh_pathid = dh;
      r = 0;
    }
  }
  else
  {
    name_parid_udata_t pathdef = {.name = NULL,.parentid = 0 };

/* get parentid for dirid */
    r = duf_sql_select( duf_sel_cb_name_parid, &pathdef, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT parentid, dirname " " FROM duf_paths WHERE id=%llu", dirid );
    DUF_TEST_R( r );
    if ( r >= 0 )
    {
      if ( pathdef.name )
      {
        if ( pathdef.parentid >= 0 )
        {
          char *parent = NULL;
          duf_dirhandle_t dhu = {.dfd = 0 };
          duf_dirhandle_t dh = {.dfd = 0 };
          duf_dirhandle_t *pdhu_pathid = NULL;

          if ( openat )
            pdhu_pathid = &dhu;

          parent = duf_pathid_to_path_dh( pathdef.parentid, pdhu_pathid, pdi, &r ); /* open!! */
          DUF_TEST_R( r );
          path = duf_join_path( parent, pathdef.name );
          if ( r >= 0 && openat )
          {
            r = duf_openat_dh( &dh, pdhu_pathid, pathdef.name, 0 /* asfile */  );
            DUF_TEST_R( r );
            if ( r >= 0 )
              *pdh_pathid = dh;
            r = 0;
          }
          if ( openat && pdhu_pathid && pdhu_pathid->dfd )
            duf_close_dh( pdhu_pathid );
          if ( parent )
            mas_free( parent );
        }
      }
    }
    if ( pathdef.name )
      mas_free( pathdef.name );
    duf_dbgfunc( DBG_ENDS, __func__, __LINE__, path );
  }
  if ( pr )
    *pr = r;
  {
    int dopen = ( duf_config->nopen - duf_config->nclose ) - ( test_o - test_c );

    assert( dopen == 1 || !openat );
  }
  duf_check_dh( "After" );
  return path;
}

char *
duf_pathid_to_path_s( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  char *s = NULL;

  s = duf_pathid_to_path_dh( dirid, ( duf_dirhandle_t * ) NULL, pdi, &r );
  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  return s;
}

#endif
/* will be static! */

/* 
 * this is callback of type:duf_sel_cb_t (second range; ; sel_cb)
 * */
int
duf_sel_cb_levinfo( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb_unused, void *str_cb_udata_unused,
                    duf_depthinfo_t * xpdi_unused, duf_scan_callbacks_t * sccb_unused )
{
  int r = 0;
  duf_levinfo_t *pli = ( duf_levinfo_t * ) sel_cb_udata;

  if ( pli )
  {
    DUF_UFIELD( dirid );
    DUF_SFIELD( dirname );
    DUF_UFIELD( ndirs );
    DUF_UFIELD( nfiles );

    /*?? memset( &pdi->levinfo[pdi->depth], 0, sizeof( pdi->levinfo[pdi->depth] ) ); */
    assert( dirname );
    assert( dirid );
    pli->dirid = dirid;
    pli->numdir = ndirs;
    pli->numfile = nfiles;
    pli->itemname = mas_strdup( dirname );
    /* unsigned depth;              */
    /* unsigned long long *levinfo; */
    /* unsigned long long seq;      */
    /* duf_node_type_t node_type;   */
    /* unsigned long long seq_leaf; */
    /* unsigned long long seq_node; */
    /* unsigned long long dirid;    */
    /* const char *name;            */
    /* unsigned long long items;    */
    /* unsigned long long ndirs;    */
    /* unsigned long long nfiles;   */
  }
  return r;
}

static unsigned long long
duf_realpath_to_pathid_x( char *rpath, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  unsigned long long pathid_new = 0;
  unsigned long long prevpathid = 0;
  unsigned long long pathid = 0;
  char *bd = NULL;
  char *cpath = NULL;


  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  bd = cpath = mas_strdup( rpath );

  DUF_TRACE( path, 0, "BD %s", bd );
  while ( bd && *bd && *bd == '/' )
    bd++;
  pdi->topdepth = pdi->depth = -1;
  while ( r >= 0 && bd && *bd )
  {
    duf_levinfo_t li = { 0 };
    char *ed;

    /* TODO DEPTH++ */

    prevpathid = pathid;
    /* fprintf( stderr, "%s: %llu bd: %s\n", __func__, pathid, bd ); */
    ed = bd;
    /* find next '/' */
    while ( ed && *ed && *ed != '/' )
      ed++;
    /* find next name */
    while ( ed && *ed && *ed == '/' )
      *ed++ = 0;
    {
      char *qbd;
      char *qname = NULL;

      qname = duf_single_quotes_2( bd );
      qbd = qname ? qname : bd;
      pathid_new = 0;

      {
        /* duf_depthinfo_t di = {.dirid = 0 }; */
        r = duf_sql_select( duf_sel_cb_levinfo /* duf_sql_path_to_pathid */ , &li, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            ( duf_scan_callbacks_t * ) NULL /* sccb */ ,
                            "SELECT duf_paths.id AS dirid, duf_paths.dirname,  duf_paths.parentid "
                            ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " " FROM duf_paths "
                            " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
                            " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id)                                    "
                            " WHERE duf_paths.parentid='%llu' AND dirname='%s' ", pathid, qbd );

        if ( r >= 0 )
          pathid_new = li.dirid;
        if ( !pathid_new )
        {
          r = DUF_ERROR_DB_NO_PATH;
          DUF_ERROR( "no pathid at %llu : %s; use -Q to see sql", pathid, qbd );
        }
        DUF_TRACE( path, 0, "(%d)SELECT PATHID %s => %llu; WHERE duf_paths.parentid='%llu' AND dirname='%s'", r, bd, pathid_new, pathid,
                   qbd );
      }
      DUF_TRACE( path, 0, "%s: pathid_new: %llu; qbd: %s", __func__, pathid_new, qbd );
      mas_free( qname );
      qname = NULL;
    }
    if ( r < 0 )
      break;
    if ( !pathid_new )
    {
      if ( notfound && !*notfound )
        *notfound = mas_strdup( bd );
      break;
    }

    pathid = pathid_new;
    bd = ed;
    /* DUF_PRINTF( 0, "@@@@@@@@ %d --- [%s]", pdi->depth, li.itemname ); */
    if ( r >= 0 )
    {
      r = duf_levinfo_down( pdi, pathid, li.itemname, bd && *bd ? 0 : li.numdir, bd && *bd ? 0 : li.numfile, 0 );
    }
    assert( pdi->depth >= 0 );
    if ( r >= 0 )
      r = duf_levinfo_openat_dh( pdi );
    DUF_OINV_OPENED( pdi-> );
    /* DUF_PRINTF( 0, "@@@@@@@@ %d %s", pdi->depth, duf_levinfo_path( pdi ) ); */
    duf_levinfo_clear_li( &li );
    /* pdi->depth++; */
  }
  pdi->topdepth = pdi->depth;
  if ( pprevpathid )
    *pprevpathid = prevpathid;
  mas_free( cpath );
  cpath = NULL;
  DUF_TRACE( path, 0, "%s: FINAL pathid: %llu; pathid_new: %llu;", __func__, pathid, pathid_new );
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid_new );
  return pathid_new;
}

static unsigned long long
duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  unsigned long long pathid_new = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  DUF_TRACE( action, 0, "PATH > ID" );

  if ( path )
  {
    char *real_path;

    real_path = mas_malloc( PATH_MAX );
    if ( real_path )
    {
      ( void ) realpath( path, real_path );

      pathid_new = duf_realpath_to_pathid_x( real_path, pprevpathid, notfound, pdi, &r );

      DUF_TRACE( path, 0, "(%d)REAL PATH %s => %llu", r, real_path, pathid_new );
      mas_free( real_path );
    }
  }

  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid_new );
  return pathid_new;
}

/* TODO new way : depth from root!;
 * duf_path_to_pathid must set depth at pdi and levinfo for each level
 * 
 * */
unsigned long long
duf_path_to_pathid( const char *path, duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  unsigned long long pathid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  pathid = duf_path_to_pathid_x( path, NULL, NULL, pdi, &r );

  DUF_TRACE( path, 0, "PATH TO PATHID '%s' => %llu", path, pathid );
  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}
