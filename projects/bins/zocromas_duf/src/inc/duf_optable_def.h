#ifndef DUF_OPTABLE_DEF_H
#  define DUF_OPTABLE_DEF_H

/*
  for short (single-char) opts: add DO_V() or DO_VF()
             example: { .o = { .......  DO_V(SMART_HELP)} ... } ;;; DUF_OPTION_SMART_HELP should be defined as a char
 * */

#  define DO_A_X(_fld) .has_arg = _fld ## _argument
#  define DO_A_O  DO_A_X(optional)
#  define DO_A_R  DO_A_X(required)
#  define DO_A_N  DO_A_X(no)

#  define DO_SET_VAL(_val) .val = _val
#  define DO_VX( _vid, ... ) DO_SET_VAL( DUF_OPTION ##  __VA_ARGS__ ## _ ## _vid )
#  define DO_V( _vid )  DO_VX( _vid )
#  define DO_VF( _vid ) DO_VX( _vid, _FLAG )

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
#  define DO_OC(_vt, _v)        DO_OO(_vt, _v, config)
#  define DO_OU(_vt, _v)	DO_OO(_vt, _v, ufilter)
#  define DO_OPDI(_vt, _v)	DO_OO(_vt, _v, depthinfo)


#  define DO_FL(_t,_fld) .afl._t={._fld=1}
                                         /* ,.setit=1 */
#  define DO_FN(_t,_fld) DO_FL(_t,_fld),.invert=1
/* #define DO_FL0(_t,_fld) .anfl._t={._fld=1} */
#  define DO_INTERACTIVE DO_FL( act, interactive )

#  define DO_SET_FUNC(_f) .func=duf_option_ ## _f
#  define DO_XFUNC(_f)	    { DO_SET_FUNC(_f)	   }
#  define DO_XFUNCA(_f, _a) { DO_SET_FUNC(_f), .arg=_a }

#  define DO_SET_CALLA(_id, _f, _a)	.call={.fdesc={ ._id=DO_XFUNCA(_f, _a) }}
#  define DO_SET_CALL(_id, _f)		.call={.fdesc={ ._id=DO_XFUNC(_f)      }}

#  define DO_VI_CALL(_f, _a) DO_SET_CALLA(vi, _f, _a)
#  define DO_VI_CALLH(cl)	DO_VI_CALL( smart_help, DUF_OPTION_CLASS_ ## cl)
#  define DO_A_CALL(_f)		DO_SET_CALL(a, _f)
#  define DO_T_CALL(_f)         DO_SET_CALL(t, _f)
#  define DO_TN_CALL(_f)        DO_SET_CALL(tn, _f)
#  define DO_TS_CALL(_f)        DO_SET_CALL(ts, _f)

#  define DO_SET_STAGE(_min, _max) .stage={.min=_min, .max=_max}
#  define DO_AT_STAGE(_stag)	DO_SET_STAGE(_stag, _stag)
#  define DO_STAGE_ANY()	DO_SET_STAGE(0, -1)

#endif
