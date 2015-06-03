#ifndef MAS_DUF_OPTION_DEFS_H
#  define MAS_DUF_OPTION_DEFS_H


#  define DUF_OPTION(_lo)          duf_config->_lo
#  define DUF_OPTION_F(_lo)        DUF_OPTION(_lo)
#  define DUF_OPTION_N(_lo)        DUF_OPTION(_lo)
#  define DUF_OPTION_A(_lo, _a)    DUF_OPTION(_lo)._a


#  define DUF_OPTION_FLAGG(_lo, _pref, _fls) DUF_OPTION_F(_pref _fls._lo)
#  define DUF_OPTION_FLAG(_lo, _ref, _pref) DUF_OPTION_FLAGG(_lo, _pref, _ref v.flag)

#  define DUF_CLI_FLAG(_lo)	DUF_OPTION_FLAG(_lo, ., cli)
#  define DUF_ACT_FLAG(_lo)	DUF_OPTION_FLAG(_lo, ., cli.act)
#  define DUF_FORMAT_FLAG(_lo)	DUF_OPTION_FLAG(_lo, ., cli.bformat)
#  define DUF_U_FLAG(_lo)	DUF_OPTION_FLAG(_lo, ->, pu)

#  define DUF_CLI_NUM( _lo, _n ) DUF_OPTION_NUM( _lo, cli._n )
#  define DUF_ACT_NUM(_lo)      DUF_CLI_NUM( _lo, act )
#  define DUF_FORMAT_NUM(_lo)   DUF_CLI_NUM( _lo, bformat )

#  define DUF_FORMAT_FLAG_NUM(_lo)   DUF_CLI_NUM( _lo, bformat.v.flag )

#  define DUF_U_NUM(_lo)        DUF_OPTION_NUM( _lo, u )
#  define DUF_TRACE_NUM(_lo)    DUF_CLI_NUM( _lo, trace )

#  define DUF_OPTION_NUM(_lo, _pref) DUF_OPTION_N(_pref._lo)

#if 0
#define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) \
	  ( _istage == -1 || ( !_extended->use_stage || ( _extended->stage.min <= _istage && _extended->stage.max >= _istage ) ) \
       || ( _xtable && ( !_xtable->use_stage || ( _xtable->stage.min <= _istage && _xtable->stage.max >= _istage ) ) ) || _extended->stage.flag )
#else
#define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) duf_check_stage( _istage, _extended, _xtable)
#endif

#endif

/*
vi: ft=c
*/
