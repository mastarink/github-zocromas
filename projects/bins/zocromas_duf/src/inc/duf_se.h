#ifndef MAS_DUF_SE_H
# define MAS_DUF_SE_H

# include <mastar/error/mas_error_types.h>
# include "duf_start_end.h"

/* #include "duf_debug_defs.h" 
 *
 * DUF_STARTTYP ~ DEBUG_STARTTYP
 * DUF_ENDRET   ~ DEBUG_ENDRET
 * */
typedef enum
{
  DUF_LAYER_NONE,
  DUF_LAYER_TOP,
  DUF_LAYER_OPTIONS,
  DUF_LAYER_SNIPPET_OPTION,
  DUF_LAYER_PDI,
  DUF_LAYER_SCCB,
} duf_layer_t;

typedef struct
{
  duf_layer_t layer_id;
  union
  {
    int r;
    mas_error_index_t ei;
    unsigned long long ul;
  } r;
} duf_fundecl_t;

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
# define QERRIND fundecl_.r.ei
# define QPERRIND &(fundecl_.r.ei)
# define F2N(_funname) duf_ ## _funname
# define F2NW(_funname) DUF_WRAPPED( duf_ ## _funname )
/* #  define F2C(_funname, ...) F2N(_funname)(__VA_ARGS__) */
/* #  define F2CW(_funname, ...) DUF_WRAPPED( duf_ ## _funname )(__VA_ARGS__) */
# define T2T(_typid) typeof((*((duf_fundecl_t *)NULL)).r._typid)
# define DT( _layer_id, _pre, _typid, _funname, ... ) T2T(_typid) F2N(_funname)( __VA_ARGS__ )
# define ST( _layer_id, _pre, _typid, _funname, ... ) DT( _layer_id, , _typid, _funname, __VA_ARGS__ ) \
			{ \
			  duf_fundecl_t fundecl_={.layer_id= DUF_LAYER_ ## _layer_id,.r={.ul=0}}; \
  			  DUF_STARTT( typeof(fundecl_.r._typid), fundecl_.r._typid )

# define ET( _layer_id, _pre, _typid, _funname, ... ) \
			  _pre; \
			  DUF_ENDRET( fundecl_.r._typid );\
			} \
			DT( _layer_id, , _typid, _funname, __VA_ARGS__ );

# define DR( _layer_id, _funname, ... ) DT( _layer_id,				,	ei, _funname, __VA_ARGS__ )
# define SR( _layer_id, _funname, ... ) ST( _layer_id,				,	ei, _funname, __VA_ARGS__ )
# define ER( _layer_id, _funname, ... ) ET( _layer_id, DUF_TEST_R( QERRIND ),	ei, _funname, __VA_ARGS__ )
# define DOCR(_rval, _x) ( (_rval>=0) ? ( (_rval=(_x))  ) : 0 )
# define CR(            _funname, ... ) DOCR( QERRIND, F2N(_funname)( __VA_ARGS__ ) )
# define CRV(           _fun    , ... ) DOCR( QERRIND, (_fun)( __VA_ARGS__) )
# define CRW(		 _funname, ... ) DOCR( QERRIND, F2NW(_funname)( __VA_ARGS__))
# define SERRV(_err) DUF_MAKE_ERROR( QERRIND, _err)
# define SERRMV(_err, ...) DUF_MAKE_ERRORM( QERRIND, _err, __VA_ARGS__ )
# define SERR(_err)  SERRV( DUF_ERROR_ ## _err)
# define SERRM(_err, ...)  SERRMV( DUF_ERROR_ ## _err, __VA_ARGS__ )
# define QNOERR  DUF_NOERROR( QERRIND )
# define QERRNAME mas_error_name_i( QERRIND )

#endif
