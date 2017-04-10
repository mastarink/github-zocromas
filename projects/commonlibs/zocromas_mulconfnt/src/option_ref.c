#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_structs.h"

int
mucs_config_option_npos( mucs_option_t * opt )
{
  return opt ? opt->npos : 0;
}

const char *
mucs_config_option_string_value( mucs_option_t * opt )
{
  return opt ? opt->string_value : NULL;
}

void
mucs_config_option_set_string_value_na( mucs_option_t * opt, char *string_value )
{
  if ( opt )
  {
    if ( opt->string_value )
      mas_free( opt->string_value );
    opt->string_value = NULL;
    if ( string_value )
      opt->string_value = string_value ;
  }
}
void
mucs_config_option_set_string_value( mucs_option_t * opt, const char *string_value )
{
  if ( opt )
  {
    if ( opt->string_value )
      mas_free( opt->string_value );
    opt->string_value = NULL;
    if ( string_value )
      opt->string_value = mas_strdup( string_value );
  }
}

