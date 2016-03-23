#ifndef MAS_DUF_LEVINFO_STMT_TYPES_H
# define MAS_DUF_LEVINFO_STMT_TYPES_H

# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

typedef enum
{
  DUF_SQL_STMD_ID_select_filedata = 0x10000000,
  DUF_SQL_STMD_ID_select_childs,
  DUF_SQL_STMD_ID_insert_filedata,
  DUF_SQL_STMD_ID_insert_path_table,
  DUF_SQL_STMD_ID_select_path,
  DUF_SQL_STMD_ID_nameid2li_existed,
  DUF_SQL_STMD_ID_dirid2li_existed,
  DUF_SQL_STMD_ID_dirid2name_existed,
  DUF_SQL_STMD_ID_insert_tagname,
  DUF_SQL_STMD_ID_select_tagname,
  DUF_SQL_STMD_ID_insert_tag,
  DUF_SQL_STMD_ID_select_tag,
  DUF_SQL_STMD_ID_insert_filename,
  DUF_SQL_STMD_ID_select_md5,
  DUF_SQL_STMD_ID_insert_md5,
  DUF_SQL_STMD_ID_update_md5id,
  DUF_SQL_STMD_ID_select_sd5,
  DUF_SQL_STMD_ID_insert_sd5,
  DUF_SQL_STMD_ID_update_sd5id,
  DUF_SQL_STMD_ID_select_sha1,
  DUF_SQL_STMD_ID_insert_sha1,
  DUF_SQL_STMD_ID_update_sha1id,
  DUF_SQL_STMD_ID_select_crc32,
  DUF_SQL_STMD_ID_insert_crc32,
  DUF_SQL_STMD_ID_update_crc32id,
  DUF_SQL_STMD_ID_select_mime,
  DUF_SQL_STMD_ID_insert_mime,
  DUF_SQL_STMD_ID_update_mime,
  DUF_SQL_STMD_ID_select_model,
  DUF_SQL_STMD_ID_insert_model,
  DUF_SQL_STMD_ID_update_model,
  DUF_SQL_STMD_ID_select_exif,
  DUF_SQL_STMD_ID_insert_exif,
  DUF_SQL_STMD_ID_update_exif,
  DUF_SQL_STMD_ID_update_priority,
} duf_stmt_ident_t;

typedef struct
{
# if 0
  const int *id;                                                     /* just as id */
# else
  duf_stmt_ident_t id;
# endif
  duf_stmnt_t *pstmt;
} duf_idstmt_t;

#endif
