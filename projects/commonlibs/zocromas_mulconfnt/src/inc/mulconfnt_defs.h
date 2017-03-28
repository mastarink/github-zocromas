#ifndef MULCONF_DEFS_H
# define MULCONF_DEFS_H

/* # define _uUu_ __attribute__((unused)) */
/* extern int do_fprintf; */

# include <mastar/regerr/masregerr_defs.h>
# include <mastar/regerr/masregerr.h>

# define QRG(_r) RGESR(_r)
# define QRGSRC(_osrc, _r)  QRG(_r)
# define QRGOPT(_opt, _r)  QRG(_r)

#endif
