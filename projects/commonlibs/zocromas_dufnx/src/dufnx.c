#define R_GOOD(_r) ((_r)>=0)
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/structs.h>
#include <mastar/mulconfnt/global.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>
#include <mastar/mulconfnt/option_ref.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_tools.h>
#include <mastar/levinfo/masxfs_levinfo_path.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo_digest.h>
#include <mastar/levinfo/masxfs_levinfo_db.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/levinfo/masxfs_levinfo_digest_enums.h>
/* #include <mastar/levinfo/masxfs_levinfo_enums.h> */

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
 * run   --store=mastest --disable-warn
 * run   --drop-tables
 * */
static int test_num = 0;
static int
fillcb( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *qstdv _uUu_, unsigned long serial _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  rDECLGOOD;

  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

  if ( detype == MASXFS_ENTRY_REG_NUM && ( flags & MASXFS_CB_DIGESTS ) )
  {
    int fd = masxfs_levinfo_fd_ref( li, flags );

    if ( fd )
    {
      int sz = 0;
      const unsigned char *dg = NULL;

      assert( !li->digests );
      li->digests = masxfs_digests_create_setup( fd, 1024 * 25, 0, 0 );

    /* rC( masxfs_digests_add( li->digests, MASXFS_DIGEST_MD5 ) ); */
      rC( masxfs_digests_add( li->digests, MASXFS_DIGEST_SHA1 ) );
    /* rC( masxfs_digests_add( li->digests, MASXFS_DIGEST_MAGIC ) ); */
    /* rC( masxfs_digests_add( li->digests, MASXFS_DIGEST_SHA1 ) ); */
      rC( masxfs_digests_compute( li->digests ) );
      sz = masxfs_digests_getn( li->digests, 0, &dg );
      if ( sz )
      {
        char sbuffer[512] = { 0 };
        char *psbuf = sbuffer;

        for ( ssize_t i = 0; i < sz; i++ )
        {
          snprintf( psbuf, sizeof( sbuffer ) - ( psbuf - sbuffer ), "%02x", dg[i] );
          psbuf += 2;
        }
#if 0
        const char *epath = masxfs_levinfo_path_ref( li, flags );

        WARN( "sz:%d - %s - %s", sz, sbuffer, epath );
#endif
      }
    /* masxfs_digests_delete( li->digests ); */
    }
  }
  ++test_num;
  if ( 0 )
  {
    char *path = masxfs_levinfo_li2path( li );

    WARN( "db_store %d : %s", test_num, path );
    mas_free( path );
  }
  masxfs_levinfo_db_store( li, flags );
  rRET;
}

static int
dufnx_config_store_fs2db( mucs_option_t * opt, void *userdata, void *extradata _uUu_ )
{
  rDECLGOOD;
  if ( opt && mucs_config_option_npos( opt ) > 0 )                   /* to skip argv[0] */
  {
    mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;

    mas_qstd_t *qstd = dufnx_qstd( &pdufnx_data->mysql );
    const char *path = mucs_config_option_string_value( opt );

    assert( qstd );
    rC( mas_qstd_create_tables( qstd ) );
    WARN( "________________________ %p '%s'", pdufnx_data, path );

    if ( rGOOD )
    {
    /*20180109.1418 rC( mas_qstd_start_transaction( qstd ) ); */
      {
        masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 /* depth limit */ , 0 );
        masxfs_levinfo_flags_t flagsfs _uUu_ = MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS | MASXFS_CB_SINGLE_CB;

//      masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;
        masxfs_entry_filter_t entry_filter = {.typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR /*, .maxdepth=pdufnx_data->max_depth */
        };

        masxfs_entry_callback_t callback = {
          .fun_simple=fillcb,.flags = MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT | MASXFS_CB_DIGESTS | MASXFS_CB_PATH /* | MASXFS_CB_NO_FD */
        };
        rC( mas_qstd_start_transaction( qstd ) );
      /* TODO FIXME : limiting maxdepth here (filling db) leads to memleak when scanning db 20170320.140237 */
        masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT;
        masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP;

        WARN( "(%d) ******** fill scan *******", rCODE );
        rC( masxfs_pathinfo_scanf_cbs( pi, &entry_filter, &callback, qstd, flagsfs | xflags1, 0 ) );
        WARN( "******** /fill scan *******" );
        rC( mas_qstd_end_transaction( qstd ) );
/* TODO commit !! */
        rC( mas_qstd_update_summary( qstd ) );
        masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
      }
    /*20180109.1418 rC( mas_qstd_end_transaction( qstd ) ); */
    }
  }
  rRET;
}

static int
dufnx_config_drop_tables( mucs_option_t * opt _uUu_, void *userdata, void *extradata _uUu_ )
{
  rDECLGOOD;
  mas_dufnx_data_t *pdufnx_data _uUu_ = ( mas_dufnx_data_t * ) userdata;

  mas_qstd_t *qstd = dufnx_qstd( &pdufnx_data->mysql );

  assert( qstd );
  rC( mas_qstd_drop_tables( qstd ) );
  rRET;
}

static int
dufnx_config_disable_warn( mucs_option_t * opt _uUu_, void *userdata _uUu_, void *extradata _uUu_ )
{
  rDECLGOOD;
  minierr_disable( MAS_MIER_TYPE_WARN );
  rRET;
}
static int
dufnx_test1( mucs_option_t * opt _uUu_, void *userdata _uUu_, void *extradata _uUu_ )
{
  rDECLGOOD;
  WARN("ONE");
  rRET;
}
static int
dufnx_test2( mucs_option_t * opt _uUu_, void *userdata _uUu_, void *extradata _uUu_ )
{
  rDECLGOOD;
  WARN("TWO");
  rRET;
}


#if 0 
static int 
dufnx_config_arg_process( mucs_option_t * opt, void *userdata, void *extradata _uUu_ )
{
  rDECLGOOD;
  if ( opt && mucs_config_option_npos( opt ) > 0 )                   /* to skip argv[0] */
  {
    mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;
    const char *path = mucs_config_option_string_value( opt );
    char *real_path = mas_normalize_path_cwd_dots( path, FALSE );
    int len = strlen( real_path );

    mucs_config_option_set_string_value_na( opt, real_path );

    rC( dufnx_tree( mucs_config_option_string_value( opt ), &pdufnx_data->entry_filter, stdout, pdufnx_data->levinfo_flags, &pdufnx_data->mysql ) );
  }
  rRET;
}
#endif

mucs_option_interface_t *
dufnx_config_interface( mas_dufnx_data_t * pdufnx_data )
{
  mas_dufnx_data_t *d = pdufnx_data;

/*
 * .p same as .cust_ptr
 * .rt same as .restype
 * .f same as .flags
 * .cb same as .callback
 */

/*
 * run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up-root --min-size=5000
 * run    --treedb mastest/  --no-empty-dirs --name='*.sh' --cb-up-root --max-size=100
 * run    --treedb mastest/  --no-empty-dirs  --cb-up-root --min-nsame=13
 * */
#define FDV MUCS_FLAG_ONLY_DEF_NVALUE
/* FDV : short for MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE */
  mucs_option_static_t soptions[] = {
    {.name = "treedb",.rt = 'O',.p = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_MODE_DB,.f = FDV}
    , {.name = "treefs",.rt = 'O',.p = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_MODE_FS,.flags = FDV}

    , {.name = "no-empty-dirs",.rt = 'O',.cust_ptr = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_SKIP_EMPTY,.flags = FDV}
    , {.name = "empty-dirs",.rt = MUCS_RTYP_ULONG_NOR,.cust_ptr = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_SKIP_EMPTY,.flags = FDV}
    , {.name = "max-depth",.restype = 'u',.p = &d->entry_filter.maxdepth}
    , {.name = "min-size",.restype = MUCS_RTYP_ULONG_LONG,.p = &d->entry_filter.min_size}
    , {.name = "max-size",.restype = MUCS_RTYP_ULONG_LONG,.p = &d->entry_filter.max_size}
    , {.name = "min-nsame",.restype = 'u',.p = &d->entry_filter.min_nsame_digest}
    , {.name = "max-nsame",.restype = 'u',.p = &d->entry_filter.max_nsame_digest}

  /* , {.name = MUCS_NONOPT_NAME,.restype = 'T',.p = &d->targv,.callback = dufnx_config_arg_process,.cb_pass = 1} */
    , {.name = MUCS_NONOPT_NAME,.restype = 'T',.p = &d->targv,.callback_s = dufnx_data_tree,.cb_pass = 1}

    , {.name = "store",.restype = 'S',.f = MUCS_FLAG_OPTIONAL_VALUE,.cb = dufnx_config_store_fs2db,.cb_pass = 1}
    , {.name = "name",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &d->entry_filter.glob /*,.flags = MUCS_FLAG_AUTOFREE|MUCS_FLAG_USE_VPASS */ }
    , {.name = "drop-tables",.shortn = '\0',.cb = dufnx_config_drop_tables}
    , {.name = "disable-warn",.cb = dufnx_config_disable_warn}
    , {.name = "test",.cb = dufnx_test1}
    , {.name = "test2",.cb = dufnx_test2}
  /* , {.name = "updatedb",.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.p = &d->levinfo_flags, */
  /*  .def_nvalue.v_ulong = MASXFS_CB_CAN_UPDATE_DB,.f = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE},                   */
    , {NULL}
  };
#undef FDV
  mucs_option_interface_t *interface =
          mucs_config_soption_interface_create_setup( "main-table", soptions, TRUE /* special_options */ , MUCS_FLAG_AUTOFREE );
  return interface;
}

void
dufnx_config_cb_plus( mucs_option_interface_t * interface, mas_dufnx_data_t * pdufnx_data )
{
  mas_dufnx_data_t *d = pdufnx_data;

#define FDV MUCS_FLAG_ONLY_DEF_NVALUE
  mucs_option_static_t soptions_cbplus[] = {
    {.name = "cb-self",.rt = MUCS_RTYP_ULONG_OR,.cust_ptr = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_SELF,.flags = FDV}
    , {.name = "cb-up",.rt = MUCS_RTYP_ULONG_OR,.cust_ptr = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_UP,.flags = FDV}
    , {.name = "cb-from-root",.rt = MUCS_RTYP_ULONG_OR,.cust_ptr = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_FROM_ROOT,.flags = FDV}
    , {.name = "cb-up-root",.rt = MUCS_RTYP_ULONG_OR,.cust_ptr = &d->levinfo_flags,.def_nvalue.v_ulong = MASXFS_CB_UP_ROOT,.flags = FDV}
  };
#undef FDV
  mucs_config_soption_interface_tabnode_add( interface, "cb-plus-table", soptions_cbplus, MUCS_FLAG_AUTOFREE );
}

int
dufnx( int argc, char *argv[] )
{
  rDECLGOOD;
  mas_dufnx_data_t dufnx_data = {.levinfo_flags = 0,.entry_filter = {0} };

  mucs_set_global_flag( MUCS_FLAG_USE_CBPASS );

  mucs_option_interface_t *interface = dufnx_config_interface( &dufnx_data );

  dufnx_config_mysql( interface, &dufnx_data );
  dufnx_config_cb_plus( interface, &dufnx_data );

/* mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL ); */
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, MULCONFNT_ETC_CONFIG /* name */ , 0, MULCONFNT_ETC_CONFIG, 0 /* min_pass */ , 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, NULL /* name */ , 0, "MAS_DUFNX", 0 /* min_pass */ , 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, NULL /* name */ , 0, NULL, 1 /* min_pass */ , 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, NULL /* name */ , argc, argv, 0 /* min_pass */ , 0 );

  rC( mucs_option_interface_lookup_all_multipass( interface, &dufnx_data, 2 ) );

  WARN( "=======================================" );
  mucs_config_option_interface_delete( interface );
  interface = NULL;

  mas_qstd_instance_delete(  );

  rRET;
}
