#ifndef MAS_FILEINFO_TYPES_H
#  define MAS_FILEINFO_TYPES_H

#  include <sys/types.h>

#  include <mastar/types/mas_unidata_types.h>


typedef struct mas_fileinfo_s mas_fileinfo_t;
struct mas_fileinfo_s
{
  mas_unidata_t *udata;
  time_t filetime;
  char *etag;
  /*  */
  char *root;
  char *tail;
  size_t filesize;
  ino_t inode;
  const void *userdata;
  data_loader_t data_loader;
  /* char *content_type; */
};



#endif
