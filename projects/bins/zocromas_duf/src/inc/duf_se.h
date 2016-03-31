#ifndef MAS_DUF_SE_H
# define MAS_DUF_SE_H

# include <mastar/wrap/mas_std_def.h>
# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ▤ */
/* # include "duf_start_end.h" */

/* #include "duf_debug_defs.h" 
 *
 * DUF_STARTTYP ~ DEBUG_STARTTYP
 * DUF_ENDRET   ~ DEBUG_ENDRET
 * */
typedef enum
{
  DUF_LAYER_NONE,
  DUF_LAYER_TOP,
  DUF_LAYER_CONFIG,
  DUF_LAYER_OPTIONS,
  DUF_LAYER_SNIPPET_OPTION,
  DUF_LAYER_PDI,
  DUF_LAYER_SCCB,
  DUF_LAYER_SCCBH,
  DUF_LAYER_LI,
  DUF_LAYER_PI,
  DUF_LAYER_SQL,
  DUF_LAYER_MOD,
  DUF_LAYER_OTHER,
} duf_layer_t;

/* typedef struct             */
/* {                          */
/*   duf_layer_t layer_id;    */
/*   union                    */
/*   {                        */
/*     int r;                 */
/*     mas_error_index_t ei;  */
/*     unsigned long long ul; */
/*   } r;                     */
/* } mas_fundecl_t;           */

# define QT(_fmt, ...) MAST_TRACE(temp,0,"@"_fmt,__VA_ARGS__)
# define QTR if (QISERR) {QT("@@@@@@(i:%d:c:%d) %s", QERRIND, QERRCODE, QERRNAME);}

/*
 * Q... - query
 * F... - function
 * T... - test
 * D... - declare
 * S... - start; set
 * SERR... - set error
 * E... - end
 * C... - call
 * */
# define QSTR(_name) #_name

# define F2N(_pref, _funname) _pref ## _funname
# define F2ND(_funname) F2N(duf_,_funname)
/* # define F2NW(_pref, _funname) DUF_WRAPPED( _pref ## _funname ) */

# define STT {
# define STTT(_typ, _rt) _rt=(_typ)0; STT

# define T2T(_typid) typeof((*((mas_fundecl_t *)NULL)).r._typid)

# define DT( _layer_id, _pre, _typid, _funname, ... )                    T2T(_typid)  F2ND(_funname)( __VA_ARGS__ )
# define DTX( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... ) _funtyp F2ND(_funname)( __VA_ARGS__ )
# define DTN( _layer_id, _funtyp,               _pre, _typid, _funname, ... ) _funtyp F2ND(_funname)( __VA_ARGS__ )
# define DTP( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... ) _funtyp F2ND(_funname)( __VA_ARGS__, T2T(_typid) *prrrrrr_ )
# define DTP0( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname )     _funtyp F2ND(_funname)( T2T(_typid) *prrrrrr_ )

# define ST( _layer_id, _pre, _typid, _funname, ... ) DT( _layer_id, , _typid, _funname, __VA_ARGS__ ) \
			{ \
			  mas_fundecl_t fundecl_={.layer_id= DUF_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )
# define STX( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... )  DTX( _layer_id, _funtyp, _rvar, _vini, , _typid, _funname, __VA_ARGS__ ) \
			{ /* S */ \
			  _funtyp _rvar=((_funtyp)0); _rvar=_vini; \
			  mas_fundecl_t fundecl_={.layer_id= DUF_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )
# define STN( _layer_id, _funtyp, _pre, _typid, _funname, ... )  DTN( _layer_id, _funtyp,  , _typid, _funname, __VA_ARGS__ ) \
			{ /* S */ \
			  mas_fundecl_t fundecl_={.layer_id= DUF_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )
# define STP( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... )  DTP( _layer_id, _funtyp, _rvar, _vini, , _typid, _funname, __VA_ARGS__ ) \
			{ /* S */ \
			  _funtyp _rvar=((_funtyp)0); _rvar=_vini; \
			  mas_fundecl_t fundecl_={.layer_id= DUF_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )
# define STP0( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname ) DTP0( _layer_id, _funtyp, _rvar, _vini, , _typid, _funname ) \
			{ /* S */ \
			  _funtyp _rvar=((_funtyp)0); _rvar=_vini; \
			  mas_fundecl_t fundecl_={.layer_id= DUF_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )

# define ETT }
# define ERT(_rt)	ETT;	return _rt
# define ET( _layer_id, _pre, _typid, _funname, ... ) \
			  _pre; \
			  ERT( fundecl_.r._typid );\
			} \
			DT( _layer_id, , _typid, _funname, __VA_ARGS__ );
# define ETP( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... ) \
			  _pre; \
			  ETT;\
			} /* E */ \
  			if (  prrrrrr_  ) \
			  *prrrrrr_=fundecl_.r._typid; \
  			DTP( _layer_id, _funtyp, _rvar, _vini, , _typid, _funname, __VA_ARGS__ ); \
			return _rvar;
# define ETX( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... ) \
			  _pre; \
			  ETT;\
			} /* E */ \
  			DTX( _layer_id, _funtyp, _rvar, _vini, , _typid, _funname, __VA_ARGS__ ); \
			return _rvar;
# define ETN( _layer_id, _funtyp,               _pre, _typid, _funname, ... ) \
			  _pre; \
			  ETT;\
			} /* E */ \
  			DTN( _layer_id, _funtyp,               , _typid, _funname, __VA_ARGS__ );
# define ETP0( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname ) \
			  _pre; \
			  ETT;\
			} /* E */ \
  			if (  prrrrrr_  ) \
			  *prrrrrr_=fundecl_.r._typid; \
			DTP0( _layer_id, _funtyp, _rvar, _vini, , _typid, _funname ); \
			return _rvar;

# define DR( _layer_id, _funname, ... )		DT( _layer_id,				,	ei, _funname, __VA_ARGS__ )
# define DRX( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	DTX( _layer_id, _funtyp, _rvar, _vini,	 ,	ei, _funname, __VA_ARGS__ )
# define DRN( _layer_id, _funtyp,               _funname, ... )	DTN( _layer_id, _funtyp,              	 ,	ei, _funname, __VA_ARGS__ )
# define DRP( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	DTP( _layer_id, _funtyp, _rvar, _vini,	 ,	ei, _funname, __VA_ARGS__ )

# define SR( _layer_id, _funname, ... )		ST( _layer_id,				,	ei, _funname, __VA_ARGS__ )
# define SRX( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	STX( _layer_id, _funtyp, _rvar, _vini,			,	ei, _funname, __VA_ARGS__ )
# define SRN( _layer_id, _funtyp,               _funname, ... )	STN( _layer_id, _funtyp, 			,	ei, _funname, __VA_ARGS__ )
# define SRP( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	STP( _layer_id, _funtyp, _rvar, _vini,			,	ei, _funname, __VA_ARGS__ )
# define SRP0( _layer_id, _funtyp, _rvar, _vini, _funname )	STP0( _layer_id, _funtyp, _rvar, _vini,			,	ei, _funname )

# define ER( _layer_id, _funname, ... )		ET( _layer_id, TER,	ei, _funname, __VA_ARGS__ )
# define ERX( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	ETX( _layer_id, _funtyp, _rvar, _vini, TER,	ei, _funname, __VA_ARGS__ )
# define ERN( _layer_id, _funtyp,               _funname, ... )	ETN( _layer_id, _funtyp,               TER,	ei, _funname, __VA_ARGS__ )
# define ERP( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	ETP( _layer_id, _funtyp, _rvar, _vini, TER,	ei, _funname, __VA_ARGS__ )
# define ERP0( _layer_id, _funtyp, _rvar, _vini, _funname )	ETP0( _layer_id, _funtyp, _rvar, _vini, TER,	ei, _funname )
# define CRX(           _funname, ... )		F2ND(_funname)( __VA_ARGS__ )
# define CRP( _funname, ... )			CRX( _funname, __VA_ARGS__, QPERRIND )
# define CR(            _funname, ... )		QDOCR( CRX(_funname, __VA_ARGS__ ) )
# define CRV(           _fun    , ... )		QDOCR( (_fun)( __VA_ARGS__) )
# define IF_CR( _funname, ...)    		(F2ND(_funname)) ? CR(_funname, __VA_ARGS__) : ERRMAKE( NO_FUNC )
# define IF_CRV( _fun, ...)    			(_fun) ? CRV((_fun), __VA_ARGS__) : ERRMAKE( NO_FUNC )

#endif
