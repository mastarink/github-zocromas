#ifndef MASXFS_LEVINFO_DEFS_H
# define MASXFS_LEVINFO_DEFS_H

# include <stddef.h>
# include <errno.h>

# define _uUu_ __attribute__(( unused ))

# ifndef TRUE
#  define TRUE (1)
# endif
# ifndef FALSE
#  define FALSE (0)
# endif

# include <mastar/regerr/masregerr_defs.h>
# include <mastar/regerr/masregerr.h>

/* # define QRLI(_li, _r)   { int r=(_r);if (r<0) { if(_li)_li->error=(r);RWARN("R: %d; errno:%d:%s", (r), errno, strerror(errno)); }} */
# define QRLI(_li, _r) RGESR(_r)
/* # define QRPI(_pi, _r)  { int r=(_r);if (r<0) { if(_pi)_pi->error=(r);RWARN("R: %d; errno:%d:%s", (r), errno, strerror(errno)); }} */
/* # define QRPI(_pi, _r) RGESR(_r) */
# define QRG(_r) RGESR(_r)

#endif
