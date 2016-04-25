#include <assert.h>
#include <string.h>
#include <stdio.h>                                                   /* FILE */
#include <errno.h>

/* #include <mastar/tools/mas_tools.h>                                  (* mas_tstrflocaltime *) */
/* #include <mastar/tools/mas_utils_path.h>                             (* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ *) */

#include "duf_config.h"                                              /* duf_get_config ✗ */

#include "duf_config_opt_types.h"
#include "duf_config_output_structs.h"
#include "duf_cfg_output_util.h"
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* ###################################################################### */
#include "duf_config_output_util.h"
/* ###################################################################### */

const duf_sformats_t *
mas_get_config_output_sformat( void )
{
  const duf_config_output_t *cfgout = NULL;
  const duf_sformats_t *sf = NULL;

  cfgout = duf_get_config_output(  );
  sf = cfgout ? &cfgout->sformat : NULL;
  return sf;
}

const char *
mas_get_config_output_sformat_list( void )
{
  const char *sformat = NULL;
  const duf_sformats_t *sf;

  sf = mas_get_config_output_sformat(  );
  sformat = sf->files_list;
  return sformat;
}

const char *
mas_get_config_output_sformat_tree( void )
{
  const char *sformat = NULL;
  const duf_sformats_t *sf;

  sf = mas_get_config_output_sformat(  );
  sformat = sf->files_tree;
  return sformat;
}

const char *
mas_get_config_output_sformat_gen( void )
{
  const char *sformat = NULL;
  const duf_sformats_t *sf;

  sf = mas_get_config_output_sformat(  );
  sformat = sf->files_gen;
  return sformat;
}

const duf_asformats_t *
mas_get_config_output_asformats( void )
{
  const duf_config_output_t *cfgout = NULL;
  const duf_asformats_t *asf = NULL;

  cfgout = duf_get_config_output(  );
  asf = cfgout ? &cfgout->as_formats : NULL;
  return asf;
}

const duf_filedirformat_t *
mas_get_config_output_formats( void )
{
  const duf_config_output_t *cfgout = NULL;
  const duf_filedirformat_t *f = NULL;

  cfgout = duf_get_config_output(  );
  f = cfgout ? cfgout->formats : NULL;
  return f;
}

const duf_filedirformat_t *
mas_get_config_output_format( unsigned id )
{
  const duf_config_output_t *cfgout = NULL;
  const duf_filedirformat_t *f = NULL;

  cfgout = duf_get_config_output(  );

  f = cfgout && id < ( sizeof( cfgout->formats ) / sizeof( cfgout->formats[0] ) ) ? &cfgout->formats[id] : NULL;

  return f;
}

const duf_filedirformat_t *
mas_get_config_output_asformat_list( void )
{
  const duf_asformats_t *asf;

  asf = mas_get_config_output_asformats(  );
  {
    const duf_filedirformat_t *f;

    f = mas_get_config_output_format( DUF_FORMAT_NAME_ID_LIST );
    assert( f == &asf->list );
  }
  return asf ? &asf->list : NULL;
}

const duf_filedirformat_t *
mas_get_config_output_asformat_tree( void )
{
  const duf_asformats_t *asf;

  asf = mas_get_config_output_asformats(  );
  {
    const duf_filedirformat_t *f;

    f = mas_get_config_output_format( DUF_FORMAT_NAME_ID_TREE );
    assert( f == &asf->tree );
  }
  return asf ? &asf->tree : NULL;
}

const duf_filedirformat_t *
mas_get_config_output_asformat_gen( void )
{
  const duf_asformats_t *asf;

  asf = mas_get_config_output_asformats(  );
  {
    const duf_filedirformat_t *f;

    f = mas_get_config_output_format( DUF_FORMAT_NAME_ID_GEN );
    assert( f == &asf->gen );
  }
  return asf ? &asf->gen : NULL;
}

unsigned
mas_get_config_output_max_width( void )
{
  const duf_config_output_t *cfgout = NULL;

  cfgout = duf_get_config_output(  );
  return cfgout ? cfgout->max_width : 0;
}

int
mas_output_level( void )
{
  return mas_output_level_c( duf_get_config_output(  ) );
}

FILE *
mas_output_file( void )
{
  return mas_output_file_c( duf_get_config_output(  ) );
}

int
mas_output_force_color( void )
{
  return mas_output_force_color_c( duf_get_config_output(  ) );
}

int
mas_output_nocolor( void )
{
  return mas_output_nocolor_c( duf_get_config_output(  ) );
}

int
duf_output_progress( void )
{
  return duf_output_progress_c( duf_get_config_output(  ) );
}

#if 0
int
duf_output_use_binformat( void )
{
  return duf_output_use_binformat_c( duf_get_config_output(  ) );
}
#endif
