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
# define MAS_FLF __func__,__LINE__,__FILE__
# define MAS_FLFFF __func__,__LINE__,__FILE__,__FUNCTION__,__PRETTY_FUNCTION__
# define RGEX(_sys, _msg) masregerr_reg(NULL, MAS_FLFFF, MAS_PACKAGE_NAME, &errno, errno?_sys:0, _msg)
# define RGE RGEX(0, NULL)
# define RGES RGEX(1, RGEMSG)
# define RGER(_r)	{ if ((_r)<0) RGE; }
# define RGESR(_r)	{ if ((_r)<0) RGES; }

# define MASREGERR_DEFAULT_SIZE 256


# if 0
#  define rDECL(_v) int r_cnt _uUu_=0,r=(_v)
#  define rC(_x) if (!r_cnt || !r) { r=(_x); r_cnt++; }
#  define rCODE r
#  define rRET return r
# else
#  define rCODE ret_code.r
#  define rCNT ret_code.cnt
#  define rRET return rCODE
#  define rDECL(_v) struct {int cnt;int r;} ret_code={0}; rCODE=(_v)
/* #  define rC(_x) if (!ret_code.cnt || !ret_code.r) { ret_code.r=(_x); ret_code.cnt++; } */
#  define rC(_x)  (!rCNT || !rCODE) ?( rCNT++,rCODE=(_x)):rCNT
# endif
/* #define rCALL if (!r) r= */


#endif
