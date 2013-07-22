#ifndef MAS_AUTOOBJECT_TYPES_H
#  define MAS_AUTOOBJECT_TYPES_H

/* FILE */
#  include <stdio.h>

#  include <mastar/types/mas_evaluate_types.h>

typedef enum mas_iaccess_type_e
{
  MAS_IACCESS_BAD = -1,
  MAS_IACCESS_CHAR = 0,
  MAS_IACCESS_FCHAR,
  MAS_IACCESS_SENDFILE,
  MAS_IACCESS_SENDFILEL,
  MAS_IACCESS_SPLICE,
  MAS_IACCESS_FILE,
} mas_iaccess_type_t;


typedef struct mas_autoobject_s
{
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
  char *docroot;
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

  mas_content_type_t icontent_type;
  ino_t inode;
  mode_t mode;
  time_t time;
  char *gtime;
  char *etag;
} mas_autoobject_t;



#endif
