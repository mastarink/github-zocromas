#ifndef MAS_DUF_TRACEN_DEFS_PRESET_H
# define MAS_DUF_TRACEN_DEFS_PRESET_H

# include "duf_config_trace_types.h"

#  ifndef MAST_TRACE_CONFIG
/* #    include "duf_config4trace_ref.h" */
/* #    define MAST_TRACE_CONFIG duf_config4trace */
/* #    define MAST_TRACE_CONFIG duf_get_config() */
#   define MAST_TRACE_CONFIG duf_get_trace_config()
#  endif

# ifndef MAST_TRACE_AT_TCFG_CR
/* #    define MAST_TRACE_AT_TCFG_CR(_tcfg)  (&_tcfg->opt.trace) */
#  define MAST_TRACE_AT_TCFG_CR(_tcfg) _tcfg
# endif

# ifndef MAST_TRACE_AT_TCFG_
/* #    define MAST_TRACE_AT_TCFG_  opt.trace. */
#  define MAST_TRACE_AT_TCFG_
# endif

/* #  ifndef MAST_DBG_AT_TCFG            */
/* #    define MAST_DBG_AT_TCFG  opt.dbg */
/* #  endif                             */

# ifndef MAST_TRACE_LOADTIME_C
#  define MAST_TRACE_LOADTIME_C( _tcfg )  ( _tcfg ? _tcfg->loadtime : 0)
# endif

# ifndef MAST_TRACE_LOADTIME
#  define MAST_TRACE_LOADTIME  MAST_TRACE_LOADTIME_C(duf_get_trace_config(  ))
# endif

# ifndef MAST_TRACE_FILE_C
#  define MAST_TRACE_FILE_C( _tcfg ) duf_trace_file_c(_tcfg)
# endif

# ifndef MAST_TRACE_FILE
#  define MAST_TRACE_FILE          duf_trace_file()
# endif

# ifndef MAST_TRACE_FUNWIDTH_C
#  define MAST_TRACE_FUNWIDTH_C( _tcfg ) ( _tcfg?_tcfg-> fun_width : 18 )
# endif

# ifndef MAST_TRACE_FORCE_COLOR_C
/* #  define MAST_TRACE_FORCE_COLOR_C( _tcfg ) duf_output_force_color() */
#  define MAST_TRACE_FORCE_COLOR_C( _tcfg ) duf_trace_output_force_color_c(_tcfg)
# endif
# ifndef MAST_TRACE_NOCOLOR
/* #  define MAST_TRACE_NOCOLOR_C( _tcfg )  duf_output_nocolor() */
#  define MAST_TRACE_NOCOLOR_C( _tcfg )  duf_trace_output_no_color_c(_tcfg)
# endif

# ifndef MAST_TRACE_LEVEL
/* #    define MAST_TRACE_LEVEL_C( _tcfg, _what, _name ) MAST_IF_TRACE_WHAT_C( _tcfg, _what, _name ) */
/* #    define MAST_TRACE_LEVEL_C( _tcfg, _what, _name ) (( _tcfg ) ? _tcfg->_what _name : 1 ) */
#  define MAST_TRACE_LEVEL_C( _tcfg, _what, _name ) (( _tcfg ) ? _tcfg-> class_levels[DUF_TRACE_LEVEL_ ## _name] : 1 )
# endif

#endif
