#ifndef MASREGERR_DEFS_H
# define MASREGERR_DEFS_H

# include <stddef.h>
# include <errno.h>

# define _uUu_ __attribute__(( unused ))

# ifndef TRUE
#  define TRUE (1)
# endif
# ifndef FALSE
#  define FALSE (0)
# endif

# define MAS_FLF __func__,__LINE__,__FILE__
# define RGEX(_sys, _msg) masregerr_reg(NULL, MAS_FLF, PACKAGE, &errno, errno?_sys:0, _msg)
# define RGE RGEX(0, NULL)
# define RGES RGEX(1, NULL)
# define RGER(_r)	{ if ((_r)<0) RGE; }
# define RGESR(_r)	{ if ((_r)<0) RGES; }

# define MASREGERR_DEFAULT_SIZE 256

#endif
