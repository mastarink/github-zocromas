#ifndef MAS_UNIDATA_TYPES_H
#  define MAS_UNIDATA_TYPES_H

#include <unistd.h>

#  include <mastar/types/mas_evaluate_types.h>


typedef struct mas_unidata_s mas_unidata_t;
struct mas_unidata_s
{
  mas_content_type_t icontent_type;
  size_t size;
  mas_evaluated_t *data;
};


#endif
