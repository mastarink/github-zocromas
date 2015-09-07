#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_pathdepth */

#include "duf_config_ref.h"
#include "duf_option_defs.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
#include "duf_context.h"
#include "duf_path2dirid.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"



#include "duf_sql2.h"
#include "duf_maindb.h"


#include "duf_path2db.h"

#include "duf_selector.h"


/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */

/* 20150904.090635 */
static int
duf_pdi_attach_selected( duf_depthinfo_t * pdi, const char *pdi_name )
{
  DEBUG_STARTR( r );
  static const char *sql = "ATTACH DATABASE '${DB_PATH}${SELECTED_DB}' AS duf${SELECTED_DB}";
  char *worksql;

  worksql = duf_expand_selected_db( sql, pdi_name );
  DUF_TRACE( pdi, 0, "@@@to open db and attach %s", pdi->pdi_name );
  DORF( r, duf_main_db_open );

  DUF_TRACE( explain, 0, "attach selected database %s", worksql );
#if 1
  DUF_SQL_START_STMT( pdi, attach, worksql, r, pstmt );
#else
  DUF_SQL_START_STMT_NOPDI( worksql, r, pstmt );
#endif
  DUF_SQL_STEP( r, pstmt );
#if 1
  DUF_SQL_END_STMT( r, pstmt );
#else
  DUF_SQL_END_STMT_NOPDI( r, pstmt );
#endif
  mas_free( worksql );
  DEBUG_ENDR( r );
}

/* 20150904.090641 */
static int
duf_pdi_detach_selected( duf_depthinfo_t * pdi, const char *pdi_name )
{
  DEBUG_STARTR( r );
  static const char *sql = "DETACH DATABASE 'duf${SELECTED_DB}'";
  char *worksql;

  worksql = duf_expand_selected_db( sql, pdi_name );
  DUF_TRACE( pdi, 0, "@@@@@detach selected database %s", worksql );

  DUF_TRACE( explain, 0, "detach selected database %s", worksql );
#if 1
  DUF_SQL_START_STMT( pdi, attach, worksql, r, pstmt );
#else
  DUF_SQL_START_STMT_NOPDI( worksql, r, pstmt );
#endif
  DUF_SQL_STEP( r, pstmt );
#if 1
  DUF_SQL_END_STMT( r, pstmt );
#else
  DUF_SQL_END_STMT_NOPDI( r, pstmt );
#endif
  DUF_TRACE( pdi, 0, "@@@@detached selected database %s", worksql );
  mas_free( worksql );
  DEBUG_ENDR( r );
}

/* 20150904.085609 */
int
duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( !pdi->inited )
  {
    assert( real_path );
    pdi->inited = 1;
    pdi->pathinfo.depth = -1;
    DUF_TRACE( pdi, 8, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
    if ( real_path )
    {
      DOR( r, duf_pathdepth( real_path ) );
      if ( DUF_NOERROR( r ) )
        pdi->pathinfo.topdepth = r;
    }
    if ( !pdi->attached_selected )
    {
      if ( pdi->pdi_name )
        DOR( r, duf_pdi_attach_selected( pdi, pdi->pdi_name ) );
      pdi->attached_selected = 1;
    }
    assert( pdi->pathinfo.depth == -1 );
    DOR( r, duf_levinfo_create( pdi, pdi->pathinfo.topdepth, frecursive, opendir ) ); /* depth = -1 */
    assert( r < 0 || pdi->pathinfo.levinfo );
    /* assert( pdi->pathinfo.depth == -1 ); */
    if ( real_path )
      DOR( r, duf_real_path2db( pdi, caninsert, real_path, sql_set ) );

    DUF_TRACE( pdi, 8, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
  }
  DEBUG_ENDR( r );
}

/* 20150904.085510 */
int
duf_pdi_init_from_dirid( duf_depthinfo_t * pdi, unsigned long long dirid, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  path = duf_dirid2path( dirid, &r );
  DUF_TRACE( pdi, 8, "%d: %s", r, path );
  DOR( r, duf_pdi_init( pdi, path, caninsert, sql_set, frecursive, opendir ) );
  DUF_TRACE( pdi, 8, "%d: %s / %s", r, path, duf_levinfo_path( pdi ) );
  mas_free( path );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150904.085443 */
int
duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  DOR( r, duf_pdi_init( pdi, real_path, caninsert, sql_set, frecursive, opendir ) );

  if ( DUF_NOERROR( r ) )
  {
    DUF_TRACE( explain, 1, "converted to real_path: %s", real_path );
    DUF_TRACE( explain, 0, "added path: %s", real_path );
    /* DUF_TRACE( path, 10, "diridpid: %llu", duf_levinfo_dirid( pdi ) ); */
  }
  /* TODO */ assert( pdi->pathinfo.levinfo );
  DEBUG_ENDR( r );
}
#endif

/* 20150904.090827 */
static int
duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_ufilter_t * pu,
                const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  int frec = 0;

  assert( pdi );
  assert( real_path && *real_path == '/' );
  DUF_TRACE( pdi, 8, "@@@@(duf_pdi_recursive(pdi):%d) real_path:%s", duf_pdi_recursive( pdi ), real_path );
  frec = frecursive < 0 ? duf_pdi_recursive( pdi ) : frecursive;
  duf_pdi_shut( pdi );
  pdi->pu = pu;
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( pdi, real_path, caninsert, sql_set, frec, opendir ) );
  /*OR: return duf_pdi_init( pdi, real_path, 0 ); */
  DEBUG_ENDR( r );
}

/* 20150904.091522 */
int
duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, int caninsert, const duf_sql_set_t * sql_set, int frecursive )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  real_path = duf_realpath( cpath, &r );
  {
    DUF_TRACE( pdi, 8, "@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
    assert( pdi->pdi_name );
    DOR( r, duf_pdi_reinit( pdi, real_path, caninsert, duf_config->pu, sql_set, frecursive, duf_pdi_opendir( pdi ) ) );
    DUF_TRACE( pdi, 8, "@@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
  }
  mas_free( real_path );
  DEBUG_ENDR( r );
}

/* 20150904.091517 */
int
duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  {
    const char *cpath = NULL;

    cpath = duf_levinfo_path( pdi );
    DUF_TRACE( pdi, 8, "@@(frecursive:%d/%d) cpath:%s", frecursive, duf_pdi_recursive( pdi ), cpath );
    if ( cpath )
      path = mas_strdup( cpath );
  }
  DOR( r, duf_pdi_reinit_anypath( pdi, path, 0 /* canisert */ , sql_set, frecursive ) );
  mas_free( path );
  DEBUG_ENDR( r );
}

int
duf_pdi_shut( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->inited )
  {
    duf_clear_context( &pdi->context );
    duf_levinfo_delete( pdi );

    for ( int i = 0; i < pdi->num_idstatements; r = 0, i++ )
      DOR( r, duf_pdi_finalize_idstmt( pdi, i ) );

    mas_free( pdi->idstatements );
    pdi->idstatements = NULL;

    pdi->num_idstatements = 0;
    pdi->inited = 0;
    pdi->opendir = 0;
    pdi->pathinfo.depth = pdi->pathinfo.topdepth = 0;
    pdi->maxdepth = 0;
    pdi->changes = 0;
    pdi->pu = 0;
    pdi->items.dirs = pdi->items.files = pdi->items.total = 0;
    pdi->seq = pdi->seq_node = pdi->seq_leaf = 0;
    assert( !pdi->inited );
    assert( !pdi->opendir );
    assert( !pdi->items.files );
    assert( !pdi->items.dirs );
    assert( !pdi->items.total );
    assert( !pdi->maxdepth );
    assert( !pdi->pathinfo.topdepth );
    assert( !pdi->pathinfo.depth );
    assert( !pdi->pathinfo.levinfo );
    assert( !pdi->changes );
    assert( !pdi->seq_node );
    assert( !pdi->seq_leaf );
    assert( !pdi->seq );
    assert( !pdi->context.ptr );
    assert( !pdi->context.destructor );
    assert( !pdi->num_idstatements );
    assert( !pdi->idstatements );
    assert( !pdi->pu );
  }
  DEBUG_ENDR( r );
}

/* 20150904.085731 */
int
duf_pdi_close( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  duf_pdi_shut( pdi );
  if ( pdi->pdi_name && pdi->attached_selected )
    DOR( r, duf_pdi_detach_selected( pdi, pdi->pdi_name ) );
  pdi->attached_selected = 0;
  DEBUG_ENDR( r );
}
