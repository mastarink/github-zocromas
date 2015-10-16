#ifndef MAS_DUF_TRACEN_DEFS_PRESET_H
#  define MAS_DUF_TRACEN_DEFS_PRESET_H

#  ifndef MAST_OUTPUT_LEVEL
#    define MAST_OUTPUT_LEVEL 0
#  endif



#  ifndef MAST_TRACE_CONFIG
#    include "duf_config4trace_ref.h"
#    define MAST_TRACE_CONFIG duf_config4trace
#  endif


#  ifndef MAST_TRACE_AT_CFG
#    define MAST_TRACE_AT_CFG  opt.trace
#  endif

#  ifndef MAST_DBG_AT_CFG
#    define MAST_DBG_AT_CFG  opt.dbg
#  endif



#  ifndef MAST_TRACE_LOADTIME_C
#    define MAST_TRACE_LOADTIME_C( _cfg )  ( _cfg ? _cfg->loadtime : 0)
#  endif

#  ifndef MAST_TRACE_LOADTIME
#    define MAST_TRACE_LOADTIME  MAST_TRACE_LOADTIME_C(duf_get_config(  ))
#  endif


#  ifndef MAST_TRACE_FILE_C
#    define MAST_TRACE_FILE_C( _cfg ) ( _cfg && _cfg->MAST_TRACE_AT_CFG.output.out ? _cfg->MAST_TRACE_AT_CFG.output.out : stdout )
#  endif

#  ifndef MAST_TRACE_FILE
#    define MAST_TRACE_FILE         MAST_TRACE_FILE_C(duf_get_config(  ))
#  endif


#  ifndef MAST_TRACE_FUNWIDTH
#    define MAST_TRACE_FUNWIDTH( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 )
#  endif


#endif

