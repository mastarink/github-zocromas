#ifndef MAS_DUF_ERROR_TYPES_H
#  define MAS_DUF_ERROR_TYPES_H

#  define DUF_SQLITE2R_ERROR_CODE(r3c) duf_sqlite2r_error_code(r3c)

typedef enum
{
  DUF_OK,
  DUF_ERROR_ERROR_BASE = -30000,
  DUF_ERROR_UNKNOWN,
  DUF_ERROR_UNKNOWN_NODE,
  DUF_ERROR_MAIN,
  DUF_ERROR_NO_ACTIONS,
  DUF_ERROR_PTR,
  DUF_ERROR_DATA,
  DUF_ERROR_SCCB,
  DUF_ERROR_SD5,
  DUF_ERROR_MD5,
  DUF_ERROR_SHA1,
  DUF_ERROR_CRC32,
  DUF_ERROR_EXIF_END,
  DUF_ERROR_EXIF_NO_MODEL,
  DUF_ERROR_EXIF_NO_DATE,
  DUF_ERROR_EXIF_BROKEN_DATE,
  DUF_ERROR_EXIF,
  DUF_ERROR_NOT_OPEN,
  DUF_ERROR_EOF,
  DUF_ERROR_PATH,
  DUF_ERROR_OPENAT,
  DUF_ERROR_OPENAT_ENOENT,
  DUF_ERROR_OPEN,
  DUF_ERROR_OPEN_ENOENT,
  DUF_ERROR_FS_DISABLED,
  DUF_ERROR_READ,
  DUF_ERROR_CLOSE,
  DUF_ERROR_UNLINK,
  DUF_ERROR_OPTION,
  DUF_ERROR_OPTION_MULTIPLE,
  DUF_ERROR_OPTION_NOT_FOUND,
  DUF_ERROR_OPTION_NOT_PARSED,
  DUF_ERROR_OPTION_NO_FUNC,
  DUF_ERROR_OPTION_VALUE,
  DUF_ERROR_SUBOPTION,
  DUF_ERROR_SCANDIR,
  DUF_ERROR_CHECK_TABLES,
  DUF_ERROR_CLEAR_TABLES,
  DUF_ERROR_NO_FILE_SELECTOR,
  DUF_ERROR_DB_NO_PATH,
  DUF_ERROR_NO_STR_CB,
  DUF_ERROR_BIND_NAME,
  DUF_ERROR_LEVINFO_SIZE,
  DUF_ERROR_TOO_DEEP,
  DUF_ERROR_MAX_REACHED,
  DUF_ERROR_MAX_SEQ_REACHED,
  DUF_ERROR_GET_FIELD,
  DUF_ERROR_NOT_IN_DB,
  DUF_ERROR_TOTALS,
  DUF_ERROR_NO_FIELD,
  DUF_ERROR_NO_FIELD_OPTIONAL,
  DUF_ERROR_INSERT_MDPATH,
  DUF_ERROR_NOT_FOUND,
  DUF_ERROR_MKDIR,
  DUF_ERROR_STAT,
  DUF_ERROR_STAT_ENOENT,
  DUF_ERROR_STATAT,
  DUF_ERROR_STATAT_ENOENT,
  DUF_ERROR_PDI_SQL,
  DUF_ERROR_SQL_NO_FIELDSET,
  DUF_ERROR_SQL_NO_TABLE,
  DUF_ERROR_MEMORY,
  DUF_ERROR_ARGUMENT,
  DUF_ERROR_FILE_EXISTS,
  DUF_ERROR_SCCB_NOT_FOUND,
  DUF_ERROR_ERROR_MAX,
} duf_error_code_t;

#  define DUF_ERROR_COUNT ( DUF_ERROR_ERROR_MAX - DUF_ERROR_ERROR_BASE )

#endif

/*
vi: ft=c
*/
