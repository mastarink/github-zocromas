#ifndef MAS_DUF_FILEINFO_TYPES_H
#  define MAS_DUF_FILEINFO_TYPES_H

typedef struct
{
  const char *name;
  struct stat st;
  unsigned long long dirid;
  /* unsigned long long truedirid; */
  unsigned long long nsame;
  unsigned long long nsame_md5;
  unsigned long long nsame_sha1;
  unsigned long long nsame_exif;
  unsigned long long sd5id;
  unsigned long long md5id;
  unsigned long long crc32id;
  unsigned long long nameid;
  const char *mime;
  unsigned long long mimeid;
  unsigned long long exifid;
  unsigned long long exifdt;
  const char *camera;
  unsigned long long dataid;
  unsigned long long sd5sum1;
  unsigned long long sd5sum2;
  unsigned long long sha1sum1;
  unsigned long long sha1sum2;
  unsigned long long sha1sum3;
  unsigned long long crc32sum;
  unsigned long long md5sum1;
  unsigned long long md5sum2;
} duf_fileinfo_t;

#endif

/*
vi: ft=c
*/
