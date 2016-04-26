#define DUF_GET_FIELD_FROM_ROW
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"                                  /* mas_output_force_color ; mas_output_nocolor */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */

#include "duf_sccb_structs.h"                                        /* duf_scan_callbacks_s; duf_sccb_data_row_s; duf_scanner_fun_s; ✗ */
#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_pathinfo_ref.h"

#include "duf_print.h"

#include "duf_ufilter_ref.h"
#include "duf_format_structs.h"

/* ########################################################################################## */
#include "duf_mod_types.h"
static DR( MOD, print_leaf2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );
static DR( MOD, pack_leaf1, duf_depthinfo_t * pdi_unused, struct duf_sccb_handle_s *sccbh );

/* ########################################################################################## */
static int use_format_once = 0;
static duf_scan_callbacks_t duf_sccb_dispatch;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"sccb", &duf_sccb_dispatch},
  {NULL, NULL}
};

/* ########################################################################################## */
static duf_scanner_set_t scanners[] = {
  {
   .name = "traditional",
   .flags = DUF_SCANNER_SET_FLAG_DB | DUF_SCANNER_SET_FLAG_DISABLED, /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( print_leaf2 ),                                       /* */
   },
  {
   .name = "packed1",
   .flags = DUF_SCANNER_SET_FLAG_DB | DUF_SCANNER_SET_FLAG_PACK /* | DUF_SCANNER_SET_FLAG_DISABLED */ , /* */
   .type = DUF_NODE_LEAF,                                            /* */
 /* .scanstage = DUF_SCANSTAGE_DB_LEAVES_PACK,                        (* *) */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( pack_leaf1 ),                                        /* */
   },

  {.fun = NULL}
};

static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "listing print",
  .name = "packlist",
  .init_scan = NULL,                                                 /* */
  .no_progress = 1,

  .scanners = scanners,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */

  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
};

/* ########################################################################################## */

static MAS_UNUSED
SR( MOD, pack_leaf1, duf_depthinfo_t * pdi_unused MAS_UNUSED, struct duf_sccb_handle_s *sccbh )
{
  int n = 0;
  const duf_sccb_data_row_t *trow = NULL;

  trow = duf_sccbh_start_first_row( sccbh );
  n = 0;
/* TODO 20160425.120000
 * only for first row : 
 *   additional call with special format
 *     used to print common things for group, for instance size, md5sum, sha1id etc.
 * */
  while ( trow && trow != duf_sccbh_get_last_row( sccbh ) /* last: don't! */  )
  {
    if ( trow && trow->nfields )
    {
      n++;
      const char *path MAS_UNUSED;
      const char *rpath MAS_UNUSED;
      const char *iname MAS_UNUSED;

      path = CRX( pi_path, &trow->pathinfo );
      rpath = CRX( pi_relpath, &trow->pathinfo );
      iname = CRX( pi_itemname, &trow->pathinfo );

      MAST_TRACE( temp, 5, "@@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, path, iname );
      if ( 1 )
        MAST_TRACE( temp, 5, "@@@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, rpath, iname );

      if ( 1 )
      {
        use_format_once = 4;
        CR( print_leaf2, pdi_unused, sccbh );
      }
    }
    trow = duf_sccbh_next_row( sccbh );
  }
  duf_sccbh_end_row( sccbh );

  fprintf( stderr, "\n^^ %d ^^\n\n", n );
  ER( MOD, pack_leaf1, duf_depthinfo_t * pdi_unused, struct duf_sccb_handle_s *sccbh );
}

const char *
duf_get_list_format( const duf_sccb_handle_t * sccbh, int *puse_format_once )
{
  const char *sformat = NULL;
  int use;
  const char *fmtname = NULL;
  const duf_filedirformat_t *fmt;

  use = puse_format_once && *puse_format_once ? *puse_format_once - 1 : CRX( ufilter_use_format, CRX( pdi_pu, H_PDI ) ) - 1;
  fmt = mas_get_config_output_format( DUF_FORMAT_NAME_ID_LIST );
#if 0
  {
    const duf_filedirformat_t *fmt1;

    fmt1 = mas_get_config_output_asformat_list(  );
    assert( fmt == fmt1 );
  }
#endif
  fmtname = CRX( ufilter_format_name, CRX( pdi_pu, H_PDI ), DUF_FORMAT_NAME_ID_LIST );
#if 0
  {
    const char *fmtname1 = NULL;

    fmtname1 = CRX( ufilter_list_format_name, CRX( pdi_pu, H_PDI ) );
    assert( ( fmtname == NULL && fmtname1 == NULL ) || 0 == strcmp( fmtname, fmtname1 ) );
  }
#endif
  for ( int nfmt = 0; !sformat && fmtname && nfmt < fmt->files.argc; nfmt++ )
  {
    const char *f;

    f = fmt->files.argv[nfmt];
    if ( f && *f == '`' )
    {
      const char *e;

      f++;
      e = strchr( f, '`' );
      if ( e && 0 == strncmp( fmtname, f, e - f ) )
        sformat = e + 1;
    }
  }
  if ( !sformat && use >= 0 && use < fmt->files.argc )
    sformat = fmt->files.argv[use];
  if ( !sformat )
    sformat = mas_get_config_output_sformat_gen(  );
  /* if ( !sformat )                                     */
  /*   sformat = mas_get_config_output_sformat_list(  ); */
  if ( !sformat )
    sformat = " _%M  =%S %8s%f\n";
  if ( puse_format_once )
    *puse_format_once = 0;
  return sformat;
}

static
SR( MOD, print_leaf2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh )
{
  const char *sformat;

  sformat = CRX( get_list_format, sccbh, &use_format_once );
  ( void ) CRX( print_sformat_file_info, H_PDI, 1 /* from row */ , CRX( sccbh_row_current, sccbh ), sccbh, NULL, sformat,
                ( duf_sccb_print_cb_t ) NULL,
                ( duf_sccb_print_cb_t ) NULL, mas_get_config_output_max_width(  ), mas_output_force_color(  ), mas_output_nocolor(  ), NULL, NULL );

  ER( MOD, print_leaf2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );
}
