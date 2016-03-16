#include <assert.h>                                                  /* assert */
#include <string.h>
#include <libgen.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */
/* #include "duf_option_defs.h" */
/* #include "duf_optimpl_defs.h"                                        (* DUF_UG_FLAG; DUF_ACT_FLAG etc. ✗ *) */
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_path2dirid.h"                                          /* duf_dirid2path; etc. ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */

/* ###################################################################### */
#include "duf_ufilter_bind.h"
/* ###################################################################### */

SR( OTHER, init_filepath, duf_filepath_t * pfp, const char *filepath )
{
/* DUF_STARTR( r ); */
  char *pathname;

  memset( pfp, 0, sizeof( duf_filepath_t ) );
  pathname = mas_strdup( filepath );
  {
    char *dir;
    char *base;

    base = basename( pathname );
    dir = dirname( pathname );
    pfp->dirid = duf_path2dirid( dir, QPERRIND );
    pfp->name = mas_strdup( base );
  }
  mas_free( pathname );
/* DUF_ENDR( r ); */
  ER( OTHER, init_filepath, duf_filepath_t * pfp, const char *filepath );
}

static void
duf_clear_filepath( duf_filepath_t * pfp )
{
  if ( pfp )
    mas_free( pfp->name );
}

static
SR( OTHER, bind_ufilter_uni_i, duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused MAS_UNUSED )
{
/* DUF_STARTR( r ); */
#if 0
# define DUF_SQL_SE_BIND_PAIR( _fld, _name ) \
  if ( DUF_CONFIGG(vars.puz)->_name.flag ) \
  { \
    DUF_SQL_SE_BIND_LL_NZ_OPT( min ## _fld, DUF_CONFIGG(vars.puz)->_name.min, pstmt ); \
    DUF_SQL_SE_BIND_LL_NZ_OPT( max ## _fld, DUF_CONFIGG(vars.puz)->_name.max, pstmt ); \
  }
  assert( DUF_CONFIGX( vars.puz ) );
#else
# define DUF_SQL_SE_BIND_PAIR( _fld, _name ) \
  if ( pu->_name.flag ) \
  { \
    DUF_SQL_SE_BIND_LL_NZ_OPT( min ## _fld, pu->_name.min,  pstmt ); \
    DUF_SQL_SE_BIND_LL_NZ_OPT( max ## _fld, pu->_name.max,  pstmt ); \
    MAST_TRACE(sql, 3, "@@@bind %s: %llu-%llu", # _name, pu->_name.min, pu->_name.max ); \
  }
#endif
  MAST_TRACE( sql, 3, "py:%d - %llu", py ? 1 : 0, py ? py->topdirid : 0 );
  if ( py )
  {
    DUF_SQL_SE_BIND_LL_NZ_OPT( topDirID, py->topdirid, pstmt );
  /* T( "(%d) BIND topdirid:%llu", r , py->topdirid ); */
  /* assert( py->topdirid ); */
  }
  else if ( 0 == strncmp( "WITH", sqlite3_sql( pstmt ), 4 ) )
  {
    assert( 0 );
  }

  DUF_SQL_SE_BIND_PAIR( Size, size );
  DUF_SQL_SE_BIND_PAIR( Md5Same, same.md5 );
  DUF_SQL_SE_BIND_PAIR( Sha1Same, same.sha1 );
  DUF_SQL_SE_BIND_PAIR( ExifSame, same.exif );
  DUF_SQL_SE_BIND_PAIR( SizeSame, same.size );
  DUF_SQL_SE_BIND_PAIR( DataSame, same.data );
  DUF_SQL_SE_BIND_PAIR( MimeSame, same.mime );

  DUF_SQL_SE_BIND_PAIR( NameID, nameid );
  DUF_SQL_SE_BIND_PAIR( Md5ID, md5id );
  DUF_SQL_SE_BIND_PAIR( Sha1ID, sha1id );
  DUF_SQL_SE_BIND_PAIR( Sd2ID, sd5id );
  DUF_SQL_SE_BIND_PAIR( DataID, dataid );
  DUF_SQL_SE_BIND_PAIR( DirID, dirid );

  DUF_SQL_SE_BIND_PAIR( MTime, mtime );
  DUF_SQL_SE_BIND_PAIR( ExifDT, exifdt );
  DUF_SQL_SE_BIND_PAIR( Inode, inode );
  DUF_SQL_SE_BIND_PAIR( MimeID, mimeid );
  DUF_SQL_SE_BIND_PAIR( ExifID, exifid );
  if ( pu->mime.type )
  {
    DUF_SQL_SE_BIND_S_OPT( MimeType, pu->mime.type, pstmt );
    MAST_TRACE( sql, 3, "@@@bind MimeType: %s", pu->mime.type );
  }
  if ( pu->filename )
  {
    DUF_SQL_SE_BIND_S_OPT( Name, pu->filename, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->glob_db )
  {
    DUF_SQL_SE_BIND_S_OPT( GName, pu->glob_db, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->glob_db_include )
  {
    DUF_SQL_SE_BIND_S_OPT( GNameI, pu->glob_db_include, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->glob_db_exclude )
  {
    DUF_SQL_SE_BIND_S_OPT( GNameX, pu->glob_db_exclude, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->exif.camera )
  {
    char *t;

    t = mas_strdup( "%" );
    t = mas_strcat_x( t, pu->exif.camera );
    t = mas_strcat_x( t, "%" );
    DUF_SQL_SE_BIND_S_OPT( Camera, t, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
    mas_free( t );
  }

#define DUF_SQL_SE_BIND_SAME(_u, _l) \
	if ( pu->same_as._l ) \
	{ \
	  duf_filepath_t fp; \
 \
	  CR( init_filepath, &fp, pu->same_as._l ); \
	  DUF_SQL_SE_BIND_LL_NZ_OPT( GSame ## _u ## PathID, fp.dirid,  pstmt ); \
	  DUF_SQL_SE_BIND_S_OPT( GSameAs ## _u, fp.name,  pstmt ); \
	  MAST_TRACE( sql, 0, "@@@bind GSame" # _u "PathID:%llu", fp.dirid ); \
	  duf_clear_filepath( &fp ); \
	  if ( QNOERR && !fp.dirid ) \
	    ERRMAKE( NOT_IN_DB ); \
	}
#if 0
  if ( pu->same_as.md5 )
  {
    duf_filepath_t fp;

    CR( init_filepath, &fp, pu->same_as.md5 );
    DUF_SQL_SE_BIND_LL_NZ_OPT( GSameMd5PathID, fp.dirid, r, pstmt );
    DUF_SQL_SE_BIND_S_OPT( GSameAsMd5, fp.name, r, pstmt );
    MAST_TRACE( sql, 0, "@@@bind GSameMd5PathID:%llu", fp.dirid );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
  if ( pu->same_as.sha1 )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_as.sha1 ) );
    DUF_SQL_SE_BIND_LL_NZ_OPT( GSameSha1PathID, fp.dirid, r, pstmt );
    DUF_SQL_SE_BIND_S_OPT( GSameAsSha1, fp.name, r, pstmt );
    MAST_TRACE( sql, 0, "@@@bind GSameSha1PathID:%llu", fp.dirid );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
  if ( pu->same_as.exif )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_as.exif ) );
    DUF_SQL_SE_BIND_LL_NZ_OPT( GSameExifPathID, fp.dirid, r, pstmt );
    DUF_SQL_SE_BIND_S_OPT( GSameAsExif, fp.name, r, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
#else
  DUF_SQL_SE_BIND_SAME( Md5, md5 );
  DUF_SQL_SE_BIND_SAME( Sha1, sha1 );
  DUF_SQL_SE_BIND_SAME( Exif, exif );
#endif

#if 1
  if ( pu->tag.file )
  {
    DUF_SQL_SE_BIND_S_OPT( TagFile, pu->tag.file, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->tag.dir )
  {
    DUF_SQL_SE_BIND_S_OPT( TagDir, pu->tag.dir, pstmt );
    MAST_TRACE( sql, 3, "@@@bind " );
  }
  DUF_SQL_SE_BIND_I_NZ_OPT( Option_Val_With_Tag_File, DUF_OPTION_VAL_FILTER_WITH_TAG_FILE, pstmt );
  DUF_SQL_SE_BIND_I_NZ_OPT( fFast, /* DUF_ACTG_FLAG( fast ) */ duf_get_config_flag_act_fast(  ), pstmt );
  DUF_SQL_SE_BIND_I_NZ_OPT( fFresh, /* DUF_ACTG_FLAG( fresh ) */ duf_get_config_flag_act_fresh(  ), pstmt );
  MAST_TRACE( sql, 3, "@@@bind Option_Val_With_Tag_File='%d'", DUF_OPTION_VAL_FILTER_WITH_TAG_FILE );
#endif
/* DUF_ENDR( r ); */
  ER( OTHER, bind_ufilter_uni_i, duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused );
}

SR( OTHER, bind_ufilter_uni, duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused,
    const void *ptr MAS_UNUSED )
{
/* DUF_STARTR( r ); */
  if ( pu )
    CR( bind_ufilter_uni_i, pstmt, pu, py, ttarg_unused );
  else
  {
    MAST_TRACE( sql, 3, "@@@bind - no pu!" );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, bind_ufilter_uni, duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused,
      const void *ptr );
}
