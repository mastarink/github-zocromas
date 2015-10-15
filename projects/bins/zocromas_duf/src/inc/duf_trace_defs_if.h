#ifndef MAS_DUF_TRACE_DEFS_IF_H
#  define MAS_DUF_TRACE_DEFS_IF_H

/* ###################################################################### */
#  define DUF_IF_TR_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->DUF_TRACE_AT_CFG.nonew) ? _cfg->_what._name: 1 )
#  define DUF_IF_TR_WHAT( _what, _name )			DUF_IF_TR_WHAT_C( DUF_TRACE_CONFIG,	_what,		_name )
#  define DUF_IF_TR( _name )					DUF_IF_TR_WHAT(				DUF_TRACE_AT_CFG,	_name)

#  define DUF_IF_TR_WHATN_C( _cfg, _what, _name, _min )		( _cfg && !_cfg->DUF_TRACE_AT_CFG.nonew && _cfg->_what._name > _min )
#  define DUF_IF_TR_WHATN( _what, _name, _min )			DUF_IF_TR_WHATN_C( DUF_TRACE_CONFIG,	_what,		_name, _min )
#  define DUF_IF_TRN( _name, _min )				DUF_IF_TR_WHATN( DUF_TRACE_AT_CFG, _name, _min)

/* #  define DUF_TR_WHAT_C( _cfg, _what, _name, _min, ... )    if ( DUF_IF_TR_WHATN_C( _cfg, _what, _name, _min )) { __VA_ARGS__ ; } */
/* #  define DUF_TR_WHAT( _what, _name, _min, ... )           DUF_TR_WHAT_C( DUF_TRACE_CONFIG, _what, _name, _min, __VA_ARGS__ ) */

/* #  define DUF_TR_WHATSYS(  _what, _name, _min, ...)        DUF_TR_WHATSYSE( errno, _what, _name, _min, __VA_ARGS__ ) */

/* #  define DUF_TR_C( _cfg,_name, ... )                     DUF_TR_WHAT_C( _cfg, DUF_TRACE_AT_CFG, _name, __VA_ARGS__ ) */
/* #  define DUF_TR(_name, ...)                             DUF_TR_C( DUF_TRACE_CONFIG, __VA_ARGS__ )                */

/* ###################################################################### */

/* #  define DUF_IF_TRACE_WHAT(_what,_name) (DUF_TRACE_CONFIG && !DUF_TRACE_CONFIG->DUF_TRACE_AT_CFG.nonew && DUF_TRACE_CONFIG->_what._name ) */

#  define DUF_IF_TRACE_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->DUF_TRACE_AT_CFG.nonew) ? _cfg->_what._name : 1 )

#  define DUF_IF_TRACE_WHAT( _what, _name )			DUF_IF_TRACE_WHAT_C( DUF_TRACE_CONFIG,	_what,		_name )
#  define DUF_IF_TRACE( _name )					DUF_IF_TRACE_WHAT(			DUF_TRACE_AT_CFG,	_name )

#  define DUF_IF_TRACE_WHATN_C( _cfg, _what, _name, _min )	( _cfg && !_cfg->DUF_TRACE_AT_CFG.nonew && _cfg->_what._name > _min )
#  define DUF_IF_TRACE_WHATN( _what, _name, _min )		DUF_IF_TRACE_WHATN_C( DUF_TRACE_CONFIG,	_what,		_name, _min )
#  define DUF_IF_TRACEN( _name, _min )				DUF_IF_TRACE_WHATN(			DUF_TRACE_AT_CFG,	_name, _min )


#  define DUF_IF_DEBUG( _lev )				DUF_IF_TRACE_WHAT( opt.dbg, debug )
#  define DUF_IF_DEBUGN( _lev )				DUF_IF_TRACE_WHATN( opt.dbg, debug, _lev )

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
