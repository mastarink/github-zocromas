#ifndef MAS_DUF_OPTION_DEFS_H
#  define MAS_DUF_OPTION_DEFS_H

#  include "duf_config_ref.h"
/* */
#  ifdef MAS_TRACING
#    define DUF_OPTIONG(_lo)          (DUF_TRACE(flags, 0, "FLAG %s", # _lo), DUF_CONFIGG(_lo))
#  else
#    define DUF_OPTIONG(_lo)          DUF_CONFIGG(_lo)
#  endif
#  define DUF_OPTIONG_F(_lo)        DUF_OPTIONG(_lo)
#  define DUF_OPTIONG_N(_lo)        DUF_OPTIONG(_lo)
#  define DUF_OPTIONG_A(_lo, _a)    DUF_OPTIONG(_lo)._a

#  define DUF_OPTIONA(_lo)          DUF_CONFIGA(_lo)
#  define DUF_OPTIONA_F(_lo)        DUF_OPTIONA(_lo)
#  define DUF_OPTIONA_N(_lo)        DUF_OPTIONA(_lo)
#  define DUF_OPTIONA_A(_lo, _a)    DUF_OPTIONA(_lo)._a

#  define DUF_OPTIONW(_lo)          DUF_CONFIGW(_lo)
#  define DUF_OPTIONW_F(_lo)        DUF_OPTIONW(_lo)
#  define DUF_OPTIONW_N(_lo)        DUF_OPTIONW(_lo)
#  define DUF_OPTIONW_A(_lo, _a)    DUF_OPTIONW(_lo)._a



/* */
#  define DUF_OPTIONG_FLAGG(_lo, _pref, _fls) DUF_OPTIONG_F(_pref _fls._lo)
#  define DUF_OPTIONG_FLAG(_lo, _ref, _pref) DUF_OPTIONG_FLAGG(_lo, _pref, _ref v.flag)

#  define DUF_OPTIONW_FLAGG(_lo, _pref, _fls) DUF_OPTIONW_F(_pref _fls._lo)
#  define DUF_OPTIONW_FLAG(_lo, _ref, _pref) DUF_OPTIONW_FLAGG(_lo, _pref, _ref v.flag)
/* */
#  define DUF_CLIW_FLAG(_lo)	DUF_OPTIONW_FLAG(_lo, ., cli)
#  define DUF_ACTW_FLAG(_lo)	DUF_OPTIONW_FLAG(_lo, ., cli.act)
#  define DUF_FORMATW_FLAG(_lo)	DUF_OPTIONW_FLAG(_lo, ., cli.bformat)
#  define DUF_UW_FLAG(_lo)	DUF_OPTIONW_FLAG(_lo, ->, puz)

#  define DUF_CLIG_FLAG(_lo)	DUF_OPTIONG_FLAG(_lo, ., cli)
#  define DUF_ACTG_FLAG(_lo)	DUF_OPTIONG_FLAG(_lo, ., cli.act)
#  define DUF_FORMATG_FLAG(_lo)	DUF_OPTIONG_FLAG(_lo, ., cli.bformat)
#  define DUF_UG_FLAG(_lo)	DUF_OPTIONG_FLAG(_lo, ->, puz)



/* */
#  define DUF_CLIW_NUM( _lo, _n ) DUF_OPTIONW_NUM( _lo, cli._n )
#  define DUF_ACTW_NUM(_lo)      DUF_CLIW_NUM( _lo, act )
#  define DUF_FORMATW_NUM(_lo)   DUF_CLIW_NUM( _lo, bformat )

#  define DUF_CLIG_NUM( _lo, _n ) DUF_OPTIONG_NUM( _lo, cli._n )
#  define DUF_ACTG_NUM(_lo)      DUF_CLIG_NUM( _lo, act )
#  define DUF_FORMATG_NUM(_lo)   DUF_CLIG_NUM( _lo, bformat )

/* */
#  define DUF_FORMATW_FLAG_NUM(_lo)   DUF_CLIW_NUM( _lo, bformat.v.flag )
#  define DUF_FORMATG_FLAG_NUM(_lo)   DUF_CLIG_NUM( _lo, bformat.v.flag )

#  define DUF_UG_NUM(_lo)        DUF_OPTIONG_NUM( _lo, u )
#  define DUF_UW_NUM(_lo)        DUF_OPTIONW_NUM( _lo, u )

#  define DUF_TRACE_NUM(_lo)    DUF_CLIW_NUM( _lo, trace )

#  define DUF_OPTIONG_NUM(_lo, _pref) DUF_OPTIONG_N(_pref._lo)
#  define DUF_OPTIONW_NUM(_lo, _pref) DUF_OPTIONW_N(_pref._lo)

#  if 0
#    define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) \
	  ( _istage == -1 || ( !_extended->use_stage || ( _extended->stage.min <= _istage && _extended->stage.max >= _istage ) ) \
       || ( _xtable && ( !_xtable->use_stage || ( _xtable->stage.min <= _istage && _xtable->stage.max >= _istage ) ) ) || _extended->stage.flag )
#  else
#    define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) duf_check_stage( _istage, _extended, _xtable)
#  endif

#endif

/*
vi: ft=c
*/
