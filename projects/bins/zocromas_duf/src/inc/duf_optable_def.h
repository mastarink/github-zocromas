#ifndef DUF_OPTABLE_DEF_H
#define DUF_OPTABLE_DEF_H


#define DO_CL( _cl )  .oclass = DUF_OPTION_CLASS_ ## _cl
#define DO_V( _vid )  .val = DUF_OPTION_ ## _vid
#define DO_VH( _vid ) DO_V( HELP_ ## _vid )
#define DO_VF( _vid )  .val = DUF_OPTION_FLAG_ ## _vid
#define DO_A_O .has_arg = optional_argument
#define DO_A_R .has_arg = required_argument
#define DO_A_N .has_arg = no_argument
#define DO_N( _n ) .name = # _n
#define DO_Q( _n ) .name = _n
#define DO_H( _h ) .help = # _h
#define DO_T(_vt) .vtype=DUF_OPTION_VTYPE_ ## _vt

#define DO_OOO(_vt, _v, _styp, _fld)   DO_T(_vt), .m_hasoff=1,.m_offset = offsetof(duf_## _styp ## _t,_v),   .relto=DUF_OFFSET_ ## _fld
#define DO_OO(_vt, _v, _styp)	DO_OOO(_vt, _v, _styp, _styp)
#define DO_O(_vt, _v)           DO_OO(_vt, _v, config)
#define DO_OU(_vt, _v)		DO_OO(_vt, _v, ufilter)
#define DO_OPDI(_vt, _v)	DO_OO(_vt, _v, depthinfo)

#define DO_FL(_t,_fld) .afl._t={._fld=1},.setit=1
/* #define DO_FL0(_t,_fld) .anfl._t={._fld=1} */
#define DO_VIFUN(_f, _a) .call={.fdesc={.vi={.func=_f, .arg=_a}}}
#define DO_AFUN(_f) .call={.fdesc={.a={.func=_f}}}
#define DO_TFUN(_f) .call={.fdesc={.t={.func=_f}}}
#define DO_STAGE(_stag) .stage={.min=_stag,.max=_stag}
#define DO_STAGE_ANY() .stage={.min=0,.max=-1}

#endif
