#ifndef MAS_AUTOOBJECT_TYPES_H
#  define MAS_AUTOOBJECT_TYPES_H

/* FILE */
#include <stdio.h>


typedef enum mas_iaccess_type_e
{
  MAS_IACCESS_BAD = -1,
  MAS_IACCESS_CHAR = 0,
  MAS_IACCESS_FCHAR,
  MAS_IACCESS_SENDFILE,
  MAS_IACCESS_SPLICE,
  MAS_IACCESS_FILE,
} mas_iaccess_type_t;

typedef enum mas_icontent_type_e
{
  MAS_ICONTENT_BAD = -1,
  MAS_ICONTENT_NONE = 0,
  MAS_ICONTENT_HTML,
  MAS_ICONTENT_TEXT,
  MAS_ICONTENT_GIF,
  MAS_ICONTENT_JPEG,
  MAS_ICONTENT_FORM_DATA,
} mas_icontent_type_t;



typedef struct mas_autoobject_s
{
  mas_icontent_type_t icontent_type;
  mas_iaccess_type_t iaccess_type;
  unsigned long cat_cnt;
  unsigned long pass;
  unsigned long load_cnt;
  unsigned long reopen_cnt;
  unsigned long close_cnt;
  unsigned long stat_cnt;
  unsigned long lseek_cnt;
  unsigned long sendfile_cnt;
  unsigned long splice_cnt;
  char *name;
  unsigned long id;
  size_t size;
  union
  {
    FILE *f;
    void *v;
    unsigned long lu;
    int i;
  } handler;
  char *data;
} mas_autoobject_t;



#endif
