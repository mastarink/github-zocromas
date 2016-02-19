#include <stddef.h>                                                  /* NULL */

/* ###################################################################### */
#include "duf_option_val.h"
/* ###################################################################### */

/* TODO : depends on optimpl !! */
const char *
duf_codeval2string( duf_option_code_t codeval )
{
  const char *rs = NULL;

  switch ( codeval )
  {
#define ENUM_WRAP(_n)                   case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAPP(_n, _rf, _rf2)       case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v)             case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAP_VP(_n, _v, _rf, _rf2) case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#include "duf_options_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAPP
#undef ENUM_WRAP_V
#undef ENUM_WRAP_VP
  }
  return rs;
}
