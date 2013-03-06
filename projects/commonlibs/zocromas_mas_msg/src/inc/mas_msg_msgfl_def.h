#ifndef MAS_MSG_MSGFL_DEF_H
#  define MAS_MSG_MSGFL_DEF_H

#  ifndef MAS_NOMSG
#    define MAS_MSGFL(...) {  if (&mas_msg) mas_msg(FL, __VA_ARGS__); }
#    define MAS_ERRFL(...) {  if (&mas_error) mas_error(FL, __VA_ARGS__); }
#  else
#    define MAS_MSGFL(...)
#  endif

#endif

