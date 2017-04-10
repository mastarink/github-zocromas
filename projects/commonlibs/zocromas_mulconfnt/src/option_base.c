#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error_base.h" */
/* #include "mulconfnt_error.h"      */

#include "option_base.h"

/*
 *
 * mucs_config_option_...
 *
 * */


void
mucs_config_option_init( mucs_option_t * opt )
{
  memset( opt, 0, sizeof( mucs_option_t ) );
}

mucs_option_t *
mucs_config_option_create( void )
{
  mucs_option_t *opt = mas_calloc( 1,sizeof( mucs_option_t ) );

  mucs_config_option_init( opt );
  return opt;
}

void
mucs_config_option_reset( mucs_option_t * opt )
{
  if ( opt )
  {
    if ( opt->s.name )
      mas_free( opt->s.name );
    if ( opt->s.desc )
      mas_free( opt->s.desc );
    if ( opt->s.argdesc )
      mas_free( opt->s.argdesc );
    if ( opt->d.string_value )
      mas_free( opt->d.string_value );
    /* mucs_error_reset( &opt->error ); */
  }
}

void
mucs_config_option_delete( mucs_option_t * opt )
{
  mucs_config_option_reset( opt );
  if ( opt )
    mas_free( opt );
}

mucs_option_t *
mucs_config_option_clone( const mucs_option_t * topt )
{
  mucs_option_t *opt = mucs_config_option_create(  );

  *opt = *topt;
  opt->s.name = mas_strdup( opt->s.name );
  opt->s.desc = mas_strdup( opt->s.desc );
  opt->s.argdesc = mas_strdup( opt->s.argdesc );
  opt->d.string_value = mas_strdup( opt->d.string_value );
  return opt;
}
