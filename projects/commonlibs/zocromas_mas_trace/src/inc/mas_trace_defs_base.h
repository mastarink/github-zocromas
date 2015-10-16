#ifndef MAS_MAST_TRACE_DEFS_BASE_H
#  define MAS_MAST_TRACE_DEFS_BASE_H




#  include "mas_trace_defs_preset.h"
#  include "mas_trace_defs_if.h"


/* #  define MAST_OINV(pref) assert(  !pref opendir || ( \                                                */
/*                           ( ( (int) MAST_TRACE_CONFIG->nopen - (int) MAST_TRACE_CONFIG->nclose ) ) \                */
/*                         - (pref  depth)  == 1 ) \                                                     */
/*                 )                                                                                     */
/*  (* - (pref levinfo?pref levinfo[pref depth].is__leaf:0) *)                                            */
/* #  define MAST_OINVC(pref) assert(  !pref opendir || ( \                                               */
/*                           ( ( (int) MAST_TRACE_CONFIG->nopen - (int) MAST_TRACE_CONFIG->nclose ) ) \                */
/*                         - (pref  depth)  == 0 ) \                                                     */
/*                 )                                                                                     */
/*                                                                                                       */
/* #  define MAST_OINV_OPENED(pref)     \                                                                 */
/*                 assert(  !pref opendir || (pref levinfo && pref levinfo[pref depth].lev_dh.dfd ))     */
/* #  define MAST_OINV_NOT_OPENED(pref) \                                                                 */
/*                 assert(  !pref opendir || (!pref levinfo || pref levinfo[pref depth].lev_dh.dfd==0 )) */
/* ###################################################################### */
/* ###################################################################### */
/* takes ern - error index */
#  define MAST_TRACE_WHAT_WP_BASE( _cfg, _prefix, _signum, _flags, _ern, _what, _name, _min, ... ) \
			mas_trace( \
			    /* MAST_TRACE_MODE_ ## _name, MAST_TRACE_SUBMODE_SIMPLE, */ \
					#_name, \
	/* level */			MAST_IF_TRACE_WHAT_C( _cfg, _what, _name ), \
	/* minlevel */			_min, \
	/* funcid, linid */		MAST_FL, \
	/* time0 */			MAST_TRACE_LOADTIME_C( _cfg ), \
	/* signum */			_signum, \
	/* flags, nerr */		_flags, _ern, \
	/* out */			MAST_TRACE_FILE_C( _cfg ), \
			    		_prefix, \
			    		MAST_TRACE_FUNWIDTH( _cfg ), \
			    		__VA_ARGS__ )
/* takes ern - error index */
#  define      MAST_TRACE_WHAT_BASE( _cfg,          _signum, _flags, _ern, _what, _name, _min, ... ) \
  					MAST_TRACE_WHAT_WP_BASE(_cfg, NULL, _signum, _flags, _ern, _what, _name, _min, __VA_ARGS__)
#endif
/*
vi: ft=c
vi: colorcolumn=65,105,121
*/