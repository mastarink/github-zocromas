#ifndef MASXFS_DEFS_H
# define MASXFS_DEFS_H
# include <stddef.h>
# include <errno.h>

# define _uUu_ __attribute__(( unused ))

# ifndef TRUE
#  define TRUE (1)
# endif
# ifndef FALSE
#  define FALSE (0)
# endif

# define QRLI(_li, r)	{ if (r<0) { if(_li)_li->error=r;RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }}
# define QRPI(_pi, r)	{ if (r<0) { if(_pi)_pi->error=r;RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }}

#endif
