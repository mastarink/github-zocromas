#ifndef MASREGERR_DEFS_H
# define MASREGERR_DEFS_H
# ifndef MAS_PACKAGE
#  define MAS_PACKAGE PACKAGE
# endif
# ifndef MAS_PACKAGE_NAME
#  define MAS_PACKAGE_NAME PACKAGE_NAME
# endif

# include <stddef.h>
# include <errno.h>

# define _uUu_ __attribute__(( unused ))

# ifndef TRUE
#  define TRUE (1)
# endif
# ifndef FALSE
#  define FALSE (0)
# endif

# ifndef RGEMSG
#  define RGEMSG strerror(errno)
# endif
/* # define MAS_FLF __func__,__LINE__,__FILE__ */
# define MAS_FLF __FUNCTION__,__LINE__,__FILE__
# define MAS_FLFFF __FUNCTION__,__LINE__,__FILE__,__func__,__PRETTY_FUNCTION__
# define RGEX(_sys, _msg) masregerr_reg(NULL, MAS_FLFFF, MAS_PACKAGE_NAME, &errno, errno?_sys:0, _msg)
# define RGE RGEX(0, NULL)
# define RGES RGEX(1, RGEMSG)
# define RGER(_r)	{ if ((_r)<0) RGE; }
# define RGESR(_r)	{ if ((_r)<0) RGES; }

# define MASREGERR_DEFAULT_SIZE 256

#endif
