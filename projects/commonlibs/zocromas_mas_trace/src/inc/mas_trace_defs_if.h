#ifndef MAS_MAST_TRACE_DEFS_IF_H
#  define MAS_MAST_TRACE_DEFS_IF_H

/* ###################################################################### */
#  define MAST_IF_TR_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->MAST_TRACE_AT_CFG.nonew) ? _cfg->_what._name: 1 )
#  define MAST_IF_TR_WHAT( _what, _name )			MAST_IF_TR_WHAT_C( MAST_TRACE_CONFIG,	_what,		_name )
#  define MAST_IF_TR( _name )					MAST_IF_TR_WHAT(				MAST_TRACE_AT_CFG,	_name)

#  define MAST_IF_TR_WHATN_C( _cfg, _what, _name, _min )		( _cfg && !_cfg->MAST_TRACE_AT_CFG.nonew && _cfg->_what._name > _min )
#  define MAST_IF_TR_WHATN( _what, _name, _min )			MAST_IF_TR_WHATN_C( MAST_TRACE_CONFIG,	_what,		_name, _min )
#  define MAST_IF_TRN( _name, _min )				MAST_IF_TR_WHATN( MAST_TRACE_AT_CFG, _name, _min)

/* #  define MAST_TR_WHAT_C( _cfg, _what, _name, _min, ... )    if ( MAST_IF_TR_WHATN_C( _cfg, _what, _name, _min )) { __VA_ARGS__ ; } */
/* #  define MAST_TR_WHAT( _what, _name, _min, ... )           MAST_TR_WHAT_C( MAST_TRACE_CONFIG, _what, _name, _min, __VA_ARGS__ ) */

/* #  define MAST_TR_WHATSYS(  _what, _name, _min, ...)        MAST_TR_WHATSYSE( errno, _what, _name, _min, __VA_ARGS__ ) */

/* #  define MAST_TR_C( _cfg,_name, ... )                     MAST_TR_WHAT_C( _cfg, MAST_TRACE_AT_CFG, _name, __VA_ARGS__ ) */
/* #  define MAST_TR(_name, ...)                             MAST_TR_C( MAST_TRACE_CONFIG, __VA_ARGS__ )                */

/* ###################################################################### */

/* #  define MAST_IF_TRACE_WHAT(_what,_name) (MAST_TRACE_CONFIG && !MAST_TRACE_CONFIG->MAST_TRACE_AT_CFG.nonew && MAST_TRACE_CONFIG->_what._name ) */

#  define MAST_IF_TRACE_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->MAST_TRACE_AT_CFG.nonew) ? _cfg->_what._name : 1 )

#  define MAST_IF_TRACE_WHAT( _what, _name )			MAST_IF_TRACE_WHAT_C( MAST_TRACE_CONFIG,	_what,		_name )
#  define MAST_IF_TRACE( _name )					MAST_IF_TRACE_WHAT(			MAST_TRACE_AT_CFG,	_name )

#  define MAST_IF_TRACE_WHATN_C( _cfg, _what, _name, _min )	( _cfg && !_cfg->MAST_TRACE_AT_CFG.nonew && _cfg->_what._name > _min )
#  define MAST_IF_TRACE_WHATN( _what, _name, _min )		MAST_IF_TRACE_WHATN_C( MAST_TRACE_CONFIG,	_what,		_name, _min )
#  define MAST_IF_TRACEN( _name, _min )				MAST_IF_TRACE_WHATN(			MAST_TRACE_AT_CFG,	_name, _min )


#  define MAST_IF_DEBUG( _lev )				MAST_IF_TRACE_WHAT( opt.dbg, debug )
#  define MAST_IF_DEBUGN( _lev )				MAST_IF_TRACE_WHATN( opt.dbg, debug, _lev )

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
