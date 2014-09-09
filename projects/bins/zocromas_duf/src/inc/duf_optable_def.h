#ifndef DUF_OPTABLE_DEF_H
#define DUF_OPTABLE_DEF_H


#define DO_CL( cl )  .oclass = DUF_OPTION_CLASS_ ## cl
#define DO_V( vid )  .val = DUF_OPTION_ ## vid
#define DO_VH( vid ) DO_V( HELP_ ## vid )
#define DO_VF( vid )  .val = DUF_OPTION_FLAG_ ## vid
#define DO_A_O .has_arg = optional_argument
#define DO_A_R .has_arg = required_argument
#define DO_A_N .has_arg = no_argument
#define DO_N( n ) .name = # n
#define DO_Q( n ) .name = n
#define DO_H( h ) .help = # h
#define DO_T(vt) .vtype=DUF_OPTION_VTYPE_ ## vt
#define DO_O(vt, v) DO_T(vt),.mf=1,.m = offsetof(duf_config_t,v)
#define DO_FL(t,fld) .afl.t={.fld=1}
#define DO_FL0(t,fld) .anfl.t={.fld=1}
#define DO_VIFUN(f, a) .call={.fdesc={.vi={.func=f, .arg=a}}}
#define DO_AFUN(f) .call={.fdesc={.a={.func=f}}}


#endif
