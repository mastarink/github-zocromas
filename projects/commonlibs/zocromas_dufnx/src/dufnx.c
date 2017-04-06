#define R_GOOD(_r) ((_r)>=0)
#include <stdio.h>

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
static int _uUu_
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
/* tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
  fprintf( fil, "%4d\t%s[%-10ld %10ld]  %s\n", numline_treecb, prefix ? prefix : "", inode, size, ename ? ename : "" /*, epath ? epath : "" */  );
#else
  fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", numline_treecb, prefix ? prefix : "", size, fd, ( long ) depth, inode,
           ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  return 0;
}

int
tree( const char *real_path, masxfs_depth_t maxdepth, FILE * fil, masxfs_levinfo_flags_t inflags )
{
  rDECLGOOD;

  masxfs_entry_callback_t callbacks[] = {
    {treecb,.flags = MASXFS_CB_NAME | /* MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ },
    {NULL}
  };
  WARN( "******** start *******" );

  {
  /* mas_qstd_t *qstd = */ mas_qstd_instance_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );
  /* if ( qstd->pfs ) */
    {
    /* const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest"; */
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );

      {
        masxfs_levinfo_flags_t walkflags _uUu_ = MASXFS_CB_RECURSIVE | MASXFS_CB_STAT | MASXFS_CB_SINGLE_CB;
        masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;
        masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT;

      /* masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP; */
        masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF;

        walkflags |= inflags;
        numline_treecb = 0;
        rC( masxfs_pathinfo_scan_cbs( pi, typeflags, callbacks, fil /* data */ , walkflags | xflags2,
                                      maxdepth ) );
      }
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    }
    mas_qstd_instance_delete(  );
  }
  WARN( "******** end *******" );
  rRET;
}

int
dufnx( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  rDECLGOOD;
  mucs_flags_t work_opt_flags = 0;
  long app_flags = 0x00ffff1111111111L;

  INFO( "dufnx" );
  mucs_option_t options[] = {
    {"xor", 'X', MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR, &app_flags,.def_string_value = "0xfffe0101",.val = 0, "app_flags",.argdesc = "value",.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {.name = "treedb",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &work_opt_flags,.def_nvalue.v_ulong =
     MASXFS_CB_MODE_DB,.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_NVALUE},
    {.name = "treefs",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &work_opt_flags,.def_nvalue.v_ulong =
     MASXFS_CB_MODE_FS,.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_NVALUE},
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };

  mucs_option_interface_t *interface = mucs_config_option_interface_create_setup( "test-table", options, TRUE );

  mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, 0, "MAS_TEST_ENV" );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, 0, NULL );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, argc, argv );
  mucs_option_interface_lookup_all( interface );

  /*******/
  {
    const char *real_path _uUu_ = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_dufnx/mastest";
    FILE *fil _uUu_ = stdout;

#if 0
    {
      char *p = mas_normalize_path( path );

      WARN( "p:%s", p );
      mas_free( p );
    }
#endif
    WARN( "(%d) %lx: %d %d", rCODE, work_opt_flags, ( work_opt_flags & MASXFS_CB_MODE_FS ) ? 1 : 0, ( work_opt_flags & MASXFS_CB_MODE_DB ) ? 1 : 0 );
  /* rC( tree( real_path, ( masxfs_depth_t ) 0 (* maxdepth OR 0 for all *) , fil, MASXFS_CB_MODE_FS ) ); */
  /* rC( tree( real_path, ( masxfs_depth_t ) 0 (* maxdepth OR 0 for all *) , fil, MASXFS_CB_MODE_DB ) ); */
  }
  /*******/

  mucs_config_option_interface_delete( interface );
  interface = NULL;

  return 0;
}

int
main( int argc, char *argv[] )
{
  return dufnx( argc, argv );
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2011 ) ) );
static void
constructor_main( void )
{
  masregerrs_set_max_print( 15 );
  INFO( "START" );
}
