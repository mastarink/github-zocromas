#ifndef MAS_DUF_TRACE_DEFS_H
#  define MAS_DUF_TRACE_DEFS_H

#  include "duf_print_defs.h"
#  include "duf_options.h"
#  include "duf_error_types.h"
#  include "duf_errors.h"

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
#  define DUF_IF_TR_WHAT_C(  _cfg, _what, _name )		(( _cfg && !_cfg->cli.trace.nonew) ? _cfg->_what._name: 1 )
#  define DUF_IF_TR_WHAT( _what, _name )			DUF_IF_TR_WHAT_C( duf_config,  _what,      _name )
#  define DUF_IF_TR( _name )				DUF_IF_TR_WHAT(		  cli.trace, _name)

#  define DUF_IF_TR_WHATN_C( _cfg, _what, _name, _min )	( _cfg && !_cfg->cli.trace.nonew && _cfg->_what._name > _min )
#  define DUF_IF_TR_WHATN( _what, _name, _min )		DUF_IF_TR_WHATN_C( duf_config, _what,      _name, _min )
#  define DUF_IF_TRN( _name, _min )			DUF_IF_TR_WHATN(		  cli.trace, _name, _min)

/* #  define DUF_TR_WHAT_C( _cfg, _what, _name, _min, ... )    if ( DUF_IF_TR_WHATN_C( _cfg, _what, _name, _min )) { __VA_ARGS__ ; } */
/* #  define DUF_TR_WHAT( _what, _name, _min, ... )           DUF_TR_WHAT_C( duf_config, _what, _name, _min, __VA_ARGS__ ) */

/* #  define DUF_TR_WHATSYS(  _what, _name, _min, ...)        DUF_TR_WHATSYSE( errno,  _what, _name, _min, __VA_ARGS__ ) */

/* #  define DUF_TR_C( _cfg,_name, ... )                     DUF_TR_WHAT_C( _cfg, cli.trace, _name, __VA_ARGS__ ) */
/* #  define DUF_TR(_name, ...)                             DUF_TR_C( duf_config, __VA_ARGS__ )                */

/* ###################################################################### */

/* #  define DUF_IF_TRACE_WHAT(_what,_name) (duf_config && !duf_config->cli.trace.nonew && duf_config->_what._name ) */

#  define DUF_IF_TRACE_WHAT_C(  _cfg, _what, _name )	(( _cfg && !_cfg->cli.trace.nonew) ? _cfg->_what._name : 1 )

#  define DUF_IF_TRACE_WHAT( _what, _name )		DUF_IF_TRACE_WHAT_C( duf_config,  _what,      _name )
#  define DUF_IF_TRACE( _name )                          DUF_IF_TRACE_WHAT(                cli.trace, _name)

#  define DUF_IF_TRACE_WHATN_C( _cfg, _what, _name, _min )  ( _cfg && !_cfg->cli.trace.nonew && _cfg->_what._name > _min )
#  define DUF_IF_TRACE_WHATN( _what, _name, _min )         DUF_IF_TRACE_WHATN_C( duf_config, _what,      _name, _min )
#  define DUF_IF_TRACEN( _name, _min )                    DUF_IF_TRACE_WHATN(               cli.trace, _name, _min)

#  define DUF_TRACE_FILE_C( _cfg ) ( _cfg && _cfg->cli.trace.output.out ? _cfg->cli.trace.output.out : stdout )

#  define DUF_TRACE_WHAT_C( _cfg, _what, _name, _min, ...)	duf_trace( DUF_TRACE_MODE_ ## _name, #_name, \
		    	DUF_IF_TRACE_WHAT_C( _cfg, _what, _name ), _min, \
			DUF_FL, _cfg?_cfg->loadtime:0, 0, 0, DUF_TRACE_FILE_C( _cfg ), __VA_ARGS__ )
#  define DUF_TRACE_WHAT( _what, _name, _min, ...)		DUF_TRACE_WHAT_C( duf_config,	  _what,	     _name, _min, __VA_ARGS__ )

#  define DUF_TRACE_C( _cfg, _name, ... )			DUF_TRACE_WHAT_C( _cfg,		  cli.trace, _name, __VA_ARGS__ )
#  define DUF_TRACE( _name, ... )			DUF_TRACE_C(	  duf_config,		     _name, __VA_ARGS__ )

#  if defined(__GNUC__)
#    define DUF_SCCB( _macr, _name, _min, _fmt, ... )			_macr( _name, _min, "%" DUF_ACTION_TITLE_FMT "; "  _fmt, \
    			duf_uni_scan_action_title( sccb ),  ##__VA_ARGS__ )
#    define DUF_SCCB_PDI( _macr, _name, _min, _pdi, _fmt, ... )	_macr( _name, _min, "%" DUF_ACTION_TITLE_FMT "; seq:%u "  _fmt, \
    			duf_uni_scan_action_title( sccb ), duf_pdi_seq(_pdi),  ##__VA_ARGS__ )
#  endif

/* ###################################################################### */

#  define DUF_TRACE_WHATSYSE_C( _cfg, _ern, _what, _name, _min, ... ) \
		duf_trace( DUF_TRACE_MODE_ ## _name, #_name, \
			DUF_IF_TRACE_WHAT_C( _cfg, _what, _name ), _min, \
			DUF_FL, _cfg?_cfg->loadtime:0, DUF_TRACE_FLAG_SYSTEM, _ern, DUF_TRACE_FILE_C( _cfg ), __VA_ARGS__ )

#  define DUF_TRACE_WHATSYSE( _ern, _what, _name, _min, ... ) \
							DUF_TRACE_WHATSYSE_C( duf_config, _ern, _what, _name, _min, __VA_ARGS__ )

#  define DUF_TRACE_WHATSYS( _what, _name, _min, ... )	DUF_TRACE_WHATSYSE( errno,  _what, _name, _min, __VA_ARGS__ )


#  define DUF_TRACESYS(_name, ...)			DUF_TRACE_WHATSYS( cli.trace, _name, __VA_ARGS__ )
#  define DUF_TRACESYSE(_ern, _name, ...)			DUF_TRACE_WHATSYSE( _ern, cli.trace, _name, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_IF_DEBUG( _lev )				DUF_IF_TRACE_WHAT( cli.dbg, debug )
#  define DUF_IF_DEBUGN( _lev )				DUF_IF_TRACE_WHATN( cli.dbg, debug, _lev )
#  define DUF_DEBUG(_lev, ...)				if ( DUF_IF_DEBUGN( _lev ) ) {  __VA_ARGS__ ; }

/* ###################################################################### */


/* ###################################################################### */
#  define DUF_E1_NO(...) duf_set_mereport(1, 0, __VA_ARGS__, 0)
#  define DUF_E_NO(...) duf_set_mereport(0, 0, __VA_ARGS__, 0)
#  define DUF_E_YES(...) duf_set_mereport(0, 1, __VA_ARGS__, 0)
#  define DUF_EREPORT(r) (duf_get_ereport(r))
#  define DUF_ERROR( ... )				DUF_TRACE( error, 0, __VA_ARGS__ )
#  ifdef DUF_T_NOIF
#    define DUF_ERRORQ( _cond, ... )		( ( _cond ) ? DUF_ERROR(__VA_ARGS__) : 0)
#  else
#    define DUF_ERRORQ( _cond, ... )		if (_cond)  DUF_ERROR(__VA_ARGS__)
#  endif
#  define DUF_ERRORR( r_t, ... )			DUF_TRACE( errorr, r_t, __VA_ARGS__ )
#  define DUF_ERRORiV( v )				DUF_ERROR( #v ":%d" , v )
#  define DUF_ERRSYS( ... )				DUF_TRACESYS( error, 0, __VA_ARGS__ )
#  define DUF_ERRSYSE( _ern, ... )			DUF_TRACESYSE( _ern, error, 0, __VA_ARGS__ )

/* ###################################################################### */

/* error message if arg is not 0 */
/* #  define DUF_TEST_RX(_rval)         if (_rval) DUF_ERROR( " - - - - - -> [%s] (#%d)", _rval<0?duf_error_name(_rval):"+", _rval ) */
#  ifdef DUF_T_NOIF
#    define DUF_TEST_RX(_rval)	   ( (DUF_EREPORT(_rval)) ? \
    					DUF_ERROR( " - - - - - -> [%s] (#%d)", (_rval)<0?duf_error_name(_rval):"+", _rval ) : \
    					0 )
#  else
#    define DUF_TEST_RX(_rval)	if (DUF_EREPORT(_rval)) \
				{ DUF_ERROR( " - - - - - -> [%s] (#%d)", (_rval)<0?duf_error_name(_rval):"+", _rval ); }
#  endif

#  ifdef DUF_T_NOIF
#    define DUF_TEST_RQX(_rval, _cond)  (( !(_cond) ) ? DUF_TEST_RX( _rval ) : 0)
#  else
#    define DUF_TEST_RQX(_rval, _cond)  if ( !(_cond) ) DUF_TEST_RX( _rval )
#  endif
/* #  define DUF_TEST_R(_rval)       if ( _rval!=DUF_ERROR_MAX_REACHED && _rval!=DUF_ERROR_MAX_SEQ_REACHED ) DUF_TEST_RX( _rval ) */

/* error message if arg is not 0, except some predefines */

/* #  define DUF_TEST_R(_rval)     DUF_TEST_RQX( _rval,  \       */
/*                            _rval==DUF_ERROR_MAX_REACHED \     */
/*                         || _rval==DUF_ERROR_MAX_SEQ_REACHED \ */
/*                         || _rval==DUF_ERROR_MAX_DEPTH  \      */
/*         )                                                     */
/* #  define DUF_TEST_R(_rval)     ( DUF_E_NO(DUF_ERROR_MAX_REACHED,DUF_ERROR_MAX_SEQ_REACHED,DUF_ERROR_MAX_DEPTH,0),DUF_TEST_RX( _rval ),DUF_E_YES(DUF_ERROR_MAX_REACHED,DUF_ERROR_MAX_SEQ_REACHED,DUF_ERROR_MAX_DEPTH,0),_rval ) */
#  define DUF_TEST_R(_rval)	DUF_TEST_RX( _rval )





#  ifdef DUF_T_NOIF
#    define DUF_TEST_RQ(_rval, _cond)   ( ( !(_cond) ) ? DUF_TEST_R( _rval ) : 0 )
/* error message if arg is < 0, except some predefines */
#    define DUF_TEST_RN(_rval)	   ( ( _rval<0 ) ? DUF_TEST_R( _rval ) : 0 )
#    define DUF_TEST_RQN(_rval, _cond)  ( ( !(_cond) ) ? DUF_TEST_RQ( _rval ) : 0 )
#  else
#    define DUF_TEST_RQ(_rval, _cond)   if ( !(_cond) ) DUF_TEST_R( _rval )
/* error message if arg is < 0, except some predefines */
#    define DUF_TEST_RN(_rval)	     if ( _rval<0 ) DUF_TEST_R( _rval )
#    define DUF_TEST_RQN(_rval, _cond)  if ( !(_cond) ) DUF_TEST_RQ( _rval )
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
#    define DUF_DO_TEST_R_INTERNAL(_rval, _x)     (_rval>=0) ? ( (_rval=(_x)), DUF_TEST_R(_rval) ) : 0
#    define DUF_DO_TEST_RQ_INTERNAL(_rval, _x, _cond)     (_rval>=0) ? ( (_rval=(_x)), DUF_TEST_RQ(_rval, _cond) ) : 0
#    define DUF_DO_TEST_R(_rval, _x)    ( DUF_DO_TEST_R_INTERNAL(_rval, _x) )
#    define DUF_DO_TEST_RQ(_rval, _x, _cond)    ( DUF_DO_TEST_RQ_INTERNAL(_rval, _x, _cond) )

#    define DUF_DO_TEST_RN_INTERNAL(_rval, _x)     (_rval>=0) ? ( (_rval=(_x)), DUF_TEST_RN(_rval) ) : 0
#    define DUF_DO_TEST_RN(_rval, _x)   ( DUF_DO_TEST_RN_INTERNAL(_rval, _x) )

#    define DUF_DO_TESTZ_R(_rval, _x)    ( _rval=0, DUF_DO_TEST_R_INTERNAL(_rval, _x)  )

#    define DUF_DO_TEST_PR(_rval, _x)   ( (_rval>=0) ? ( (_x), DUF_TEST_R(_rval) ) : 0 )
#  else
#    define DUF_DO_TEST_R(_rval, _x)    if (_rval>=0) { (_rval=(_x)); DUF_TEST_R(_rval); }
#    define DUF_DO_TEST_RQ(_rval, _x, _cond)    if (_rval>=0) { (_rval=(_x)); DUF_TEST_RQ(_rval, _cond); }
#    define DUF_DO_TEST_RN(_rval, _x)   if (_rval>=0) { (_rval=(_x)); DUF_TEST_RN(_rval); }

#    define DUF_DO_TESTZ_R(_rval, _x)   { _rval=0;DUF_DO_TEST_R(_rval, _x) }

#    define DUF_DO_TEST_PR(_rval, _x)   if (_rval>=0) { _x; DUF_TEST_R(_rval); }
#  endif
/* ###################################################################### */

#  define DUF_START() {
#  define DUF_STARTTYP(typ, _rt) typ _rt=(typ)0; DUF_START()
#  define DUF_STARTR(_rt)         DUF_STARTTYP(int, _rt)
#  define DUF_STARTULL(_rt)       DUF_STARTTYP(unsigned long long, _rt)
#  define DUF_END() }
#  define DUF_ENDRET(_rt)                                                       DUF_END(); return _rt
#  define DUF_ENDR(_rt)           DUF_TEST_R(_rt);                                              DUF_ENDRET(_rt)
#  define DUF_ENDR_CLEAR(_rt,...) DUF_TEST_R(_rt); _rt=duf_clear_error( _rt, __VA_ARGS__, 0 );  DUF_ENDRET(_rt)
#  define DUF_ENDR_YES_CLEAR(_rt,...) \
  				  DUF_E_YES(__VA_ARGS__); DUF_TEST_R(_rt); \
					_rt=duf_clear_error( _rt, __VA_ARGS__, 0 ); \
												DUF_ENDRET(_rt)
#  define DUF_ENDRQ(_rt,_cond)    DUF_TEST_RQ(_rt,_cond);                                       DUF_ENDRET(_rt)
#  define DUF_ENDRN(_rt)          DUF_TEST_RN(_rt);                                             DUF_ENDRET(_rt)
#  define DUF_ENDR3(_rt3)         DUF_TEST_R( DUF_SQLITE_ERROR_CODE(_rt3) );                    DUF_ENDRET(_rt3)
#  define DUF_ENDULL(_rt)                                                                       DUF_ENDRET(_rt)
#  define DUF_ENDS(_s)                                                                          DUF_ENDRET(_s)

/* ###################################################################### */
/* ###################################################################### */

#  define T(_fmt, ...) DUF_TRACE(temp,0,"[T] #@#@# "_fmt,__VA_ARGS__)

/* ###################################################################### */

#  define DOR(_rval, _x)          DUF_DO_TEST_R(_rval, _x)
#  define DORQ(_rval, _x, _cond)  DUF_DO_TEST_RQ(_rval, _x, _cond)
#  define DORN(_rval, _x)         DUF_DO_TEST_RN(_rval, _x)
#  define DOZR(_rval, _x)         DUF_DO_TESTZ_R(_rval, _x)
#  define DOPR(_rval, _x)         DUF_DO_TEST_PR(_rval, _x)


#endif
