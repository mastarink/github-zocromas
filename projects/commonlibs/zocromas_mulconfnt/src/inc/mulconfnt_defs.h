#ifndef MULCONF_DEFS_H
# define MULCONF_DEFS_H

# include <mastar/regerr/masregerr_defs.h>
# include <mastar/regerr/masregerr.h>

# define QRG(_r) (RGESR(_r))
# define QRGM(_r,  ...) (RGESRM(_r, __VA_ARGS__))
# define QRGSRC(_osrc, _r)  (QRG(_r))
# define QRGOPT(_opt, _r)  (QRG(_r))

# define QRGSRCM(_osrc, _r, ...)  ( mucs_source_check(_osrc),       QRGM(_r, __VA_ARGS__))
# define QRGOPTM(_opt, _r,  ...)  ( mucs_config_option_check(_opt), QRGM(_r, __VA_ARGS__))

#endif
