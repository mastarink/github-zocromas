#ifndef MAS_MUC_SE_H
# define MAS_MUC_SE_H

# include <mastar/wrap/mas_std_def.h>
# include <mastar/error/mas_error_types.h>
/* # define QERRIND fundecl_.r.ei */
/* # define QPERRIND &(fundecl_.r.ei) */

/* # include "muc_start_end.h" */

/* #include "muc_debug_defs.h" 
 *
 * MUC_STARTTYP ~ DEBUG_STARTTYP
 * MUC_ENDRET   ~ DEBUG_ENDRET
 * */
typedef enum
{
  MUC_LAYER_NONE,
  MUC_LAYER_TOP,
  MUC_LAYER_OPTIONS,
  MUC_LAYER_SNIPPET_OPTION,
  MUC_LAYER_PDI,
  MUC_LAYER_SCCB,
} muc_layer_t;

typedef struct
{
  muc_layer_t layer_id;
  union
  {
    int r;
    mas_error_index_t ei;
    unsigned long long ul;
  } r;
} muc_fundecl_t;

# define muc_QT(_fmt, ...) MAST_TRACE(temp,0,"@"_fmt,__VA_ARGS__)
# define muc_QTR if (QISERR) {muc_QT("@@@@@@(i:%d:c:%d) %s", QERRIND, QERRCODE, QERRNAME);}

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
# define muc_QSTR(_name) #_name

# define muc_F2N(_pref, _funname) _pref ## _funname
/* # define F2NW(_pref, _funname) DUF_WRAPPED( _pref ## _funname ) */

# define muc_STT {
# define muc_STTT(_typ, _rt) _rt=(_typ)0; muc_STT

# define muc_T2T(_typid) typeof((*((muc_fundecl_t *)NULL)).r._typid)

# define muc_DT( _layer_id, _pre, _typid, _funname, ... ) muc_T2T(_typid) muc_F2N(muc_,_funname)( __VA_ARGS__ )
# define muc_DTP( _layer_id, _pre, _typid, _funname, ... ) muc_F2N(muc_,_funname)( __VA_ARGS__, muc_T2T(_typid) *prrrrrr_ )

# define muc_ST( _layer_id, _pre, _typid, _funname, ... ) muc_DT( _layer_id, , _typid, _funname, __VA_ARGS__ ) \
			{ \
			  muc_fundecl_t fundecl_={.layer_id= MUC_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  muc_STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )
# define muc_STP( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... ) _funtyp muc_DTP( _layer_id, , _typid, _funname, __VA_ARGS__ ) \
			{ /* S */ \
			  _funtyp _rvar=((_funtyp)0); _rvar=_vini; \
			  muc_fundecl_t fundecl_={.layer_id= MUC_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  muc_STTT( typeof(fundecl_.r._typid), fundecl_.r._typid )

# define muc_ETT }
# define muc_ERT(_rt)	muc_ETT;	return _rt
# define muc_ET( _layer_id, _pre, _typid, _funname, ... ) \
			  _pre; \
			  muc_ERT( fundecl_.r._typid );\
			} \
			muc_DT( _layer_id, , _typid, _funname, __VA_ARGS__ );
# define muc_ETP( _layer_id, _funtyp, _rvar, _vini, _pre, _typid, _funname, ... ) \
			  _pre; \
			  muc_ETT;\
			} /* E */ \
  			if (  prrrrrr_  ) \
			  *prrrrrr_=fundecl_.r._typid; \
			_funtyp muc_DTP( _layer_id, , _typid, _funname, __VA_ARGS__ ); \
			return _rvar;

# define muc_DR( _layer_id, _funname, ... )		muc_DT( _layer_id,				,	ei, _funname, __VA_ARGS__ )
# define muc_DRP( _layer_id, _funname, ... )	muc_DTP( _layer_id,				,	ei, _funname, __VA_ARGS__ )

# define muc_SR( _layer_id, _funname, ... )		muc_ST( _layer_id,				,	ei, _funname, __VA_ARGS__ )
# define muc_SRP( _layer_id, _funtyp, _rvar, _vini, _funname, ... ) \
  	muc_STP( _layer_id, _funtyp, _rvar, _vini,			,	ei, _funname, __VA_ARGS__ )

# define muc_ER( _layer_id, _funname, ... )		muc_ET( _layer_id, TER,	ei, _funname, __VA_ARGS__ )
# define muc_ERP( _layer_id, _funtyp, _rvar, _vini, _funname, ... )	muc_ETP( _layer_id, _funtyp, _rvar, _vini, TER,	ei, _funname, __VA_ARGS__ )
# define muc_DOCR(_rval, _x)			( (_rval>=0) ? ( (_rval=(_x))  ) : 0 )
# define muc_CR(            _funname, ... )		muc_DOCR( QERRIND, muc_F2N(muc_,_funname)( __VA_ARGS__ ) )
# define muc_CRV(           _fun    , ... )		muc_DOCR( QERRIND, (_fun)( __VA_ARGS__) )
/* # define CRW(           _funname, ... )         muc_DOCR( QERRIND, F2NW(muc_,_funname)( __VA_ARGS__)) */
/* # define muc_IF_CR( _fun, ...)                      { if (_fun) muc_CR(_fun, __VA_ARGS__); else ERRMAKE( NO_FUNC ); } */
# define muc_IF_CR( _funname, ...)    		(muc_F2N(muc_,_funname)) ? muc_CR(_funname, __VA_ARGS__) : ERRMAKE( NO_FUNC )
# define muc_IF_CRV( _fun, ...)    			(_fun) ? muc_CRV((_fun), __VA_ARGS__) : ERRMAKE( NO_FUNC )

/* following error macros ==> new error lib's mas_se.h */
#endif
