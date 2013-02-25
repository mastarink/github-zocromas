#ifndef MAS_COMMON_DEFS_H
#  define MAS_COMMON_DEFS_H

#  define IEVAL(_rv, _code) if (!(_rv<0)){ _rv = ( _code ); if (_rv<0) { if (ctrl.error_handler) { (ctrl.error_handler)(FL, _rv); } } };

#endif
