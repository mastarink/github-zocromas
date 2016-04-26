/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_types.h"                                           /* duf_depthinfo_t ✗ */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_format_structs.h"
#include "duf_ufilter_ref.h"

/* ###################################################################### */
#include "duf_sccbh.h"
/* ###################################################################### */

SRX( SCCBH, const char *, sformat, NULL, get_item_format, const duf_sccb_handle_t * sccbh, duf_format_name_ids_t id, int dirs, int *puse_format_once )
{
/* const char *sformat = NULL; */
  const char *fmtname = NULL;

  fmtname = CRX( ufilter_format_name, CRX( pdi_pu, H_PDI ), id /* DUF_FORMAT_NAME_ID_LIST */  );
#if 0
  {
    const char *fmtname1 = NULL;

    fmtname1 = CRX( ufilter_list_format_name, CRX( pdi_pu, H_PDI ) );
    assert( ( fmtname == NULL && fmtname1 == NULL ) || 0 == strcmp( fmtname, fmtname1 ) );
  }
#endif
  {
    const duf_filedirformat_t *fmt;
    const mas_argvc_t *fset;

    fmt = mas_get_config_output_format( id /* DUF_FORMAT_NAME_ID_LIST */  );
    fset = dirs ? &fmt->dirs : &fmt->files;
#if 0
    {
      const duf_filedirformat_t *fmt1;

      fmt1 = mas_get_config_output_asformat_list(  );
      assert( fmt == fmt1 );
    }
#endif

    for ( int nfmt = 0; !sformat && fmtname && nfmt < fset->argc; nfmt++ )
    {
      const char *f;

      f = fset->argv[nfmt];
      if ( f && *f == '`' )
      {
        const char *e;

        f++;
        e = strchr( f, '`' );
        if ( e && 0 == strncmp( fmtname, f, e - f ) )
          sformat = e + 1;
      }
    }
    {
      int use;

      use = puse_format_once && *puse_format_once ? *puse_format_once - 1 : CRX( ufilter_use_format, CRX( pdi_pu, H_PDI ) ) - 1;
      if ( !sformat && use >= 0 && use < fmt->files.argc )
        sformat = fmt->files.argv[use];
    }
  }
  if ( !sformat )
    sformat = mas_get_config_output_sformat_gen(  );
/* if ( !sformat )                                     */
/*   sformat = mas_get_config_output_sformat_list(  ); */
  if ( !sformat )
    sformat = " _%M  =%S %8s%f\n";
  if ( puse_format_once )
    *puse_format_once = 0;
/* return sformat; */
  ERX( SCCBH, const char *, sformat, NULL, get_item_format, const duf_sccb_handle_t * sccbh, duf_format_name_ids_t id, int dirs,
       int *puse_format_once );
}
