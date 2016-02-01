#ifndef MAS_DUF_TRACEN_DEFS_PRESET_H
#  define MAS_DUF_TRACEN_DEFS_PRESET_H


#  ifndef MAST_TRACE_CONFIG
/* #    include "duf_config4trace_ref.h" */
/* #    define MAST_TRACE_CONFIG duf_config4trace */
/* #    define MAST_TRACE_CONFIG duf_get_config() */
#    define MAST_TRACE_CONFIG duf_get_trace_config()
#  endif

#  ifndef MAST_TRACE_AT_CFG_CR
/* #    define MAST_TRACE_AT_CFG_CR(_cfg)  (&_cfg->opt.trace) */
#    define MAST_TRACE_AT_CFG_CR(_cfg) _cfg
#  endif

#  ifndef MAST_TRACE_AT_CFG_
/* #    define MAST_TRACE_AT_CFG_  opt.trace. */
#    define MAST_TRACE_AT_CFG_
#  endif

/* #  ifndef MAST_DBG_AT_CFG            */
/* #    define MAST_DBG_AT_CFG  opt.dbg */
/* #  endif                             */



#  ifndef MAST_TRACE_LOADTIME_C
#    define MAST_TRACE_LOADTIME_C( _cfg )  ( _cfg ? _cfg->loadtime : 0)
#  endif

#  ifndef MAST_TRACE_LOADTIME
#    define MAST_TRACE_LOADTIME  MAST_TRACE_LOADTIME_C(duf_get_trace_config(  ))
#  endif

#  ifndef MAST_TRACE_FILE_C
#    define MAST_TRACE_FILE_C( _cfg ) duf_trace_file_c(_cfg)
#  endif

#  ifndef MAST_TRACE_FILE
#    define MAST_TRACE_FILE          duf_trace_file()
#  endif


#  ifndef MAST_TRACE_FUNWIDTH_C
#    define MAST_TRACE_FUNWIDTH_C( _cfg ) ( _cfg?_cfg->MAST_TRACE_AT_CFG_ fun_width : 18 )
#  endif

#  ifndef MAST_TRACE_FORCE_COLOR_C
#    define MAST_TRACE_FORCE_COLOR_C( _cfg ) duf_trace_force_color()
#  endif
#  ifndef MAST_TRACE_NOCOLOR
#    define MAST_TRACE_NOCOLOR_C( _cfg )  duf_trace_nocolor()
#  endif

#  ifndef MAST_TRACE_LEVEL
#    define MAST_TRACE_LEVEL_C( _cfg, _what, _name ) MAST_IF_TRACE_WHAT_C( _cfg, _what, _name )
/* #    define MAST_TRACE_LEVEL_C( _cfg, _what, _name ) 0 */
#  endif




#endif
