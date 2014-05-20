#ifndef MAS_DUF_DBG_H
#  define MAS_DUF_DBG_H

typedef enum
{
  DBG_START = 1000,
  DBG_STEP,
  DBG_STEPS,
  DBG_STEPIS,
  DBG_STEPI2S,
  DBG_STEPULL,
  DBG_ENDR,
  DBG_ENDULL,
  DBG_ENDS,
  DBG_ENDRS,
  DBG_END,
} duf_dbgcode_t;

int duf_dbgfunc( duf_dbgcode_t code, const char *func, int line, ... );

#endif
