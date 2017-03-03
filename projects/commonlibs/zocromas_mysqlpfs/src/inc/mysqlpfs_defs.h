#ifndef MAS_MYSQLPFS_DEFS_H
# define MAS_MYSQLPFS_DEFS_H

# include <stddef.h>
# include <errno.h>

# define _uUu_ __attribute__(( unused ))

# ifndef TRUE
#  define TRUE (1)
# endif
# ifndef FALSE
#  define FALSE (0)
# endif

# define QRGS(_r) RGESR(_r?-1:0)
# define QRG(_r) RGER(_r)

# define QRGSP(_ptr)   QRGS( _ptr ? 0 : -1 );
# define QRGP(_ptr)    QRG( _ptr ? 0 : -1 );

# define rSET(_v) int r_cnt _uUu_=0,r=(_v)
# define rC(_x) if (!r_cnt || !r) { r=(_x); r_cnt++; }
/* #define rCALL if (!r) r= */

# include <mastar/regerr/masregerr_defs.h>

#endif
