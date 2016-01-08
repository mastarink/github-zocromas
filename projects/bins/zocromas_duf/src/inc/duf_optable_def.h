#ifndef MAS_DUF_OPTABLE_DEF_H
#  define MAS_DUF_OPTABLE_DEF_H

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
#  define DO_VUF( _vid ) DO_VX( _vid, FILTER_ )

#  define DO_SET_OCLASS(_ocl) .oclass = _ocl
#  define DO_CL( _cl )  DO_SET_OCLASS( DUF_OPTION_CLASS_ ## _cl )
/* #  define DO_VH( _vid ) DO_V( HELP_ ## _vid ) */

#  define DO_SET_NAME( _nam ) .name = _nam
#  define DO_N( _n ) DO_SET_NAME( # _n )
#  define DO_Q( _n ) DO_SET_NAME( _n )
#  define DO_H( _h ) .help = # _h


#  define DO_SET_VTYPE(_vt) .vtype=DUF_OPTION_VTYPE_ ## _vt
#  define DO_T(_vt) DO_SET_VTYPE(_vt)
#  define DO_SET_CALL_TYPE(_ct) .calltype=DUF_OPTION_CALL_TYPE_ ## _ct
#  define DOC_CT(_ct) DO_SET_CALL_TYPE(_ct)

#  define DO_TF(_vt, ...) DO_SET_VTYPE(_vt), DO_ ## _vt(__VA_ARGS__)

#  define DO_OFFSET( _v, _styp, _fld ) .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_ ## _fld
#  define DO_OOO(_vt, _v, _styp, _fld)   DO_SET_VTYPE(_vt), DO_OFFSET( _v, _styp, _fld )
#  define DO_OO(_vt, _v, _styp)	DO_OOO(_vt, _v, _styp, _styp)

/* Offset pdi */
/* #  define DO_OP(_vt, _v)        DO_OO(_vt, _v, depthinfo) */
/* Offset config */
#  define DO_OC(_vt, _v)        DO_OO(_vt, _v, config)



/* Offset ufilter */
/* #  define DO_OU(_vt, _v)        DO_OOO(_vt, scn.puz._v, config, config) */
/* #  define DO_OU(_vt, _v)        DO_OOO(_vt, _v, ufilter, ufilter) */
#  define DO_OU(_vt, _v)        DO_OO(_vt, _v, ufilter)




/* #  define DO_ODI(_vt, _v)       DO_OO(_vt, _v, depthinfo) */


#  define DO_FL(_t,_fld) .afl._t={._fld=1}, .can_no=1
                                         /* ,.setit=1 */
#  define DO_FN(_t,_fld) DO_FL(_t,_fld),.invert=1
/* #define DO_FL0(_t,_fld) .anfl._t={._fld=1} */
/* #  define DO_INTERACTIVE DO_FL( act, interactive ) */

#  define DO_SET_FUNC(_f) .func=duf_option_O_ ## _f
#  define DO_XFUNC(_f)	    { DO_SET_FUNC(_f)	   }
#  define DO_XFUNCA(_f, _a) { DO_SET_FUNC(_f), .arg=_a }

#  define DO_SET_CALLA(_id, _f, _a)	.call={.fdesc={ ._id=DO_XFUNCA(_f, _a) },.funcname= #_f}
#  define DO_SET_CALL(_id, _f)		.call={.fdesc={ ._id=DO_XFUNC(_f) },.funcname= #_f}


/* void f( .eia.arg ) -- call with numeric (int) arg from table (EIA:void-int-arg) */
#  define DO_EIA_CALL(_f, _a)		/* DO_SET_VTYPE( EIA_CALL ), */ DO_SET_CALL_TYPE( EIA ), DO_SET_CALLA(eia, _f, _a)
#  define DO_EIA_CALLH(_cl)		DO_EIA_CALL( smart_help, DUF_OPTION_CLASS_ ## _cl) /**/
#  define DO_EIA_CALLCL(_f, _cl)	DO_EIA_CALL( _f, DUF_OPTION_CLASS_ ## _cl) /**/
      /* */
      /* void f( int argc, char *const *argv ) ;;;; call with carg[cv] (A:argv) */
#  define DO_A_CALL(_f)		/* DO_SET_VTYPE( A_CALL ), */ DO_SET_CALL_TYPE( A ), DO_SET_CALL(a, _f) /**/
      /* */
      /* void f( mas_cargvc_t arg ) ;;;; call with carg (AA:argv-argv) */
#  define DO_AA_CALL(_f)		/* DO_SET_VTYPE( A_CALL ), */ DO_SET_CALL_TYPE( A ), DO_SET_CALL(aa, _f) /**/
/*#define DO_T_CALL(_f)         DO_SET_VTYPE( T_CALL ), DO_SET_CALL_TYPE( T ), DO_SET_CALL(t, _f) */
      /* */
      /* void f( &duf_config->targc, &duf_config->targv, duf_strtol_suff( optargg, &r ) ) ;;;; call with targ[cv] + numeric optarg (TN1: targ and numeric) */
#  define DO_TN1_CALL(_f)        /* DO_SET_VTYPE( TN1_CALL ), */ DO_SET_CALL_TYPE( TN1 ), DO_SET_CALL(tn1, _f) /**/
      /* void f( &duf_config->targc, &duf_config->targv, duf_strtol_suff( optargg, &r ) ) ;;;; call with targ[cv] + numeric optarg (TN2: targ and numeric) */
#  define DO_TN2_CALL(_f)        /* DO_SET_VTYPE( TN2_CALL ), */ DO_SET_CALL_TYPE( TN2 ), DO_SET_CALL(tn2, _f) /**/
      /* */
      /* duf_error_code_t f( void ) ;;;; call with nothing (EV:errc-void) */
#  define DO_EV_CALL(_f)        /* DO_SET_VTYPE( EV_CALL ), */ DO_SET_CALL_TYPE( EV ), DO_SET_CALL(ev, _f) /**/
      /* */
      /* void f( duf_strtol_suff( optargg, &r ) ) ;;;; call with numeric optarg (N:numeric) */
#  define DO_N_CALL(_f)         /* DO_SET_VTYPE( N_CALL ), */ DO_SET_CALL_TYPE( N ), DO_SET_CALL(n, _f) /**/
      /* */
      /* void f( &duf_config->targc, &duf_config->targv, optargg ) ;;;; call with targ[cv] + string optarg (TS1: targ and string) */
#  define DO_TS1_CALL(_f)        /* DO_SET_VTYPE( TS1_CALL ), */ DO_SET_CALL_TYPE( TS1 ), DO_SET_CALL(ts1, _f) /**/
      /* void f( &duf_config->targc, &duf_config->targv, optargg ) ;;;; call with targ[cv] + string optarg (TS: targ and string) */
#  define DO_TS2_CALL(_f)        /* DO_SET_VTYPE( TS2_CALL ), */ DO_SET_CALL_TYPE( TS2 ), DO_SET_CALL(ts2, _f) /**/
      /* */
      /* void f( optargg ) ;;;; call with string optarg (S: string) */
#  define DO_S_CALL(_f)         /* DO_SET_VTYPE( S_CALL ), */ DO_SET_CALL_TYPE( S ), DO_SET_CALL(s, _f) /**/
      /* */
      /* void f( .sas.arg ) -- ;;;; call with constant string arg from table (SAS:void-string-sarg) */
#  define DO_SAS_CALL(_f, _a)		/* DO_SET_VTYPE( SAS_CALL ), */ DO_SET_CALL_TYPE( SAS ), DO_SET_CALLA(sas, _f, _a) /**/
      /* void f( optargg, .san.arg ) -- ;;;; call with constant string arg from table (SAS:void-string-narg) */
#  define DO_SAN_CALL(_f, _a)		/* DO_SET_VTYPE( SAN_CALL ), */ DO_SET_CALL_TYPE( SAN ), DO_SET_CALLA(san, _f, _a) /**/
      /* */
#  define DO_TDB( ) 	DO_SET_VTYPE( TDB ) /**/
      /* */
#  define DO_SET_STAGE(_min, _max) .stage={.min= DUF_OPTION_STAGE_ ## _min, .max= DUF_OPTION_STAGE_ ## _max},.use_stage=1 /**/
      /* */
#  define DO_AT_STAGE(_stag)	DO_SET_STAGE(_stag, _stag) /**/
      /* */
#  define DO_STAGE_ANY		DO_SET_STAGE(SETUP, ANY) /**/
      /* */
#  define DO_STG_MASK(_v) .use_stage_mask=1, .stage_mask= _v /**/
      /* */
#  define DO_STG_NOT(_v) DO_STG_MASK( (1<< DUF_OPTION_STAGE_ ## _v ) ) /**/
#endif
