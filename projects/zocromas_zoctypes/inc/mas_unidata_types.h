#ifndef MAS_UNIDATA_TYPES_H
#  define MAS_UNIDATA_TYPES_H

#include <unistd.h>


typedef enum mas_content_type_e
{
  MAS_CONTENT_BAD=-1,
  MAS_CONTENT_NONE=0,
  MAS_CONTENT_HTML,
  MAS_CONTENT_TEXT,
  MAS_CONTENT_GIF,
  MAS_CONTENT_JPEG,
  MAS_CONTENT_FORM_DATA,
} mas_content_type_t;


struct content_type_details_s
{
  mas_content_type_t ctype;
  char *mtype;
  char *dtype;
};
typedef struct content_type_details_s content_type_details_t;



typedef struct content_type_ext_s content_type_ext_t;
struct content_type_ext_s
{
  mas_content_type_t ctype;
  char *extset;
};

typedef struct mas_unidata_s mas_unidata_t;
struct mas_unidata_s
{
  mas_content_type_t icontent_type;
  size_t size;
  char *data;
};


#endif
