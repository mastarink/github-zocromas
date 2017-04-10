#define R_GOOD(_r) ((_r)>=0)
#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/levinfo/masxfs_levinfo_types.h>
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include <mastar/qstd/qstd_mstmt_base.h>

#include "dufnx.h"

static int numline_treecb = 0;

/* of type: masxfs_scan_fun_simple_t */
static int
treecb( masxfs_levinfo_t * li _uUu_, masxfs_levinfo_flags_t flags _uUu_, void *data _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) data;
  static masxfs_depth_t top_depth = 0;

/* EXAM( !epath, TRUE, "%d ? %d" ); */
  size_t size = masxfs_levinfo_size_ref( li, flags );
  int fd _uUu_ = masxfs_levinfo_fd_ref( li, flags );
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *epath _uUu_ = masxfs_levinfo_path_ref( li, flags );

  if ( !numline_treecb && depth )
    top_depth = depth - 1;
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", top_depth + 1, flags );

  numline_treecb++;
#if 1
/* /usr/bin/tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
  fprintf( fil, "%4d\t%s[%-10ld %10ld]  %s\n", numline_treecb, prefix ? prefix : "", inode, size, ename ? ename : "" /*, epath ? epath : "" */  );
#else
  fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", numline_treecb, prefix ? prefix : "", size, fd, ( long ) depth, inode,
           ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  return 0;
}

static int
mas_tree( const char *real_path, masxfs_depth_t maxdepth, FILE * fil, masxfs_levinfo_flags_t inflags, mas_dufnx_mysql_data_t * mysql )
{
  rDECLGOOD;

  masxfs_entry_callback_t callbacks[] = {
    {treecb,.flags = MASXFS_CB_NAME /* MASXFS_CB_PATH  */  | MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ },
    {NULL}
  };
  mas_qstd_instance_setup( mysql->server, mysql->user, mysql->password, mysql->db, mysql->port );
  masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );

  masxfs_levinfo_flags_t walkflags = MASXFS_CB_RECURSIVE | MASXFS_CB_STAT | MASXFS_CB_SINGLE_CB;
  masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;

/* masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT; */

/* masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP; */
  masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF;

  walkflags |= inflags;
  numline_treecb = 0;
  rC( masxfs_pathinfo_scan_cbs( pi, typeflags, callbacks, fil /* data */ , walkflags | xflags2,
                                maxdepth ) );
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
  mas_qstd_instance_delete(  );
  rRET;
}

static int
arg_process( mucs_option_t * opt _uUu_, void *userdata )
{
  rDECLGOOD;
  char *path;
  int len;
  mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;

#if 0
  path = mas_normalize_path_cwd( opt->string_value );
#else
  path = mas_normalize_path_cwd_dots( opt->string_value );
#endif
  len = strlen( path );
  mas_free( opt->string_value );
  if ( path[0] && path[1] && path[len - 1] == '/' )
    path[len - 1] = 0;
  opt->string_value = path;

  rC( mas_tree( opt->string_value, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout, pdufnx_data->levinfo_flags, &pdufnx_data->mysql ) );

  rRET;
}

void
dufnx_config_mysql( mucs_option_interface_t * interface, mas_dufnx_data_t * pdufnx_data )
{
  mucs_option_t _uUu_ options_mysql[] = {
    {.name = "mysql-server",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.server,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-user",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.user,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-password",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.password,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-db",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.db,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-port",.shortn = '\0',.restype = MUCS_RTYP_UINT,.cust_ptr = &pdufnx_data->mysql.port,.flags = MUCS_FLAG_AUTOFREE},
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_config_option_interface_tabnode_add( interface, "mysql-table", options_mysql );
}

mucs_option_interface_t *
dufnx_config_interface( mas_dufnx_data_t * pdufnx_data )
{
  mucs_option_t options[] = {
    {.name = "treedb",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags,
     .def_nvalue.v_ulong = MASXFS_CB_MODE_DB,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE},
    {.name = "treefs",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags,
     .def_nvalue.v_ulong = MASXFS_CB_MODE_FS,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE},
    {.name = MUCS_NONOPT_NAME,.restype = MUCS_RTYP_TARG,.flags = MUCS_FLAG_AUTOFREE,.cust_ptr = &pdufnx_data->targv,.callback = arg_process},
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_option_interface_t *interface = mucs_config_option_interface_create_setup( "main-table", options, TRUE );

  return interface;
}

int
dufnx( int argc, char *argv[] )
{
  rDECLGOOD;
  mas_dufnx_data_t dufnx_data = { 0 };

  mucs_option_interface_t *interface = dufnx_config_interface( &dufnx_data );

  dufnx_config_mysql( interface, &dufnx_data );

/* mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL ); */
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, 0, "MAS_DUFNX" );
/* mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, 0, NULL ); */
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, argc, argv );

  rC( mucs_option_interface_lookup_all( interface, &dufnx_data ) );

  mucs_config_option_interface_delete( interface );
  interface = NULL;

  rRET;
}

int
main( int argc, char *argv[] )
{
  int r = 0;

  r = dufnx( argc, argv );
  return r;
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2011 ) ) );
static void
constructor_main( void )
{
  masregerrs_set_max_print( 15 );
  INFO( "START" );
}
