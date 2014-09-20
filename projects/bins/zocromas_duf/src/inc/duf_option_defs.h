#ifndef MAS_DUF_OPTION_DEFS_H
#  define MAS_DUF_OPTION_DEFS_H


#  define DUF_OPTION(_lo)          duf_config->_lo
#  define DUF_OPTION_F(_lo)        DUF_OPTION(_lo)
#  define DUF_OPTION_N(_lo)        DUF_OPTION(_lo)
#  define DUF_OPTION_A(_lo, _a)    DUF_OPTION(_lo)._a


#  define DUF_OPTION_FLAGG(_lo, _pref, _fls) DUF_OPTION_F(_pref _fls._lo)
#  define DUF_OPTION_FLAG(_lo, _ref, _pref) DUF_OPTION_FLAGG(_lo, _pref, _ref v.flag)

#  define DUF_CLI_FLAG(_lo) DUF_OPTION_FLAG(_lo, ., cli)
#  define DUF_ACT_FLAG(_lo) DUF_OPTION_FLAG(_lo, ., cli.act)
#  define DUF_U_FLAG(_lo) DUF_OPTION_FLAG(_lo, ->, pu)

#  define DUF_CLI_NUM( _lo, n ) DUF_OPTION_NUM( _lo, cli.n )
#  define DUF_ACT_NUM(_lo)      DUF_CLI_NUM( _lo, act )
#  define DUF_U_NUM(_lo)        DUF_OPTION_NUM( _lo, u )
#  define DUF_TRACE_NUM(_lo)    DUF_CLI_NUM( _lo, trace )

#  define DUF_OPTION_NUM(_lo, _pref) DUF_OPTION_N(_pref._lo)


#endif
