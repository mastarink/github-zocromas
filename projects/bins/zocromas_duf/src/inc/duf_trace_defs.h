#ifndef MAS_DUF_TRACE_DEFS_H
#  define MAS_DUF_TRACE_DEFS_H

#  include "duf_print_defs.h"
#  include "duf_options.h"

/* ###################################################################### */

#  define DUF_OINV(pref) assert(  !pref opendir || ( \
    		          ( ( (int) duf_config->nopen - (int) duf_config->nclose ) ) \
	    		- (pref  depth)  == 1 ) \
    		)
 /* - (pref levinfo?pref levinfo[pref depth].is_leaf:0) */
#  define DUF_OINVC(pref) assert(  !pref opendir || ( \
    		          ( ( (int) duf_config->nopen - (int) duf_config->nclose ) ) \
	    		- (pref  depth)  == 0 ) \
    		)

#  define DUF_OINV_OPENED(pref)     \
  		assert(  !pref opendir || (pref levinfo && pref levinfo[pref depth].lev_dh.dfd ))
#  define DUF_OINV_NOT_OPENED(pref) \
  		assert(  !pref opendir || (!pref levinfo || pref levinfo[pref depth].lev_dh.dfd==0 ))

/* ###################################################################### */



/* ###################################################################### */
#  define DUF_IF_TR_WHAT_C(  cfg, what, name )		(( cfg && !cfg->cli.trace.nonew) ? cfg->what.name: 1 )
#  define DUF_IF_TR_WHAT( what, name )			DUF_IF_TR_WHAT_C( duf_config,  what,      name )
#  define DUF_IF_TR( name )				DUF_IF_TR_WHAT(		  cli.trace, name)

#  define DUF_IF_TR_WHATN_C( cfg, what, name, min )	( cfg && !cfg->cli.trace.nonew && cfg->what.name > min )
#  define DUF_IF_TR_WHATN( what, name, min )		DUF_IF_TR_WHATN_C( duf_config, what,      name, min )
#  define DUF_IF_TRN( name, min )			DUF_IF_TR_WHATN(		  cli.trace, name, min)

#  define DUF_TR_WHAT_C( cfg, what, name, min, ... )	if ( DUF_IF_TR_WHATN_C( cfg, what, name, min )) { __VA_ARGS__ ; }
#  define DUF_TR_WHAT( what, name, min, ... )		DUF_TR_WHAT_C( duf_config, what, name, min, __VA_ARGS__ )

#  define DUF_TR_WHATSYS(  what, name, min, ...)	DUF_TR_WHATSYSE( errno,  what, name, min, __VA_ARGS__ )

#  define DUF_TR_C( cfg,name, ... )			DUF_TR_WHAT_C( cfg, cli.trace, name, __VA_ARGS__ )
#  define DUF_TR(name, ...)				DUF_TR_C( duf_config, __VA_ARGS__ )

/* ###################################################################### */

/* #  define DUF_IF_TRACE_WHAT(what,name) (duf_config && !duf_config->cli.trace.nonew && duf_config->what.name ) */
#  define DUF_IF_TRACE_WHAT_C(  cfg, what, name )	(( cfg && !cfg->cli.trace.nonew) ? cfg->what.name: 1 )
#  define DUF_IF_TRACE_WHAT( what, name )		DUF_IF_TRACE_WHAT_C( duf_config,  what,      name )
#  define DUF_IF_TRACE( name )				DUF_IF_TRACE_WHAT(		  cli.trace, name)

#  define DUF_IF_TRACE_WHATN_C( cfg, what, name, min )  ( cfg && !cfg->cli.trace.nonew && cfg->what.name > min )
#  define DUF_IF_TRACE_WHATN( what, name, min )		DUF_IF_TRACE_WHATN_C( duf_config, what,      name, min )
#  define DUF_IF_TRACEN( name, min )			DUF_IF_TRACE_WHATN(		  cli.trace, name, min)

#  define DUF_TRACE_FILE_C( cfg ) ( cfg && cfg->cli.trace.out ? cfg->cli.trace.out : stdout )

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
#  define DUF_ERRORR( r, ... )				DUF_TRACE( errorr, r, __VA_ARGS__ )
#  define DUF_ERRORiV( v )				DUF_ERROR( #v ":%d" , v )
#  define DUF_ERRSYS( ... )				DUF_TRACESYS( error, 0, __VA_ARGS__ )
#  define DUF_ERRSYSE( ern, ... )			DUF_TRACESYSE( ern, error, 0, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_TEST_RX(val)	if (val) DUF_ERROR( " - - - - - -> [%s] (#%d)", val<0?duf_error_name(val):"-", val )
#  define DUF_TEST_RQX(val, cond) if ( !(cond) ) DUF_TEST_RX( val )

/* #  define DUF_TEST_R(val)       if ( val!=DUF_ERROR_MAX_REACHED && val!=DUF_ERROR_MAX_SEQ_REACHED ) DUF_TEST_RX( val ) */
#  define DUF_TEST_R(val)	DUF_TEST_RQX( val,  val==DUF_ERROR_MAX_REACHED || val==DUF_ERROR_MAX_SEQ_REACHED  )
#  define DUF_DO_TEST_R(val, x) { val=x; DUF_TEST_R(val); }
#  define DUF_TEST_RQ(val, cond) if ( !(cond) ) DUF_TEST_R( val )

#  define DUF_TEST_RN(val)	if ( val<0 ) DUF_TEST_R( val )

/* #  define DUF_TEST_RR(val)      if ( val!=DUF_SQL_ROW && val!=DUF_SQL_DONE ) DUF_TEST_R( val ) */
#  define DUF_TEST_RR(val)	DUF_TEST_RQ(val, val==DUF_SQL_ROW || val==DUF_SQL_DONE )


#  define DUF_TEST_R3(val)	if (val \
    			&& (val)!=SQLITE_ROW \
    			&& (val)!=SQLITE_DONE \
    					)		\
					DUF_ERROR( " - - - - - -> sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(val) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(val)) : "-", \
					    DUF_SQLITE_ERROR_CODE(val) \
					    )

#endif
