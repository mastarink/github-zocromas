#define R_GOOD(_r) ((_r)>=0)
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>

#include <mastar/minierr/minierr.h>
/* #include <mastar/regerr/masregerr.h> */
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/structs.h>
#include <mastar/mulconfnt/global.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>
#include <mastar/mulconfnt/option_ref.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_tools.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo_db.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/levinfo/masxfs_levinfo_enums.h>

#include <mastar/qstd/qstd_query.h>
#include <mastar/qstd/qstd_mstmt_base.h>
#include <mastar/qstd/qstd_mstmt.h>
/* #include <mastar/qstd/qstd_mstmt_parents.h> */
/* #include <mastar/qstd/qstd_mstmt_names.h> */
/* #include <mastar/qstd/qstd_mstmt_sizes.h> */
/* #include <mastar/qstd/qstd_mstmt_datas.h> */
/* #include <mastar/qstd/qstd_mstmt_props.h> */

#include "structs.h"
#include "mysql.h"
#include "tree.h"

#include "dufnx.h"
/*
 * run   mastest <<<treedb:max-depth=32
 * run  --max-depth=3 --treedb mastest
 * run mastest/ --max-depth=3 --treedb
 * echo treedb:max-depth=32 | run  mastest
 *
 * */

static int
fillcb( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *qstdv _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  masxfs_levinfo_db_store( li, flags );
  return 0;
}

static int
store_fs2db( mucs_option_t * opt, void *userdata )
{
  rDECLGOOD;
  if ( opt && mucs_config_option_npos( opt ) > 0 )                   /* to skip argv[0] */
  {
    mas_dufnx_data_t *pdufnx_data _uUu_ = ( mas_dufnx_data_t * ) userdata;

    mas_qstd_t *qstd = dufnx_qstd( &pdufnx_data->mysql );
    const char *path = mucs_config_option_string_value( opt );

  /* assert(0); */
    assert( qstd );
    WARN( "________________________ %p '%s'", pdufnx_data, path );

    rC( mas_qstd_start_transaction( qstd ) );
    {
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 /* depth limit */ , 0 );

      {
        masxfs_levinfo_flags_t flagsfs _uUu_ = MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS | MASXFS_CB_SINGLE_CB;
        masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;

        {
          masxfs_entry_callback_t callback = { fillcb,.flags = MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */  };
          rC( mas_qstd_start_transaction( qstd ) );
        /* TODO FIXME : limiting maxdepth here (filling db) leads to memleak when scanning db 20170320.140237 */
          masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT;
          masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP;

          WARN( "(%d) ******** fill scan *******", rCODE );
          rC( masxfs_pathinfo_scan_cbs( pi, typeflags, &callback, qstd, flagsfs | xflags1, pdufnx_data->max_depth ) );
          WARN( "******** /fill scan *******" );
          rC( mas_qstd_end_transaction( qstd ) );
        }

        rC( mas_qstd_update_summary( qstd ) );
      }
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    }
    rC( mas_qstd_end_transaction( qstd ) );

  }
  rRET;
}

static int
arg_process( mucs_option_t * opt, void *userdata )
{
  rDECLGOOD;
  if ( opt && mucs_config_option_npos( opt ) > 0 )                   /* to skip argv[0] */
  {
    char *path;
    int len;
    mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;

  /* WARN( "%d. ARG:'%s'", opt->npos, opt->string_value ); */
#if 0
    path = mas_normalize_path_cwd( opt->string_value );
#else
    {
      char *path;

      path = mas_normalize_path_cwd_dots( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs//..//zocromas_dufnx/mastest/",
                                          TRUE );
      WARN( "path:'%s'", path );
      mas_free( path );
    }
    path = mas_normalize_path_cwd_dots( mucs_config_option_string_value( opt ), FALSE );
    WARN( "path:'%s'", path );
#endif
    len = strlen( path );
    if ( path[0] && path[1] && path[len - 1] == '/' )
      path[len - 1] = 0;
    mucs_config_option_set_string_value_na( opt, path );

    rC( dufnx_tree( mucs_config_option_string_value( opt ), pdufnx_data->max_depth, stdout, pdufnx_data->levinfo_flags, &pdufnx_data->mysql ) );
    WARN( "######################## %p [%d]", pdufnx_data, pdufnx_data->targv.argc );
  }
  rRET;
}

mucs_option_interface_t *
dufnx_config_interface( mas_dufnx_data_t * pdufnx_data )
{
  mucs_option_static_t soptions[] = {
    {.name = "treedb",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags,
     .def_nvalue.v_ulong = MASXFS_CB_MODE_DB,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}
    ,
    {.name = "treefs",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags,
     .def_nvalue.v_ulong = MASXFS_CB_MODE_FS,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}
    ,
  /* {.name = "updatedb",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags, */
  /*  .def_nvalue.v_ulong = MASXFS_CB_CAN_UPDATE_DB,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}                   */
  /* ,                                                                                                                        */
    {.name = "max-depth",.shortn = '\0' /* 'd' */ ,.restype = MUCS_RTYP_UINT,.cust_ptr = &pdufnx_data->max_depth}
    ,
    {.name = MUCS_NONOPT_NAME,.restype = MUCS_RTYP_TARG,.flags = MUCS_FLAG_AUTOFREE,.cust_ptr = &pdufnx_data->targv,
     .callback = arg_process,.cb_pass = 1}
    ,
    {.name = "store",.shortn = '\0',.restype = MUCS_RTYP_STRING,.flags = MUCS_FLAG_OPTIONAL_VALUE,
     .callback = store_fs2db,.cb_pass = 1}
    ,
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL}
  };
  mucs_option_interface_t *interface = mucs_config_soption_interface_create_setup( "main-table", soptions, TRUE );

  return interface;
}

int
dufnx( int argc, char *argv[] )
{
  rDECLGOOD;
  mas_dufnx_data_t dufnx_data = {.levinfo_flags = 0 };

  mucs_set_global_flag( MUCS_FLAG_USE_CBPASS );

  mucs_option_interface_t *interface = dufnx_config_interface( &dufnx_data );

  dufnx_config_mysql( interface, &dufnx_data );

/* mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL ); */
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG, 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, 0, "MAS_DUFNX", 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, 0, NULL, 1 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, argc, argv, 0 );

  rC( mucs_option_interface_lookup_all_multipass( interface, &dufnx_data, 2 ) );

  WARN( "=======================================" );
  mucs_config_option_interface_delete( interface );
  interface = NULL;

  mas_qstd_instance_delete(  );

  rRET;
}
