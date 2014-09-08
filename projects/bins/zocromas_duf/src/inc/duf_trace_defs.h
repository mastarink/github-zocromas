#ifndef MAS_DUF_TRACE_DEFS_H
#  define MAS_DUF_TRACE_DEFS_H

#  include "duf_print_defs.h"
#  include "duf_options.h"

/* ###################################################################### */
#  define DUF_T_NOIF

/* #  define DUF_OINV(pref) assert(  !pref opendir || ( \                                                */
/*                           ( ( (int) duf_config->nopen - (int) duf_config->nclose ) ) \                */
/*                         - (pref  depth)  == 1 ) \                                                     */
/*                 )                                                                                     */
/*  (* - (pref levinfo?pref levinfo[pref depth].is_leaf:0) *)                                            */
/* #  define DUF_OINVC(pref) assert(  !pref opendir || ( \                                               */
/*                           ( ( (int) duf_config->nopen - (int) duf_config->nclose ) ) \                */
/*                         - (pref  depth)  == 0 ) \                                                     */
/*                 )                                                                                     */
/*                                                                                                       */
/* #  define DUF_OINV_OPENED(pref)     \                                                                 */
/*                 assert(  !pref opendir || (pref levinfo && pref levinfo[pref depth].lev_dh.dfd ))     */
/* #  define DUF_OINV_NOT_OPENED(pref) \                                                                 */
/*                 assert(  !pref opendir || (!pref levinfo || pref levinfo[pref depth].lev_dh.dfd==0 )) */

/* ###################################################################### */



/* ###################################################################### */
#  define DUF_IF_TR_WHAT_C(  cfg, what, name )		(( cfg && !cfg->cli.trace.nonew) ? cfg->what.name: 1 )
#  define DUF_IF_TR_WHAT( what, name )			DUF_IF_TR_WHAT_C( duf_config,  what,      name )
#  define DUF_IF_TR( name )				DUF_IF_TR_WHAT(		  cli.trace, name)

#  define DUF_IF_TR_WHATN_C( cfg, what, name, min )	( cfg && !cfg->cli.trace.nonew && cfg->what.name > min )
#  define DUF_IF_TR_WHATN( what, name, min )		DUF_IF_TR_WHATN_C( duf_config, what,      name, min )
#  define DUF_IF_TRN( name, min )			DUF_IF_TR_WHATN(		  cli.trace, name, min)

/* #  define DUF_TR_WHAT_C( cfg, what, name, min, ... )    if ( DUF_IF_TR_WHATN_C( cfg, what, name, min )) { __VA_ARGS__ ; } */
/* #  define DUF_TR_WHAT( what, name, min, ... )           DUF_TR_WHAT_C( duf_config, what, name, min, __VA_ARGS__ ) */

/* #  define DUF_TR_WHATSYS(  what, name, min, ...)        DUF_TR_WHATSYSE( errno,  what, name, min, __VA_ARGS__ ) */

/* #  define DUF_TR_C( cfg,name, ... )                     DUF_TR_WHAT_C( cfg, cli.trace, name, __VA_ARGS__ ) */
/* #  define DUF_TR(name, ...)                             DUF_TR_C( duf_config, __VA_ARGS__ )                */

/* ###################################################################### */

/* #  define DUF_IF_TRACE_WHAT(what,name) (duf_config && !duf_config->cli.trace.nonew && duf_config->what.name ) */
#  define DUF_IF_TRACE_WHAT_C(  cfg, what, name )	(( cfg && !cfg->cli.trace.nonew) ? (cfg->what.name?1:0) : 1 )
#  define DUF_IF_TRACE_WHAT( what, name )		DUF_IF_TRACE_WHAT_C( duf_config,  what,      name )
#  define DUF_IF_TRACE( name )                          DUF_IF_TRACE_WHAT(                cli.trace, name)

#  define DUF_IF_TRACE_WHATN_C( cfg, what, name, min )  ( cfg && !cfg->cli.trace.nonew && cfg->what.name > min )
#  define DUF_IF_TRACE_WHATN( what, name, min )         DUF_IF_TRACE_WHATN_C( duf_config, what,      name, min )
#  define DUF_IF_TRACEN( name, min )                    DUF_IF_TRACE_WHATN(               cli.trace, name, min)

#  define DUF_TRACE_FILE_C( cfg ) ( cfg && cfg->cli.trace.output.out ? cfg->cli.trace.output.out : stdout )

#  define DUF_TRACE_WHAT_C( cfg, what, name, min, ...)	duf_trace( DUF_TRACE_MODE_ ## name, #name, \
		    	DUF_IF_TRACE_WHAT_C( cfg, what, name ), min, \
			DUF_FL, cfg?cfg->loadtime:0, 0, 0, DUF_TRACE_FILE_C( cfg ), __VA_ARGS__ )
#  define DUF_TRACE_WHAT( what, name, min, ...)		DUF_TRACE_WHAT_C( duf_config,	  what,	     name, min, __VA_ARGS__ )

#  define DUF_TRACE_C( cfg, name, ... )			DUF_TRACE_WHAT_C( cfg,		  cli.trace, name, __VA_ARGS__ )
#  define DUF_TRACE( name, ... )			DUF_TRACE_C(	  duf_config,		     name, __VA_ARGS__ )

#  if defined(__GNUC__)
#    define DUF_SCCB( macr, name, min, fmt, ... )			macr( name, min, "%" DUF_ACTION_TITLE_FMT "; "  fmt, \
    			duf_uni_scan_action_title( sccb ),  ##__VA_ARGS__ )
#    define DUF_SCCB_PDI( macr, name, min, pdi, fmt, ... )	macr( name, min, "%" DUF_ACTION_TITLE_FMT "; seq:%u "  fmt, \
    			duf_uni_scan_action_title( sccb ), duf_pdi_seq(pdi),  ##__VA_ARGS__ )
#  endif

/* ###################################################################### */

#  define DUF_TRACE_WHATSYSE_C( cfg, ern, what, name, min, ... ) \
		duf_trace( DUF_TRACE_MODE_ ## name, #name, \
			DUF_IF_TRACE_WHAT_C( cfg, what, name ), min, \
			DUF_FL, cfg?cfg->loadtime:0, DUF_TRACE_FLAG_SYSTEM, ern, DUF_TRACE_FILE_C( cfg ), __VA_ARGS__ )

#  define DUF_TRACE_WHATSYSE( ern, what, name, min, ... ) \
							DUF_TRACE_WHATSYSE_C( duf_config, ern, what, name, min, __VA_ARGS__ )

#  define DUF_TRACE_WHATSYS( what, name, min, ... )	DUF_TRACE_WHATSYSE( errno,  what, name, min, __VA_ARGS__ )


#  define DUF_TRACESYS(name, ...)			DUF_TRACE_WHATSYS( cli.trace, name, __VA_ARGS__ )
#  define DUF_TRACESYSE(ern, name, ...)			DUF_TRACE_WHATSYSE( ern, cli.trace, name, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_IF_DEBUG( lev )				DUF_IF_TRACE_WHAT( cli.dbg, debug )
#  define DUF_IF_DEBUGN( lev )				DUF_IF_TRACE_WHATN( cli.dbg, debug, lev )
#  define DUF_DEBUG(lev, ...)				if ( DUF_IF_DEBUGN( lev ) ) {  __VA_ARGS__ ; }

/* ###################################################################### */


/* ###################################################################### */

#  define DUF_ERROR( ... )				DUF_TRACE( error, 0, __VA_ARGS__ )
#  ifdef DUF_T_NOIF
#    define DUF_ERRORQ( cond, ... )		( ( cond ) ? DUF_ERROR(__VA_ARGS__) : 0)
#  else
#    define DUF_ERRORQ( cond, ... )		if (cond)  DUF_ERROR(__VA_ARGS__)
#  endif
#  define DUF_ERRORR( r_t, ... )			DUF_TRACE( errorr, r_t, __VA_ARGS__ )
#  define DUF_ERRORiV( v )				DUF_ERROR( #v ":%d" , v )
#  define DUF_ERRSYS( ... )				DUF_TRACESYS( error, 0, __VA_ARGS__ )
#  define DUF_ERRSYSE( ern, ... )			DUF_TRACESYSE( ern, error, 0, __VA_ARGS__ )

/* ###################################################################### */

/* error message if arg is not 0 */
/* #  define DUF_TEST_RX(_rval)         if (_rval) DUF_ERROR( " - - - - - -> [%s] (#%d)", _rval<0?duf_error_name(_rval):"+", _rval ) */
#  define DUF_TEST_RX(_rval)	   DUF_ERRORQ( _rval, " - - - - - -> [%s] (#%d)", (_rval)<0?duf_error_name(_rval):"+", _rval )

#  ifdef DUF_T_NOIF
#    define DUF_TEST_RQX(_rval, cond)  (( !(cond) ) ? DUF_TEST_RX( _rval ) : 0)
#  else
#    define DUF_TEST_RQX(_rval, cond)  if ( !(cond) ) DUF_TEST_RX( _rval )
#  endif
/* #  define DUF_TEST_R(_rval)       if ( _rval!=DUF_ERROR_MAX_REACHED && _rval!=DUF_ERROR_MAX_SEQ_REACHED ) DUF_TEST_RX( _rval ) */

/* error message if arg is not 0, except some predefines */
#  define DUF_TEST_R(_rval)	DUF_TEST_RQX( _rval,  _rval==DUF_ERROR_MAX_REACHED || _rval==DUF_ERROR_MAX_SEQ_REACHED  )
#  ifdef DUF_T_NOIF
#    define DUF_TEST_RQ(_rval, cond)   ( ( !(cond) ) ? DUF_TEST_R( _rval ) : 0 )
/* error message if arg is < 0, except some predefines */
#    define DUF_TEST_RN(_rval)	   ( ( _rval<0 ) ? DUF_TEST_R( _rval ) : 0 )
#    define DUF_TEST_RQN(_rval, cond)  ( ( !(cond) ) ? DUF_TEST_RQ( _rval ) : 0 )
#  else
#    define DUF_TEST_RQ(_rval, cond)   if ( !(cond) ) DUF_TEST_R( _rval )
/* error message if arg is < 0, except some predefines */
#    define DUF_TEST_RN(_rval)	     if ( _rval<0 ) DUF_TEST_R( _rval )
#    define DUF_TEST_RQN(_rval, cond)  if ( !(cond) ) DUF_TEST_RQ( _rval )
#  endif


/* #  define DUF_TEST_RR(_rval)      if ( _rval!=DUF_SQL_ROW && _rval!=DUF_SQL_DONE ) DUF_TEST_R( _rval ) */
#  define DUF_TEST_RR(_rval)	DUF_TEST_RQ(_rval, _rval==DUF_SQL_ROW || _rval==DUF_SQL_DONE )

/* ###################################################################### */

#  ifdef DUF_T_NOIF
#    define DUF_TEST_R3(_rval)	( (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					) ?		\
					DUF_ERROR( " - - - - - -> sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval) \
					    ) : 0 )
#    define DUF_TEST_R3S(_rval, _xstr) 	( (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					) ?		\
					DUF_ERROR( " - - - - - -> sqlite3 [%s] (#%d) {%s}", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval), _xstr \
					    ) : 0 )
#  else
#    define DUF_TEST_R3(_rval)	if (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					)		\
					DUF_ERROR( " - - - - - -> sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval) \
					    )
#    define DUF_TEST_R3S(_rval, _xstr) 	if (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					)		\
					DUF_ERROR( " - - - - - -> sqlite3 [%s] (#%d) {%s}", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval), _xstr \
					    )
#  endif




/* ###################################################################### */

#  ifdef DUF_T_NOIF
#    define DUF_DO_TEST_R(_rval, _x)    ( (_rval>=0) ? ( (_rval=(_x)), DUF_TEST_R(_rval) ) : 0 )
/* #  define DUF_DO_TEST_R(_rval, _x)  ((_rval>=0) ?( (_rval=_x), DUF_TEST_R(_rval) ) : 0 ) */

#    define DUF_DO_TEST_PR(_rval, _x)   ( (_rval>=0) ? ( (_x), DUF_TEST_R(_rval) ) : 0 )
#  else
#    define DUF_DO_TEST_R(_rval, _x)    if (_rval>=0) { (_rval=(_x)); DUF_TEST_R(_rval); }
/* #  define DUF_DO_TEST_R(_rval, _x)  ((_rval>=0) ?( (_rval=_x), DUF_TEST_R(_rval) ) : 0 ) */

#    define DUF_DO_TEST_PR(_rval, _x)   if (_rval>=0) { _x; DUF_TEST_R(_rval); }
#  endif
/* ###################################################################### */

#  define DUF_START() {
#  define DUF_STARTR(_rt) int _rt=0; DUF_START()
#  define DUF_STARTULL(_rt) unsigned long long _rt=0; DUF_START()
#  define DUF_END() }
#  define DUF_ENDR(_rt)    DUF_TEST_R(_rt);                           DUF_END(); return _rt
#  define DUF_ENDRN(_rt)   DUF_TEST_RN(_rt);                          DUF_END(); return _rt
#  define DUF_ENDR3(_rt3)  DUF_TEST_R( DUF_SQLITE_ERROR_CODE(_rt3) ); DUF_END(); return _rt3
#  define DUF_ENDULL(_rt)                                             DUF_END(); return _rt
#  define DUF_ENDS(_s)                                                DUF_END(); return _s

/* ###################################################################### */
/* ###################################################################### */

#  define T(fmt, ...) DUF_TRACE(temp,0,"[T] #@#@# "fmt,__VA_ARGS__)

/* ###################################################################### */

#  define DOR(_rval, _x) DUF_DO_TEST_R(_rval, _x)
#  define DOPR(_rval, _x) DUF_DO_TEST_PR(_rval, _x)


#endif
