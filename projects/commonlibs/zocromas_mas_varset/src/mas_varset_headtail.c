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
#include "mas_varset_vclass_object.h"
#include "mas_varset_vclass.h"
#include "mas_varset_vclass_search.h"
#include "mas_varset_vclass_namevalue.h"
#include "mas_varset_vclass_headtail.h"


#include "mas_varset_object.h"
#include "mas_varset.h"
#include "mas_varset_search.h"

#include "mas_varset_headtail.h"

mas_varset_t *
mas_varset_set_head( mas_varset_t * varset, const char *vclass_name, const char *head )
{
  mas_varset_class_t *vclass = NULL;

  vclass = mas_varset_search_vclass( varset, vclass_name );
  _mas_varset_vclass_set_head( vclass, head );
  return varset;
}

mas_varset_t *
mas_varset_add_tail( mas_varset_t * varset, const char *vclass_name, const char *tail )
{
  mas_varset_class_t *vclass = NULL;

  vclass = mas_varset_search_vclass( varset, vclass_name );
  _mas_varset_vclass_add_tail( vclass, tail );
  return varset;
}

