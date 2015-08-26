#ifndef DUF_OPTABLE_DEF_H
#  define DUF_OPTABLE_DEF_H

#  include "duf_option_types.h"
/*
  for short (single-char) opts: add DO_V() or DO_VF()
             example: { .o = { .......  DO_V(SMART_HELP)} ... } ;;; DUF_OPTION_SMART_HELP should be defined as a char
 * */

#  define DO_A_X(_fld) .has_arg = _fld ## _argument
#  define DO_A_O  DO_A_X(optional)
#  define DO_A_R  DO_A_X(required)
#  define DO_A_N  DO_A_X(no)

#  define DO_SET_VAL(_val) .val = _val
#  define DO_VX( _vid, ... ) DO_SET_VAL( DUF_OPTION_VAL_ ##  __VA_ARGS__  ## _vid )
#  define DO_V( _vid )  DO_VX( _vid )
#  define DO_VF( _vid ) DO_VX( _vid, FLAG_ )

#  define DO_SET_OCLASS(_ocl) .oclass = _ocl
#  define DO_CL( _cl )  DO_SET_OCLASS( DUF_OPTION_CLASS_ ## _cl )
/* #  define DO_VH( _vid ) DO_V( HELP_ ## _vid ) */

#  define DO_SET_NAME( _nam ) .name = _nam
#  define DO_N( _n ) DO_SET_NAME( # _n )
#  define DO_Q( _n ) DO_SET_NAME( _n )
#  define DO_H( _h ) .help = # _h


#  define DO_SET_VTYPE(_vt) .vtype=DUF_OPTION_VTYPE_ ## _vt
#  define DO_T(_vt) DO_SET_VTYPE(_vt)

#  define DO_TF(_vt, ...) DO_SET_VTYPE(_vt), DO_ ## _vt(__VA_ARGS__)

#  define DO_OFFSET( _v, _styp, _fld ) .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_ ## _fld
#  define DO_OOO(_vt, _v, _styp, _fld)   DO_SET_VTYPE(_vt), DO_OFFSET( _v, _styp, _fld )
#  define DO_OO(_vt, _v, _styp)	DO_OOO(_vt, _v, _styp, _styp)
#  define DO_OP(_vt, _v)        DO_OO(_vt, _v, depthinfo)
#  define DO_OC(_vt, _v)        DO_OO(_vt, _v, config)
#  define DO_OU(_vt, _v)	DO_OO(_vt, _v, ufilter)
#  define DO_ODI(_vt, _v)	DO_OO(_vt, _v, depthinfo)


#  define DO_FL(_t,_fld) .afl._t={._fld=1}
                                         /* ,.setit=1 */
#  define DO_FN(_t,_fld) DO_FL(_t,_fld),.invert=1
/* #define DO_FL0(_t,_fld) .anfl._t={._fld=1} */
/* #  define DO_INTERACTIVE DO_FL( act, interactive ) */

#  define DO_SET_FUNC(_f) .func=duf_option_$_ ## _f
#  define DO_XFUNC(_f)	    { DO_SET_FUNC(_f)	   }
#  define DO_XFUNCA(_f, _a) { DO_SET_FUNC(_f), .arg=_a }

#  define DO_SET_CALLA(_id, _f, _a)	.call={.fdesc={ ._id=DO_XFUNCA(_f, _a) }}
#  define DO_SET_CALL(_id, _f)		.call={.fdesc={ ._id=DO_XFUNC(_f)      }}

/* void f( .via.arg ) -- */
#  define DO_VIA_CALL(_f, _a)		DO_SET_VTYPE( VIA_CALL ), DO_SET_CALLA(via, _f, _a)
#  define DO_VIA_CALLH(_cl)		DO_VIA_CALL( smart_help, DUF_OPTION_CLASS_ ## _cl)
#  define DO_VIA_CALLCL(_f, _cl)	DO_VIA_CALL( _f, DUF_OPTION_CLASS_ ## _cl)
/* void f( int argc, char *const *argv ) */
#  define DO_A_CALL(_f)		DO_SET_VTYPE( A_CALL ), DO_SET_CALL(a, _f)
/* void f( duf_cargvc_t arg ) */
#  define DO_AA_CALL(_f)		DO_SET_VTYPE( A_CALL ), DO_SET_CALL(aa, _f)
/*#define DO_T_CALL(_f)         DO_SET_VTYPE( T_CALL ), DO_SET_CALL(t, _f) */
/* void f( &duf_config->targc, &duf_config->targv, duf_strtol_suff( optargg, &r ) ) */
#  define DO_TN_CALL(_f)        DO_SET_VTYPE( TN_CALL ), DO_SET_CALL(tn, _f)
/* void f( void ) */
#  define DO_VV_CALL(_f)        DO_SET_VTYPE( VV_CALL ), DO_SET_CALL(vv, _f)
/* void f( duf_strtol_suff( optargg, &r ) ) */
#  define DO_N_CALL(_f)         DO_SET_VTYPE( N_CALL ), DO_SET_CALL(n, _f)
/* void f( &duf_config->targc, &duf_config->targv, optargg ) */
#  define DO_TS_CALL(_f)        DO_SET_VTYPE( TS_CALL ), DO_SET_CALL(ts, _f)
/* void f( optargg ) */
#  define DO_S_CALL(_f)         DO_SET_VTYPE( S_CALL ), DO_SET_CALL(s, _f)
/* void f( .vsa.arg ) -- */
#  define DO_VSA_CALL(_f, _a)		DO_SET_VTYPE( VSA_CALL ), DO_SET_CALLA(vsa, _f, _a)
#  define DO_TDB( ) 	DO_SET_VTYPE( TDB )

#  define DO_SET_STAGE(_min, _max) .stage={.min= DUF_OPTION_STAGE_ ## _min, .max= DUF_OPTION_STAGE_ ## _max},.use_stage=1
#  define DO_AT_STAGE(_stag)	DO_SET_STAGE(_stag, _stag)
#  define DO_STAGE_ANY		DO_SET_STAGE(DEFAULT, ANY)

#  define DO_STG_MASK(_v) .use_stage_mask=1, .stage_mask= _v
#  define DO_STG_NOT(_v) DO_STG_MASK( (1<< DUF_OPTION_STAGE_ ## _v ) )

#endif
