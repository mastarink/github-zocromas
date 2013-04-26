#ifndef MAS_COMMON_DEFS_H
#  define MAS_COMMON_DEFS_H

#ifdef MAS_NO_CTRL
#define MAS_CTRL_IS_CLIENT 1
#define MAS_CTRL_IS_SERVER 0
#define MAS_CTRL_IN_CLIENT 1
#define MAS_CTRL_IN_SERVER 0
#define MAS_CTRL_MESSAGES 1
#define MAS_CTRL_STATUS 0
#  define IEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
  } \
}
#  define YEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
  } \
}


#  define EEVALM(_rv, _code, fmt, msg) \
{ \
  _rv = ( _code ); \
}
#else
#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;
#define MAS_CTRL_IS_SERVER ctrl.is_server
#define MAS_CTRL_IS_CLIENT ctrl.is_client
#define MAS_CTRL_IN_SERVER ctrl.in_server
#define MAS_CTRL_IN_CLIENT ctrl.in_client
#define MAS_CTRL_MESSAGES ctrl.messages
#define MAS_CTRL_STATUS ctrl.status

#define ERRREG(fmt,msg) (ctrl.error_handler)(FL, 0, -1, 0, NULL, fmt, msg)
#define ERRRG(msg) (ctrl.error_handler)(FL, 0, -1, 0, NULL, "%s", msg)
#define _ERRHAN(sys,rv,merrno,perrno,fmt,msg) { if (ctrl.error_handler) { rv=(ctrl.error_handler)(FL, sys, rv, merrno, perrno, fmt, msg); } }
#  define IEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
    if (_rv<0) { _ERRHAN(0,_rv,errno,&errno,fmt,msg) } \
  } \
}
#  define YEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
    if (_rv<0) { _ERRHAN(1,_rv,errno,&errno,fmt,msg) } \
  } \
}


#  define EEVALM(_rv, _code, fmt, msg) \
{ \
  _rv = ( _code ); \
  if (_rv) { int merr=_rv;_rv=-1;_ERRHAN(0,_rv,merr,NULL,fmt,msg) } \
}

#endif


#  define EEVAL(_rv, _code) {EEVALM(_rv, _code, NULL, NULL); }

#  define IEVAL_OPT(_rv, _code) { if (!(_rv<0)) { _rv = ( _code ); } }
#  define IEVAL(_rv, _code) {IEVALM(_rv, _code, NULL, NULL); }

#  define YEVAL_OPT(_rv, _code) { if (!(_rv<0)) { _rv = ( _code ); } }
#  define YEVAL(_rv, _code) {YEVALM(_rv, _code, NULL, NULL); }


#endif
