#ifndef MAS_DUF_OPTIONS_ENUM_H
#  define MAS_DUF_OPTIONS_ENUM_H

#  define DUF_CONFIG_DIR_FROM_ENV "MSHDIRS_CONF_DIR"
#  define DUF_CONFIG_FILE_NAME "zocromas_duf"

/* #  define DUF_DECLARE_OPTION(_name)                                         */
/* #  define DUF_DECLARE_OPTION_V(_name, _val) DUF_OPTION_VAL_ ## _name = _val */

typedef enum
{
#  define ENUM_WRAP(_n)       DUF_OPTION_VAL_ ## _n,
#  define ENUM_WRAPP(_n, _rf, _rf2)       DUF_OPTION_VAL_ ## _n,
#  define ENUM_WRAP_V(_n, _v) DUF_OPTION_VAL_ ## _n = _v,
#  define ENUM_WRAP_VP(_n, _v, _rf, _rf2) DUF_OPTION_VAL_ ## _n = _v,
#  include "duf_options_enum.def"
#  undef ENUM_WRAP
#  undef ENUM_WRAPP
#  undef ENUM_WRAP_V
#  undef ENUM_WRAP_VP
} duf_option_code_t;

#endif

/*
vi: ft=c
*/
