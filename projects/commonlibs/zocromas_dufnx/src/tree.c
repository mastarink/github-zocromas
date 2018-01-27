#define R_GOOD(_r) ((_r)>=0)
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>

#include <mastar/minierr/minierr.h>
/* #include <mastar/regerr/masregerr.h> */
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>
#include <mastar/masxfs/masxfs_pathinfo_ref.h>

#include <mastar/levinfo/masxfs_levinfo_tools.h>
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo_ref_xstat.h>

#include <mastar/levinfo/masxfs_levinfo_path.h>
#include <mastar/levinfo/masxfs_levinfo_digest.h>
#include <mastar/levinfo/masxfs_levinfo_db.h>

#include <mastar/levinfo/masxfs_levinfo_digest_enums.h>

#include <mastar/qstd/qstd_query.h>
#include <mastar/qstd/qstd_mstmt_base.h>
#include <mastar/qstd/qstd_mstmt.h>

#include "structs.h"
#include "mysql.h"

#include "tree.h"

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
  rC( masxfs_levinfo_db_store( li, flags ) );
  rRET;
}

/* of type: masxfs_scan_fun_simple_t */
static int _uUu_
treecb( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *userdata, unsigned long serial, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) userdata;
  static masxfs_depth_t top_depth = 0;

  size_t size = masxfs_levinfo_size_ref( li, flags );

/*int fd _uUu_ = masxfs_levinfo_fd_ref( li, flags );*/
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  unsigned long nsamedigest = masxfs_levinfo_nsamedigest_ref( li, flags );

  if ( !serial && depth )
    top_depth = depth - 1;

  {
    /**
    const masxfs_digests_t *digests = li->digests;
    const unsigned char *dg = NULL;

    int sz =                                                         // masxfs_digests_getn( li->digests, 0, NULL ); //
            masxfs_digests_get( digests, MASXFS_DIGEST_SHA1, &dg );

    char sbuffer[512] = { 0 };
    char *psbuf = sbuffer;

    for ( ssize_t i = 0; i < sz; i++ )
    {
      snprintf( psbuf, sizeof( sbuffer ) - ( psbuf - sbuffer ), "%02x", dg[i] );
      psbuf += 2;
    } **/
#if 1
    char hh[32] = "";

    memset( hh, 0, sizeof( hh ) );
    if ( nsamedigest == 0 )
      hh[0] = 0;
    else if ( nsamedigest > 31 )
      sprintf( hh, "# %lu", nsamedigest );
    else
      for ( unsigned long i = 0; i < nsamedigest; i++ )
        hh[i] = '+';
/* /usr/bin/tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
    const char *epath _uUu_ = masxfs_levinfo_path_ref( li, flags );
    const char *name = epath && li->detype != MASXFS_ENTRY_DIR_NUM ? epath : ( ename ? ename : "" );
    char lab[128] = "";

    const char *treeprefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", top_depth + 1, flags );

    if ( li->detype == MASXFS_ENTRY_DIR_NUM )
    {

      fprintf( fil, "%4ld %s[", serial, treeprefix ? treeprefix : "" );
      if ( masxfs_levinfo_has_stat( li, flags ) )
        fprintf( fil, "%-10ld ", inode );
      snprintf( lab, sizeof( lab ), "DIR =%ld]", li->leaf_count );
      fprintf( fil, "%-10s%-s", lab, name );
    }
    else
    {
      fprintf( fil, "%4ld %s", serial, treeprefix ? treeprefix : "" );
      if ( masxfs_levinfo_has_stat( li, flags ) )
        fprintf( fil, "[%-10ld %10ld]", inode, size );
      fprintf( fil, "%-35s", name );
    }
    if ( flags & MASXFS_CB_DIGESTS )
    {
      const char *digest = masxfs_levinfo_hexdigest_ref( li, flags );

      if ( digest )
        fprintf( fil, "## %-40s", digest ? digest : "" );
      if ( nsamedigest )
        fprintf( fil, " \t%s", hh );
    }
    fprintf( fil, "\n" );
//  fprintf(fil, "#(%5ld) %s\n", li->leaf_count, li->name);
#else
    fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", serial, treeprefix ? treeprefix : "", size, fd, ( long ) depth, inode,
             ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  }
  return 0;
}

static int _uUu_
treestatcb( const char *ename, struct stat *st, void *userdata, unsigned depth _uUu_, unsigned long serial, const char *treeprefix,
            masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) userdata;

/* /usr/bin/tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
  fprintf( fil, "%4ld\t%s[%-10ld %10ld]  %-35s\n", serial, treeprefix ? treeprefix : "", st->st_ino, st->st_size,
           ename ? ename : "" /*, epath ? epath : "" */  );
  return 0;
}

static int
dufnx_tree( const char *real_path, masxfs_entry_filter_t * entry_pfilter, FILE * fil, masxfs_levinfo_flags_t inflags, mas_dufnx_mysql_data_t * mysql )
{
  rDECLGOOD;

  masxfs_levinfo_flags_t walkflags = 0 /* | MASXFS_CB_RECURSIVE | MASXFS_CB_STAT | MASXFS_CB_SINGLE_CB */  | inflags;

  masxfs_entry_callback_t _uUu_ callbacks1[] = { {.fun_counter = 0} };
  masxfs_entry_callback_t callbacks[] = {
    {.cb_type = MASXFS_CBTYPE_SIMPLE,.fun_simple = treecb,
     .flags = /* MASXFS_CB_OFF_NAME | MASXFS_CB_PATH |  MASXFS_CB_PREFIX | */ MASXFS_CB_TRAILINGSLASH /*| MASXFS_CB_STAT | MASXFS_CB_FD */ ,
   /*.entry_filter =   {.maxdepth = 0} */ },
    {.cb_type = MASXFS_CBTYPE_STAT,.fun_stat = treestatcb,
     .flags = /*MASXFS_CB_PREFIX | */ MASXFS_CB_TRAILINGSLASH /* | MASXFS_CB_STAT ,.entry_filter = {.maxdepth = 0} */ },
    {0}
  };
  dufnx_qstd( mysql );
  masxfs_pathinfo_t *pi = NULL;

#if 0
  pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );
  masxfs_pathinfo_open( pi, walkflags );
  WARN( "pidepth:%d", masxfs_pathinfo_pidepth( pi ) );
  for ( masxfs_depth_t i = 0; i < masxfs_pathinfo_pidepth( pi ); i++ )
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_li( pi, i );

    WARN( "node_id:%ld %s", masxfs_levinfo_node_id_val( li ), masxfs_levinfo_detype2s( masxfs_levinfo_detype( li, walkflags ) ) );
  }
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
  pi - NULL;
#endif

  pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );

/* masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT; */
/* masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP; */

  masxfs_levinfo_flags_t xflags2 = 0 /* | MASXFS_CB_USE_STAT_CB | MASXFS_CB_USE_SIMPLE_CB */ ;

#if 0
  masxfs_scanner_t scanner = {
    .entry_pfilter = {
                      .typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR,.maxdepth = maxdepth /* */
                      }
    ,.cbs = cbs,.flags = flags,.maxdepth = maxdepth
  };
  masxfs_pathinfo_scanf_scanner( pi, &scanner, userdata );

#else
  masxfs_entry_filter_t entry_filter = *entry_pfilter;

  entry_filter.typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;
  rC( masxfs_pathinfo_scanf_cbs( pi, &entry_filter, callbacks, fil /* userdata */ , walkflags | xflags2 ) );
#endif
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
  rRET;
}

int
dufnx_data_path2db( const char *path, int npos, void *userdata, void *extradata _uUu_ )
{
  rDECLGOOD;
  if ( npos > 0 )
  {
    mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;

    masxfs_entry_filter_t entry_filter = {
      .typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR /*, .maxdepth=pdufnx_data->max_depth */
    };

    masxfs_entry_callback_t callback = {
      .cb_type = MASXFS_CBTYPE_SIMPLE,.fun_simple = fillcb,
    /* .flags = MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT | MASXFS_CB_DIGESTS | MASXFS_CB_PATH (* | MASXFS_CB_NO_FD *) */
      .flags = MASXFS_CB_STAT | MASXFS_CB_DIGESTS
    };
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 /* depth limit */ , 0 );

    {
      mas_qstd_t *qstd = dufnx_qstd( &pdufnx_data->mysql );

      rC( mas_qstd_start_transaction( qstd ) );
      {
        masxfs_levinfo_flags_t flagsfs = MASXFS_CB_SINGLE_CB /*  MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS | MASXFS_CB_USE_SIMPLE_CB */ ;
        masxfs_levinfo_flags_t xflags1 = MASXFS_CB_UP_ROOT;
        masxfs_levinfo_flags_t cflags = pdufnx_data->levinfo_flags;

        cflags = cflags & ~( MASXFS_CB_SKIP_EMPTY );                 // No for fill

        WARN( "(%d) ******** fill scan ******* %lx : %lx", rCODE, flagsfs | xflags1, pdufnx_data->levinfo_flags | flagsfs | xflags1 );
        rC( masxfs_pathinfo_scanf_cbs( pi, &entry_filter, &callback, qstd, cflags | flagsfs | xflags1 ) );
        WARN( "******** /fill scan *******" );
      }
      rC( mas_qstd_end_transaction( qstd ) );
/* TODO commit !! */
      rC( mas_qstd_update_summary( qstd ) );
    }
    masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
  }
  rRET;
}

int
dufnx_data_tree( const char *path, int npos, void *userdata, void *extradata _uUu_ )
{
  rDECLGOOD;
  if ( npos > 0 )
  {
    char *real_path;
    mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;

    real_path = mas_normalize_path_cwd_dots( path, FALSE );
    if ( pdufnx_data->dufnx_flags & MASDUFNX_COLLECT )
    {
      rC( dufnx_data_path2db( real_path, npos, userdata, extradata ) );
      WARN( "COLLECTED" );
    }
  /* if ( pdufnx_data->levinfo_flags & ( MASXFS_CB_MODE_DB | MASXFS_CB_MODE_FS ) ) */
    if ( pdufnx_data->dufnx_flags & MASDUFNX_LIST )
    {
      FILE *save = stdout;
      FILE *ofile = NULL;

      if ( pdufnx_data->savefname )
      {
        if ( 0 == strcmp( pdufnx_data->savefname, "-" ) )
        {
          save = stdout;
        }
        else
        {
          struct stat st;
          int r = stat( pdufnx_data->savefname, &st );

          if ( r < 0 && errno == ENOENT )
          {
            save = ofile = fopen( pdufnx_data->savefname, "w" );
          }
        }
      }
      rC( dufnx_tree( real_path, &pdufnx_data->entry_filter, save, pdufnx_data->levinfo_flags, &pdufnx_data->mysql ) );
      if ( ofile )
      {
        fclose( ofile );
        ofile = save = NULL;
      }
    }
    mas_free( real_path );
  }
  rRET;
}
