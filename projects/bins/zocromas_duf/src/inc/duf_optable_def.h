#ifndef MAS_DUF_OPTABLE_DEF_H
#  define MAS_DUF_OPTABLE_DEF_H

#  include "duf_option_types.h"
#  include "duf_config.h" /* duf_get_pointer_config */
#  include "duf_config_util.h" /* duf_get_pointer_ufilter */
/*
  for short (single-char) opts: add DO_V() or DO_VF()
             example: { .o = { .......  DO_V(SMART_HELP)} ... } ;;; DUF_OPTION_SMART_HELP should be defined as a char
 * */

#  define DO_A(_fld)    .has_arg = _fld ## _argument
#  define DOO_A(_fld) .o.has_arg = _fld ## _argument

#  define DO_A_O    DO_A(optional)
#  define DOO_A_O  DOO_A(optional)

#  define DO_A_R    DO_A(required)
#  define DOO_A_R  DOO_A(required)

#  define DO_A_N    DO_A(no)
#  define DOO_A_N  DOO_A(no)

#  define DO_SET_VAL(_val)    .val = _val
#  define DOO_SET_VAL(_val) .o.val = _val

#  define DO_VX( _vid, ... )   DO_SET_VAL( DUF_OPTION_VAL_ ##  __VA_ARGS__  ## _vid )
#  define DOO_VX( _vid, ... ) DOO_SET_VAL( DUF_OPTION_VAL_ ##  __VA_ARGS__  ## _vid )

#  define DO_V( _vid )   DO_VX( _vid )
#  define DOO_V( _vid ) DOO_VX( _vid )

#  define DO_VF( _vid )   DO_VX( _vid, FLAG_ )
#  define DOO_VF( _vid ) DOO_VX( _vid, FLAG_ )

#  define DO_VUF( _vid )   DO_VX( _vid, FILTER_ )
#  define DOO_VUF( _vid ) DOO_VX( _vid, FILTER_ )

#  define DO_SET_OCLASS(_ocl)  .oclass = _ocl
#  define DOO_SET_OCLASS(_ocl) .oclass = _ocl

#  define DO_CL( _cl )    DO_SET_OCLASS( DUF_OPTION_CLASS_ ## _cl )
#  define DOO_CL( _cl )  DOO_SET_OCLASS( DUF_OPTION_CLASS_ ## _cl )

/* #  define DO_VH( _vid ) DO_V( HELP_ ## _vid ) */

#  define DO_SET_NAME( _nam )    .name = _nam
#  define DOO_SET_NAME( _nam ) .o.name = _nam

#  define DO_N( _n )   DO_SET_NAME( # _n )
#  define DOO_N( _n ) DOO_SET_NAME( # _n )

#  define DO_Q( _n )   DO_SET_NAME( _n )
#  define DOO_Q( _n ) DOO_SET_NAME( _n )

#  define DOO_H( _h ) .help = # _h
#  define DO_H( _h )  .help = # _h

#  define DOO_HQ( _h ) .help = _h
#  define DO_HQ( _h )  .help = _h


#  define DO_SET_VTYPE(_vt)  .vtype=DUF_OPTION_VTYPE_ ## _vt
#  define DOO_SET_VTYPE(_vt) .vtype=DUF_OPTION_VTYPE_ ## _vt

#  define DO_T(_vt)   DO_SET_VTYPE(_vt)
#  define DOO_T(_vt) DOO_SET_VTYPE(_vt)

#  define DO_SET_CALL_TYPE(_ct)  .calltype=DUF_OPTION_CALL_TYPE_ ## _ct
#  define DOO_SET_CALL_TYPE(_ct) .calltype=DUF_OPTION_CALL_TYPE_ ## _ct

#  define DOC_CT(_ct)   DO_SET_CALL_TYPE(_ct)
#  define DOOC_CT(_ct) DOO_SET_CALL_TYPE(_ct)

#  define DO_TF(_vt, ...)   DO_SET_VTYPE(_vt),  DO_ ## _vt(__VA_ARGS__)
#  define DOO_TF(_vt, ...) DOO_SET_VTYPE(_vt), DOO_ ## _vt(__VA_ARGS__)

#if 0
#  define DO_OFFSET( _v, _styp, _fld )  .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_ ## _fld, .reltoptr= duf_get_pointer_##_fld
#  define DOO_OFFSET( _v, _styp, _fld ) .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_ ## _fld, .reltoptr= duf_get_pointer_##_fld
#else
#  define DO_OFFSET( _v, _styp, _fld )  .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_funcptr, .reltoptr= duf_get_pointer_##_fld
#  define DOO_OFFSET( _v, _styp, _fld ) .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_funcptr, .reltoptr= duf_get_pointer_##_fld
#endif


#  define DO_OOO(_vt, _v, _styp, _fld)     DO_SET_VTYPE(_vt), DO_OFFSET( _v, _styp, _fld )
#  define DOO_OOO(_vt, _v, _styp, _fld)   DOO_SET_VTYPE(_vt), DOO_OFFSET( _v, _styp, _fld )

#  define DO_OO(_vt, _v, _styp)	      DO_OOO(_vt, _v, _styp, _styp)
#  define DOO_OO(_vt, _v, _styp)      DOO_OOO(_vt, _v, _styp, _styp)

/* Offset pdi */
/* #  define DO_OP(_vt, _v)        DO_OO(_vt, _v, depthinfo) */
/* Offset config */
/* _vt is field at config, to get offset */
#  define DO_OC(_vt, _v)        DO_OO(_vt, _v, config)
#  define DOO_OC(_vt, _v)      DOO_OO(_vt, _v, config)


#  define DO_OI(_vt, _v)        DO_OO(_vt, _v, config_cli)
#  define DOO_OI(_vt, _v)      DOO_OO(_vt, _v, config_cli)

#  define DO_OT(_vt, _v)        DO_OO(_vt, _v, config_trace)
#  define DOO_OT(_vt, _v)      DOO_OO(_vt, _v, config_trace)


/* Offset ufilter */
#  define DO_OU(_vt, _v)        DO_OO(_vt, _v, ufilter)
#  define DOO_OU(_vt, _v)      DOO_OO(_vt, _v, ufilter)


#  define DO_BFL(_loc, _fld)             .flag_bitnum=DUF_FLAG_ ## _loc ## _ ## _fld, .can_no=1
#  define DOO_BFL(_vt, _prf, _loc, _fld) .flag_bitnum=DUF_FLAG_ ## _loc ## _ ## _fld, .can_no=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

#  define DO_BFS(_loc, _fld)             .flag_bitnum=DUF_FLAG_ ## _loc ## _ ## _fld, .can_no=0
#  define DOO_BFS(_vt, _prf, _loc, _fld) .flag_bitnum=DUF_FLAG_ ## _loc ## _ ## _fld, .can_no=0, DOO_A_N, DOO_OC(_vt, _prf._loc )

#  define DO_BFN(_loc,_fld)               DO_BFL(_loc,_fld),.unset=1
#  define DOO_BFN(_vt, _prf, _loc, _fld)  DOO_BFL(_vt, _prf,_loc,_fld),.unset=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

#  define DO_BFNS(_loc, _fld)             DO_BFS(_loc, _fld),.unset=1
#  define DOO_BFNS(_vt, _prf, _loc, _fld) DOO_BFS(_vt, _prf, _loc, _fld),.unset=1




/* --flag-option sets flag to 1, --no-flag-option sets flag to 0 */
/* DO_OC(NOFLAG, ...) + DO_FL eq. to DO_OC(FLAG, ...) + DO_FN */
#  define DO_FL(_loc, _fld)             .afl._loc={._fld=1}, .can_no=1
#  define DOO_FL(_vt, _prf, _loc, _fld) .afl._loc={._fld=1}, .can_no=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

#  define DO_FS(_loc, _fld)             .afl._loc={._fld=1}, .can_no=0
#  define DOO_FS(_vt, _prf, _loc, _fld) .afl._loc={._fld=1}, .can_no=0, DOO_A_N, DOO_OC(_vt, _prf._loc )

/* --flag-option sets flag to 0, --no-flag-option sets flag to 1 */
#  define DO_FN(_loc,_fld)               DO_FL(_loc,_fld),.unset=1
#  define DOO_FN(_vt, _prf, _loc, _fld) DOO_FL(_vt, _prf,_loc,_fld),.unset=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

#  define DO_FNS(_loc,_fld)               DO_FS(_loc,_fld),.unset=1
#  define DOO_FNS(_vt, _prf, _loc, _fld) DOO_FS(_vt, _prf,_loc,_fld),.unset=1, DOO_A_N, DOO_OC(_vt, _prf._loc )



#  define DO_SET_FUNC(_f)  .func=duf_option_O_ ## _f
#  define DOO_SET_FUNC(_f) .func=duf_option_O_ ## _f

#  define DO_XFUNC(_f)	    {  DO_SET_FUNC(_f)	   }
#  define DOO_XFUNC(_f)	    { DOO_SET_FUNC(_f)	   }

#  define DO_XFUNCA(_f, _a)  {  DO_SET_FUNC(_f), .arg=_a }
#  define DOO_XFUNCA(_f, _a) { DOO_SET_FUNC(_f), .arg=_a }

#  define DO_SET_CALLA(_id, _f, _a)	.call={.fdesc={ ._id= DO_XFUNCA(_f, _a) },.funcname= #_f}
#  define DOO_SET_CALLA(_id, _f, _a)	.call={.fdesc={ ._id=DOO_XFUNCA(_f, _a) },.funcname= #_f}

#  define DO_SET_CALL(_id, _f)		.call={.fdesc={ ._id= DO_XFUNC(_f) },.funcname= #_f}
#  define DOO_SET_CALL(_id, _f)		.call={.fdesc={ ._id=DOO_XFUNC(_f) },.funcname= #_f}


/* void f( .eia.arg ) -- call with numeric (int) arg from table (EIA:void-int-arg) */
#  define DO_EIA_CALL(_f, _a)		/* DO_SET_VTYPE( EIA_CALL ), */   DO_SET_CALL_TYPE( EIA ),  DO_SET_CALLA(eia, _f, _a)
#  define DOO_EIA_CALL(_f, _a)		/* DOO_SET_VTYPE( EIA_CALL ), */ DOO_SET_CALL_TYPE( EIA ), DOO_SET_CALLA(eia, _f, _a)

#  define DO_EIA_CALLH(_cl)		 DO_EIA_CALL( smart_help, DUF_OPTION_CLASS_ ## _cl)
#  define DOO_EIA_CALLH(_cl)		DOO_EIA_CALL( smart_help, DUF_OPTION_CLASS_ ## _cl)

#  define DO_EIA_CALLCL(_f, _cl)	 DO_EIA_CALL( _f, DUF_OPTION_CLASS_ ## _cl)
#  define DOO_EIA_CALLCL(_f, _cl)	DOO_EIA_CALL( _f, DUF_OPTION_CLASS_ ## _cl)


      /* void f( int argc, char *const *argv ) ;;;; call with carg[cv] (A:argv) */
#  define DO_A_CALL(_f)		/* DO_SET_VTYPE( A_CALL ), */   DO_SET_CALL_TYPE( A ),  DO_SET_CALL(a, _f)
#  define DOO_A_CALL(_f)	/* DOO_SET_VTYPE( A_CALL ), */ DOO_SET_CALL_TYPE( A ), DOO_SET_CALL(a, _f)


      /* void f( mas_cargvc_t arg ) ;;;; call with carg (AA:argv-argv) */
#  define DO_AA_CALL(_f)		/* DO_SET_VTYPE( A_CALL ), */   DO_SET_CALL_TYPE( A ),  DO_SET_CALL(aa, _f)
#  define DOO_AA_CALL(_f)		/* DOO_SET_VTYPE( A_CALL ), */ DOO_SET_CALL_TYPE( A ), DOO_SET_CALL(aa, _f)

      /* void f( &duf_config->targc, &duf_config->targv, duf_strtol_suff( optargg, &r ) ) ;;;; call with targ[cv] + numeric optarg (TN1: targ and numeric) */
#  define DO_TN1_CALL(_f)        /* DO_SET_VTYPE( TN1_CALL ), */   DO_SET_CALL_TYPE( TN1 ),  DO_SET_CALL(tn1, _f)
#  define DOO_TN1_CALL(_f)       /* DOO_SET_VTYPE( TN1_CALL ), */ DOO_SET_CALL_TYPE( TN1 ), DOO_SET_CALL(tn1, _f)


      /* void f( &duf_config->targc, &duf_config->targv, duf_strtol_suff( optargg, &r ) ) ;;;; call with targ[cv] + numeric optarg (TN2: targ and numeric) */
#  define DO_TN2_CALL(_f)        /* DO_SET_VTYPE( TN2_CALL ), */    DO_SET_CALL_TYPE( TN2 ),  DO_SET_CALL(tn2, _f)
#  define DOO_TN2_CALL(_f)        /* DOO_SET_VTYPE( TN2_CALL ), */ DOO_SET_CALL_TYPE( TN2 ), DOO_SET_CALL(tn2, _f)


      /* duf_error_code_t f( void ) ;;;; call with nothing (EV:errc-void) */
#  define DO_EV_CALL(_f)        /* DO_SET_VTYPE( EV_CALL ), */    DO_SET_CALL_TYPE( EV ),  DO_SET_CALL(ev, _f)
#  define DOO_EV_CALL(_f)        /* DOO_SET_VTYPE( EV_CALL ), */ DOO_SET_CALL_TYPE( EV ), DOO_SET_CALL(ev, _f)


      /* void f( duf_strtol_suff( optargg, &r ) ) ;;;; call with numeric optarg (N:numeric) */
#  define DO_N_CALL(_f)         /* DO_SET_VTYPE( N_CALL ), */    DO_SET_CALL_TYPE( N ),  DO_SET_CALL(n, _f)
#  define DOO_N_CALL(_f)         /* DOO_SET_VTYPE( N_CALL ), */ DOO_SET_CALL_TYPE( N ), DOO_SET_CALL(n, _f)


      /* void f( &duf_config->targc, &duf_config->targv, optargg ) ;;;; call with targ[cv] + string optarg (TS1: targ and string) */
#  define DO_TS1_CALL(_f)        /* DO_SET_VTYPE( TS1_CALL ), */    DO_SET_CALL_TYPE( TS1 ),  DO_SET_CALL(ts1, _f)
#  define DOO_TS1_CALL(_f)        /* DOO_SET_VTYPE( TS1_CALL ), */ DOO_SET_CALL_TYPE( TS1 ), DOO_SET_CALL(ts1, _f)

      /* void f( &duf_config->targc, &duf_config->targv, optargg ) ;;;; call with targ[cv] + string optarg (TS: targ and string) */
#  define DO_TS2_CALL(_f)        /* DO_SET_VTYPE( TS2_CALL ), */ DO_SET_CALL_TYPE( TS2 ), DO_SET_CALL(ts2, _f)
#  define DOO_TS2_CALL(_f)        /* DOO_SET_VTYPE( TS2_CALL ), */ DOO_SET_CALL_TYPE( TS2 ), DOO_SET_CALL(ts2, _f)


      /* void f( optargg ) ;;;; call with string optarg (S: string) */
#  define DO_S_CALL(_f)         /* DO_SET_VTYPE( S_CALL ), */    DO_SET_CALL_TYPE( S ),  DO_SET_CALL(s, _f)
#  define DOO_S_CALL(_f)         /* DOO_SET_VTYPE( S_CALL ), */ DOO_SET_CALL_TYPE( S ), DOO_SET_CALL(s, _f)


      /* void f( .sas.arg ) -- ;;;; call with constant string arg from table (SAS:void-string-sarg) */
#  define DO_SAS_CALL(_f, _a)		/* DO_SET_VTYPE( SAS_CALL ), */   DO_SET_CALL_TYPE( SAS ),  DO_SET_CALLA(sas, _f, _a)
#  define DOO_SAS_CALL(_f, _a)		/* DOO_SET_VTYPE( SAS_CALL ), */ DOO_SET_CALL_TYPE( SAS ), DOO_SET_CALLA(sas, _f, _a)

      /* void f( optargg, .san.arg ) -- ;;;; call with constant string arg from table (SAS:void-string-narg) */
#  define DO_SAN_CALL(_f, _a)		/* DO_SET_VTYPE( SAN_CALL ), */   DO_SET_CALL_TYPE( SAN ),  DO_SET_CALLA(san, _f, _a)
#  define DOO_SAN_CALL(_f, _a)		/* DOO_SET_VTYPE( SAN_CALL ), */ DOO_SET_CALL_TYPE( SAN ), DOO_SET_CALLA(san, _f, _a)


#  define DO_TDB( ) 	DO_SET_VTYPE( TDB )
#  define DOO_TDB( ) 	DOO_SET_VTYPE( TDB )


#  define DO_SET_STAGE(_min, _max)  .stage_opts.stage={.min= DUF_OPTION_STAGE_ ## _min, .max= DUF_OPTION_STAGE_ ## _max},.stage_opts.use_stage=1
#  define DOO_SET_STAGE(_min, _max) .stage_opts.stage={.min= DUF_OPTION_STAGE_ ## _min, .max= DUF_OPTION_STAGE_ ## _max},.stage_opts.use_stage=1

#  define DO_STAGES(_min, _max)   DO_SET_STAGE(_min, _max)
#  define DOO_STAGES(_min, _max)  DOO_SET_STAGE(_min, _max)

#  define DO_AT_STAGE(_stag)	 DO_SET_STAGE(_stag, _stag)
#  define DOO_AT_STAGE(_stag)	DOO_SET_STAGE(_stag, _stag)


#  define DO_STAGE_SPLS		 DO_SET_STAGE(SETUP, MAX)
#  define DOO_STAGE_SPLS	DOO_SET_STAGE(SETUP, MAX)

#  define DO_STAGE_IPLS		 DO_SET_STAGE(INIT, MAX)
#  define DOO_STAGE_IPLS	DOO_SET_STAGE(INIT, MAX)

#  define DO_STAGE_ALL		 DO_SET_STAGE(DEBUG, MAX)
#  define DOO_STAGE_ALL		DOO_SET_STAGE(DEBUG, MAX)


#  define DO_STG_MASK(_v)  .stage_opts.use_stage_mask=1, .stage_opts.stage_mask= _v
#  define DOO_STG_MASK(_v) .stage_opts.use_stage_mask=1, .stage_opts.stage_mask= _v


#  define DO_STG_NOT(_v)   DO_STG_MASK( (1<< DUF_OPTION_STAGE_ ## _v ) )
#  define DOO_STG_NOT(_v) DOO_STG_MASK( (1<< DUF_OPTION_STAGE_ ## _v ) )


#  define DO_CMD(_otxt, _oarg, _ocode , ...)  {.o={DO_Q(_otxt), DO_A_ ## _oarg , DO_V(_ocode)}, __VA_ARGS__}
#  define DOO_CMD(_otxt, _oarg, _ocode , ...) {DOO_Q(_otxt), DOO_A_ ##  _oarg , DOO_V(_ocode), __VA_ARGS__}

/* #  define DO_CMD2(_o, ...) {.o=_o, __VA_ARGS__} */


#endif
