#include <stdio.h>
#include <errno.h>

#include "mas_control_constructor.h"

#define _uUu_  __attribute__((unused))

void
mas_ctrl_constructor( const char *fname, int lin _uUu_, int lev _uUu_)
{
  fprintf( stderr, "\n%s CONstructor %s e%d @", lev == 0 ? "--------------------" : "********************", fname, errno );
}

void
mas_ctrl_destructor( const char *fname, int lin _uUu_, int lev _uUu_ )
{
  fprintf( stderr, "\n%s DEstructor %s e%d @", lev == 0 ? "--------------------" : "********************", fname, errno );
}


