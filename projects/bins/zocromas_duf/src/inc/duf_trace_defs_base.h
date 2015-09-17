#ifndef MAS_DUF_TRACE_DEFS_BASE_H
#  define MAS_DUF_TRACE_DEFS_BASE_H

#  include "duf_config4trace_ref.h"
#  define DUF_TRACE_CONFIG duf_config4trace
#  include "duf_trace_defs_if.h"

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





#  define DUF_TRACE_FILE_C( _cfg ) ( _cfg && _cfg->cli.trace.output.out ? _cfg->cli.trace.output.out : stdout )

/* ###################################################################### */

#  define DUF_TRACE_WHAT_WP_BASE( _cfg, _prefix, _signum, _flags, _ern, _what, _name, _min, ... ) \
			duf_trace(	DUF_TRACE_MODE_ ## _name, DUF_TRACE_SUBMODE_SIMPLE, \
					#_name, \
	/* level */			DUF_IF_TRACE_WHAT_C( _cfg, _what, _name ), \
	/* minlevel */			_min, \
	/* funcid, linid */		DUF_FL, \
	/* time0 */			_cfg?_cfg->loadtime:0, \
	/* signum */			_signum, \
	/* flags, nerr */		_flags, _ern, \
	/* out */			DUF_TRACE_FILE_C( _cfg ), \
			    		_prefix, \
			    		__VA_ARGS__ )
#  define      DUF_TRACE_WHAT_BASE( _cfg,          _signum, _flags, _ern, _what, _name, _min, ... ) \
  					DUF_TRACE_WHAT_WP_BASE(_cfg, NULL, _signum, _flags, _ern, _what, _name, _min, __VA_ARGS__)


#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
