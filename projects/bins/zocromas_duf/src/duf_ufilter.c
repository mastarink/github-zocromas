#include <assert.h>                                                  /* assert */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
/* #include "duf_errorn_defs_preset.h"                                  (* MAST_ERRORS_FILE; etc. ✗ *) */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
/* #include <mastar/error/mas_error_defs_ctrl.h> */
/* #include <mastar/error/mas_error_defs_make.h> */
/* #include <mastar/error/mas_error_defs.h> */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_ufilter.h"
/* ###################################################################### */

duf_ufilter_t *
duf_ufilter_create( void )
{
  duf_ufilter_t *pu = NULL;

  pu = mas_malloc( sizeof( duf_ufilter_t ) );
  memset( pu, 0, sizeof( duf_ufilter_t ) );
  pu->max_rel_depth = 99;
  return pu;
}

/* 20160212.130923 */
void
duf_ufilter_delete( duf_ufilter_t * pu )
{
/* Don't forget to update duf_ufilter_copy too */
  if ( pu )
  {
    mas_free( pu->std_node_set_name );                               /* 20160212.130932 */
    pu->std_node_set_name = NULL;                                    /* 20160212.130932 */

    mas_free( pu->std_leaf_set_name );                               /* 20160212.130932 */
    pu->std_leaf_set_name = NULL;                                    /* 20160212.130932 */

    mas_free( pu->filename );
    pu->filename = NULL;

    mas_free( pu->glob_db );
    pu->glob_db = NULL;

    mas_free( pu->mime.type );
    pu->mime.type = NULL;

    mas_free( pu->exif.camera );
    pu->exif.camera = NULL;

    mas_free( pu->tag.dir );
    pu->tag.dir = NULL;
    mas_free( pu->tag.file );
    pu->tag.file = NULL;

    mas_free( pu->glob_db_include );
    pu->glob_db_include = NULL;

    mas_free( pu->glob_db_exclude );
    pu->glob_db_exclude = NULL;

    mas_free( pu->same_as.md5 );
    pu->same_as.md5 = NULL;

    mas_free( pu->same_as.sha1 );
    pu->same_as.sha1 = NULL;

    mas_free( pu->same_as.exif );
    pu->same_as.exif = NULL;

    mas_argv_delete( pu->globx.include_fs_files.argc, pu->globx.include_fs_files.argv );
    pu->globx.include_fs_files.argc = 0;
    pu->globx.include_fs_files.argv = NULL;

    mas_argv_delete( pu->globx.exclude_fs_files.argc, pu->globx.exclude_fs_files.argv );
    pu->globx.exclude_fs_files.argc = 0;
    pu->globx.exclude_fs_files.argv = NULL;
    mas_free( pu );
  }
}

void
duf_ufilter_copy( duf_ufilter_t * pu, const duf_ufilter_t * pusrc )
{
  if ( pu && pusrc )
  {
    memcpy( pu, pusrc, sizeof( duf_ufilter_t ) );
    pu->tag.file = mas_strdup( pusrc->tag.file );
    pu->tag.dir = mas_strdup( pusrc->tag.dir );

    pu->glob_db = mas_strdup( pusrc->glob_db );

    pu->glob_db_include = mas_strdup( pusrc->glob_db_include );

    pu->glob_db_exclude = mas_strdup( pusrc->glob_db_exclude );

    pu->same_as.md5 = mas_strdup( pusrc->same_as.md5 );
    pu->same_as.sha1 = mas_strdup( pusrc->same_as.sha1 );
    pu->same_as.exif = mas_strdup( pusrc->same_as.exif );

    pu->mime.type = mas_strdup( pusrc->mime.type );

    pu->exif.camera = mas_strdup( pusrc->exif.camera );

    pu->globx.include_fs_files.argc = 0;
    pu->globx.include_fs_files.argv = NULL;
    pu->globx.include_fs_files.argc =
            mas_argv_clone( &pu->globx.include_fs_files.argv, pusrc->globx.exclude_fs_files.argc, pusrc->globx.exclude_fs_files.argv );

    pu->globx.exclude_fs_files.argc = 0;
    pu->globx.exclude_fs_files.argv = NULL;
    pu->globx.include_fs_files.argc =
            mas_argv_clone( &pu->globx.include_fs_files.argv, pusrc->globx.exclude_fs_files.argc, pusrc->globx.exclude_fs_files.argv );
  }
}

duf_ufilter_t *
duf_ufilter_clone( const duf_ufilter_t * pusrc )
{
  duf_ufilter_t *pu = NULL;

  if ( pusrc )
  {
    pu = duf_ufilter_create(  );
    duf_ufilter_copy( pu, pusrc );
  }
  return pu;
}

#if 0
duf_ufilter_t *
duf_ufilter_create_from( const duf_ufilter_t * pusrc )
{
  duf_ufilter_t *pu = NULL;

  pu = duf_ufilter_create(  );
  if ( pusrc )
    duf_ufilter_copy( pu, pusrc );
  return pu;
}
#endif
