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
#  define IEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
    if (_rv<0) { if (ctrl.error_handler) { _rv=(ctrl.error_handler)(FL, _rv, errno, fmt, msg); } } \
  } \
}

#  define EEVALM(_rv, _code, fmt, msg) \
{ \
  _rv = ( _code ); \
  if (_rv) { if (ctrl.error_handler) { _rv=(ctrl.error_handler)(FL, -1, _rv, fmt, msg); } } \
}

#endif



#  define IEVAL_OPT(_rv, _code) { if (!(_rv<0)) { _rv = ( _code ); } }
#  define IEVAL(_rv, _code) {IEVALM(_rv, _code, NULL, NULL); }
#  define EEVAL(_rv, _code) {EEVALM(_rv, _code, NULL, NULL); }

#endif
