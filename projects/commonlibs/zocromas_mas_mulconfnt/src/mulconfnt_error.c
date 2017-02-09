#include <stdio.h>
#include "mulconfnt_error.h"

static int _mulconfnt_error = 0;
void
mulconfnt_set_error( int line, const char *func )
{
  fprintf (stderr, "SETTING ERROR: %d:%s", line, func);
  _mulconfnt_error++;
}

int
mulconfnt_error( void )
{
  return _mulconfnt_error;
}
