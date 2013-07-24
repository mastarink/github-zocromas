#ifndef MAS_COMMON_DEFS_H
#  define MAS_COMMON_DEFS_H


/* #define MAS_NOPASS_OPTS */

/* #  ifdef MAS_NOPASS_OPTS                                                                     */
/* #    define  MAS_PASS_OPTS_DECLARE mas_options_t * _popts,                                   */
/* #    define  MAS_PASS_OPTS_DECLARE1 mas_options_t * _popts                                   */
/* #    define MAS_PASS_OPTS_DCLOFF 1                                                           */
/* #    define MAS_PASS_OPTS_PASS NULL,                                                         */
/* #    define MAS_PASS_OPTS_PASS1 NULL                                                         */
/* #    define MAS_PASS_OPTS_DECL_PREF extern mas_options_t gopts __attribute__ ( ( unused ) )  */
/* #    define MAS_PASS_OPTS_DECL_GPREF extern mas_options_t gopts __attribute__ ( ( unused ) ) */
/* #    define MAS_PASS_OPTS_REF &gopts                                                         */
/* #    define MAS_PASS_OPTS_PREF gopts.                                                        */
/* #    define MAS_PASS_OPTS_GPREF gopts.                                                       */
/* #    define MAS_PASS_OPTS_DECL_GREF                                                          */
/* #    define MAS_PASS_OPTS_GREF NULL,                                                         */
/* #    define MAS_PASS_OPTS_GREF1 NULL                                                         */
/* #  else                                                                                      */
/* #    define  MAS_PASS_OPTS_DECLARE mas_options_t * popts,                                    */
/* #    define  MAS_PASS_OPTS_DECLARE1 mas_options_t * popts                                    */
/* #    define MAS_PASS_OPTS_DCLOFF 1                                                           */
/* #    define MAS_PASS_OPTS_PASS popts,                                                        */
/* #    define MAS_PASS_OPTS_PASS1 popts                                                        */
/* #    define MAS_PASS_OPTS_DECL_PREF                                                          */
/* #    define MAS_PASS_OPTS_DECL_GPREF extern mas_options_t gopts __attribute__ ( ( unused ) ) */
/* #    define MAS_PASS_OPTS_REF popts                                                          */
/* #    define MAS_PASS_OPTS_PREF popts->                                                       */
/* #    define MAS_PASS_OPTS_GPREF gopts.                                                       */
/* #    define MAS_PASS_OPTS_DECL_GREF extern mas_options_t gopts __attribute__ ( ( unused ) )  */
/* #    define MAS_PASS_OPTS_GREF &gopts,                                                       */
/* #    define MAS_PASS_OPTS_GREF1 &gopts                                                       */
/* #  endif                                                                                     */



#  ifdef MAS_NO_CTRL
#    define MAS_CTRL_IS_CLIENT 1
#    define MAS_CTRL_IS_SERVER 0
#    define MAS_CTRL_IN_CLIENT 1
#    define MAS_CTRL_IN_SERVER 0
#    define MAS_CTRL_MESSAGES 1
#    define MAS_CTRL_STATUS 0
#    define IEVALME(_rv, _code, pserrno, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
  } \
}
#    define IEVALM(_rv, _code, fmt, msg) IEVALME(_rv, _code, NULL, fmt, msg)
#    define YEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
  } \
}


#    define EEVALM(_rv, _code, fmt, msg) \
{ \
  _rv = ( _code ); \
}
#  else
#    define MAS_CTRL_IS_SERVER ctrl.is_server
#    define MAS_CTRL_IS_CLIENT ctrl.is_client
#    define MAS_CTRL_IN_SERVER ctrl.in_server
#    define MAS_CTRL_IN_CLIENT ctrl.in_client
#    define MAS_CTRL_MESSAGES ( ctrl.messages || !ctrl.inited )
#    define MAS_CTRL_STATUS ctrl.status

#    define ERRREG(fmt,msg) (ctrl.error_handler)(FL, 0, -1, 0, NULL,NULL, fmt, msg)
#    define ERRRG(msg) (ctrl.error_handler)(FL, 0, -1, 0, NULL,NULL, "%s", msg)
#    define _ERRHAN(sys,rv,merrno,perrno,pserrno,fmt,msg) \
	{ if (ctrl.error_handler) { rv=(ctrl.error_handler)(FL, sys, rv, merrno, perrno, pserrno, fmt, msg); } }
#    define IEVALME(_rv, _code, pserrno, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
    if (_rv<0) { _ERRHAN(0,_rv,errno,&errno,pserrno,fmt,msg) } \
  } \
}
#    define IEVALM(_rv, _code, fmt, msg) IEVALME(_rv, _code, NULL, fmt, msg)
#    define YEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
    if (_rv<0) { _ERRHAN(1,_rv,errno,&errno,NULL,fmt,msg) } \
  } \
}


#    define EEVALM(_rv, _code, fmt, msg) \
{ \
  _rv = ( _code ); \
  if (_rv) { int merr=_rv;_rv=-1;_ERRHAN(0,_rv,merr,NULL,NULL,fmt,msg) } \
}

#    include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;

#  endif



#  define EEVAL(_rv, _code) {EEVALM(_rv, _code, NULL, NULL); }

#  define IEVAL_OPT(_rv, _code) { if (!(_rv<0)) { _rv = ( _code ); } }
#  define IEVAL(_rv, _code) {IEVALM(_rv, _code, NULL/*fmt*/, NULL /*msg*/); }
#  define IEVALE(_rv, _code, pserrno) {IEVALME(_rv, _code, pserrno, NULL/*fmt*/, NULL/*fmt*/); }

#  define YEVAL_OPT(_rv, _code) { if (!(_rv<0)) { _rv = ( _code ); } }
#  define YEVAL(_rv, _code) {YEVALM(_rv, _code, NULL, NULL); }







#endif
