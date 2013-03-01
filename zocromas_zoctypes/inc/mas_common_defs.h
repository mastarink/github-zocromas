#ifndef MAS_COMMON_DEFS_H
#  define MAS_COMMON_DEFS_H

#  define IEVALM(_rv, _code, fmt, msg) \
{ \
  if (!(_rv<0)) \
  { \
    _rv = ( _code ); \
    if (_rv<0) { if (ctrl.error_handler) { _rv=(ctrl.error_handler)(FL, _rv, errno, fmt, msg); } } \
  } \
}
#  define IEVAL_OPT(_rv, _code) { if (!(_rv<0)) { _rv = ( _code ); } }
#  define IEVAL(_rv, _code) {IEVALM(_rv, _code, NULL, NULL); }

#  define EEVALM(_rv, _code, fmt, msg) \
{ \
  _rv = ( _code ); \
  if (_rv) { if (ctrl.error_handler) { _rv=(ctrl.error_handler)(FL, -1, _rv, fmt, msg); } } \
}
#  define EEVAL(_rv, _code) {EEVALM(_rv, _code, NULL, NULL); }


#endif
