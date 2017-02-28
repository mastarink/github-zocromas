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

#include <mastar/regerr/masregerr.h>

# define QRG(_r) RGESR(_r)

#endif
