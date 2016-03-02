#ifndef MAS_MUC_TRACEN_DEFS_PRESET_H
# define MAS_MUC_TRACEN_DEFS_PRESET_H

/* # include "muc_config_trace_types.h" */
/* # include "muc_config_trace_enum.h" */

# ifndef MAST_TRACE_CONFIG
/* #    include "..._config4trace_ref.h" */
/* #    define MAST_TRACE_CONFIG ..._config4trace */
/* #    define MAST_TRACE_CONFIG ..._get_config() */
#  define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
# endif

# ifndef MAST_TRACE_AT_TCFG_CR
/* #    define MAST_TRACE_AT_TCFG_CR(_tcfg)  (&_tcfg->opt.trace) */
#  define MAST_TRACE_AT_TCFG_CR(_tcfg) _tcfg
# endif

# ifndef MAST_TRACE_AT_TCFG_
/* #    define MAST_TRACE_AT_TCFG_  opt.trace. */
#  define MAST_TRACE_AT_TCFG_
# endif


# ifndef MAST_TRACE_INDEX
#  define MAST_TRACE_INDEX(_name) MUC_TRACE_LEVEL_ ## _name
# endif

#endif
