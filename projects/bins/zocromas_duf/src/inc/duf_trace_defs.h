#ifndef MAS_DUF_TRACE_DEFS_H
#  define MAS_DUF_TRACE_DEFS_H

/* #  include "duf_print_defs.h" */
/* #  include "duf_options.h" */
/* #  include "duf_error_types.h" */
/* #  include "duf_errors.h" */
#  include "duf_config4trace_ref.h"
/* ###################################################################### */

#  define DUF_TRACE_CONFIG duf_config4trace
/* #  define DUF_OINV(pref) assert(  !pref opendir || ( \                                                */
/*                           ( ( (int) DUF_TRACE_CONFIG->nopen - (int) DUF_TRACE_CONFIG->nclose ) ) \                */
/*                         - (pref  depth)  == 1 ) \                                                     */
/*                 )                                                                                     */
/*  (* - (pref levinfo?pref levinfo[pref depth].is_leaf:0) *)                                            */
/* #  define DUF_OINVC(pref) assert(  !pref opendir || ( \                                               */
/*                           ( ( (int) DUF_TRACE_CONFIG->nopen - (int) DUF_TRACE_CONFIG->nclose ) ) \                */
/*                         - (pref  depth)  == 0 ) \                                                     */
/*                 )                                                                                     */
/*                                                                                                       */
/* #  define DUF_OINV_OPENED(pref)     \                                                                 */
/*                 assert(  !pref opendir || (pref levinfo && pref levinfo[pref depth].lev_dh.dfd ))     */
/* #  define DUF_OINV_NOT_OPENED(pref) \                                                                 */
/*                 assert(  !pref opendir || (!pref levinfo || pref levinfo[pref depth].lev_dh.dfd==0 )) */

/* ###################################################################### */



/* ###################################################################### */
#  define DUF_IF_TR_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->cli.trace.nonew) ? _cfg->_what._name: 1 )
#  define DUF_IF_TR_WHAT( _what, _name )			DUF_IF_TR_WHAT_C( DUF_TRACE_CONFIG,	_what,		_name )
#  define DUF_IF_TR( _name )					DUF_IF_TR_WHAT(				cli.trace,	_name)

#  define DUF_IF_TR_WHATN_C( _cfg, _what, _name, _min )		( _cfg && !_cfg->cli.trace.nonew && _cfg->_what._name > _min )
#  define DUF_IF_TR_WHATN( _what, _name, _min )			DUF_IF_TR_WHATN_C( DUF_TRACE_CONFIG,	_what,		_name, _min )
#  define DUF_IF_TRN( _name, _min )				DUF_IF_TR_WHATN( cli.trace, _name, _min)

/* #  define DUF_TR_WHAT_C( _cfg, _what, _name, _min, ... )    if ( DUF_IF_TR_WHATN_C( _cfg, _what, _name, _min )) { __VA_ARGS__ ; } */
/* #  define DUF_TR_WHAT( _what, _name, _min, ... )           DUF_TR_WHAT_C( DUF_TRACE_CONFIG, _what, _name, _min, __VA_ARGS__ ) */

/* #  define DUF_TR_WHATSYS(  _what, _name, _min, ...)        DUF_TR_WHATSYSE( errno, _what, _name, _min, __VA_ARGS__ ) */

/* #  define DUF_TR_C( _cfg,_name, ... )                     DUF_TR_WHAT_C( _cfg, cli.trace, _name, __VA_ARGS__ ) */
/* #  define DUF_TR(_name, ...)                             DUF_TR_C( DUF_TRACE_CONFIG, __VA_ARGS__ )                */

/* ###################################################################### */

/* #  define DUF_IF_TRACE_WHAT(_what,_name) (DUF_TRACE_CONFIG && !DUF_TRACE_CONFIG->cli.trace.nonew && DUF_TRACE_CONFIG->_what._name ) */

#  define DUF_IF_TRACE_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->cli.trace.nonew) ? _cfg->_what._name : 1 )

#  define DUF_IF_TRACE_WHAT( _what, _name )			DUF_IF_TRACE_WHAT_C( DUF_TRACE_CONFIG,	_what,		_name )
#  define DUF_IF_TRACE( _name )					DUF_IF_TRACE_WHAT(			cli.trace,	_name )

#  define DUF_IF_TRACE_WHATN_C( _cfg, _what, _name, _min )	( _cfg && !_cfg->cli.trace.nonew && _cfg->_what._name > _min )
#  define DUF_IF_TRACE_WHATN( _what, _name, _min )		DUF_IF_TRACE_WHATN_C( DUF_TRACE_CONFIG,	_what,		_name, _min )
#  define DUF_IF_TRACEN( _name, _min )				DUF_IF_TRACE_WHATN(			cli.trace,	_name, _min)

#  define DUF_TRACE_FILE_C( _cfg ) ( _cfg && _cfg->cli.trace.output.out ? _cfg->cli.trace.output.out : stdout )

/* ###################################################################### */

#  define DUF_TRACE_WHAT_BASE( _cfg, _signum, _flags, _ern, _what, _name, _min, ... ) \
			duf_trace(	DUF_TRACE_MODE_ ## _name, \
					#_name, \
	/* level */			DUF_IF_TRACE_WHAT_C( _cfg, _what, _name ), \
	/* minlevel */			_min, \
	/* funcid, linid */		DUF_FL, \
	/* time0 */			_cfg?_cfg->loadtime:0, \
	/* signum */			_signum, \
	/* flags, nerr */		_flags, _ern, \
	/* out */			DUF_TRACE_FILE_C( _cfg ), \
			    		__VA_ARGS__ )

/* ###################################################################### */

#  define DUF_TRACE_WHATSYSE_C( _cfg, _ern, _what, _name, _min, ... ) \
		DUF_TRACE_WHAT_BASE( _cfg, _cfg?'$':'_', DUF_TRACE_FLAG_SYSTEM, _ern, _what, _name, _min, __VA_ARGS__ )

#  define DUF_TRACE_WHATSYSE( _ern, _what, _name, _min, ... ) \
							DUF_TRACE_WHATSYSE_C( DUF_TRACE_CONFIG, _ern,	_what,		_name, _min, __VA_ARGS__ )

#  define DUF_TRACE_WHATSYS( _what, _name, _min, ... )	DUF_TRACE_WHATSYSE( errno,			_what,		_name, _min, __VA_ARGS__ )


#  define DUF_TRACESYS(_name, ...)			DUF_TRACE_WHATSYS( cli.trace, _name, __VA_ARGS__ )
#  define DUF_TRACESYSE(_ern, _name, ...)			DUF_TRACE_WHATSYSE( _ern,		cli.trace,	_name, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_TRACE_WHAT_C( _cfg, _what, _name, _min, ...)      \
                DUF_TRACE_WHAT_BASE( _cfg, _cfg?'*':'-', 0, 0, _what, _name, _min, __VA_ARGS__ )

#  define DUF_TRACE_WHAT( _what, _name, _min, ...)	DUF_TRACE_WHAT_C( DUF_TRACE_CONFIG,		_what,		_name, _min, __VA_ARGS__ )

#  define DUF_TRACE_C( _cfg, _name, ... )		DUF_TRACE_WHAT_C( _cfg,cli.trace, _name, __VA_ARGS__ )
#  define DUF_TRACE( _name, ... )			DUF_TRACE_C(	  DUF_TRACE_CONFIG, _name, __VA_ARGS__ )

#  if defined(__GNUC__)
#    define DUF_SCCB( _macr, _name, _min, _fmt, ... )			_macr( _name, _min, "%" DUF_ACTION_TITLE_FMT "; "  _fmt, \
    			duf_uni_scan_action_title( SCCB ), ##__VA_ARGS__ )
#    define DUF_SCCB_PDI( _macr, _name, _min, _pdi, _fmt, ... )	_macr( _name, _min, "%" DUF_ACTION_TITLE_FMT "; seq:%u "  _fmt, \
    			duf_uni_scan_action_title( SCCB ), duf_pdi_seq(_pdi), ##__VA_ARGS__ )
#  endif

/* ###################################################################### */

#  define DUF_IF_DEBUG( _lev )				DUF_IF_TRACE_WHAT( cli.dbg, debug )
#  define DUF_IF_DEBUGN( _lev )				DUF_IF_TRACE_WHATN( cli.dbg, debug, _lev )
#  define DUF_DEBUG(_lev, ...)				if ( DUF_IF_DEBUGN( _lev ) ) {  __VA_ARGS__ ; }

/* ###################################################################### */

#  define T(_fmt, ...) DUF_TRACE(temp,0,"[T] #@#@# "_fmt,__VA_ARGS__)

/* ###################################################################### */

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
