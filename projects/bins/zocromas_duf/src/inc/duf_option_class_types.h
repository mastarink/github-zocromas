#ifndef MAS_DUF_OPTION_CLASS_TYPES_H
#define MAS_DUF_OPTION_CLASS_TYPES_H

typedef enum
{
# define ENUM_WRAP(_n, ...)       DUF_OPTION_CLASS_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) DUF_OPTION_CLASS_ ## _n = _v, */
# define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
# include "duf_option_class_enum.def"
# undef ENUM_WRAP
# undef ENUM_WRAP_V
} duf_option_class_t;



#endif
