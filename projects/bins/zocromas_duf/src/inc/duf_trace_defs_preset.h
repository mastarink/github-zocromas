#ifndef MAS_DUF_TRACE_DEFS_PRESET_H
#  define MAS_DUF_TRACE_DEFS_PRESET_H

#  ifndef DUF_OUTPUT_LEVEL
#    define DUF_OUTPUT_LEVEL 0
#  endif


#  ifndef DUF_TRACE_CONFIG
#    include "duf_config4trace_ref.h"
#    define DUF_TRACE_CONFIG duf_config4trace
#  endif

#  ifndef DUF_TRACE_AT_CFG
#    define DUF_TRACE_AT_CFG  opt.trace
#  endif
#  ifndef DUF_DBG_AT_CFG
#    define DUF_DBG_AT_CFG  opt.dbg
#  endif


#  ifndef DUF_TRACE_LOADTIME_C
#    define DUF_TRACE_LOADTIME_C( _cfg )  ( _cfg ? _cfg->loadtime : 0)
#  endif
#  ifndef DUF_TRACE_LOADTIME
#    define DUF_TRACE_LOADTIME  DUF_TRACE_LOADTIME_C(duf_get_config(  ))
#  endif

#  ifndef DUF_TRACE_FILE_C
#    define DUF_TRACE_FILE_C( _cfg ) ( _cfg && _cfg->DUF_TRACE_AT_CFG.output.out ? _cfg->DUF_TRACE_AT_CFG.output.out : stdout )
#  endif
#  ifndef DUF_TRACE_FILE
#    define DUF_TRACE_FILE         DUF_TRACE_FILE_C(duf_get_config(  ))
#  endif

#  ifndef DUF_TRACE_FUNWIDTH
#    define DUF_TRACE_FUNWIDTH( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 )
#  endif

#endif
