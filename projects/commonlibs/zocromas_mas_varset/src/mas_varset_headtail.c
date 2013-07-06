#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_varset_types.h>

#  include <mastar/varvec/mas_varvec_object.h>
#  include <mastar/varvec/mas_varvec.h>
#  include <mastar/varvec/mas_varvec_search.h>
#  include <mastar/varvec/mas_varvec_namevalue.h>
#  include <mastar/varvec/mas_varvec_headtail.h>

#include "mas_varset_object.h"
#include "mas_varset.h"
#include "mas_varset_search.h"

#include "mas_varset_headtail.h"

mas_varset_t *
mas_varset_set_head( mas_varset_t * varset, const char *vclass_name, const char *head )
{
  mas_varvec_t *vclass;

  vclass = mas_varset_search_vclass( varset, vclass_name );
  _mas_varvec_set_head( vclass, head );
  return varset;
}

mas_varset_t *
mas_varset_add_tail( mas_varset_t * varset, const char *vclass_name, const char *tail )
{
  mas_varvec_t *vclass;

  vclass = mas_varset_search_vclass( varset, vclass_name );
  _mas_varvec_add_tail( vclass, tail );
  return varset;
}
