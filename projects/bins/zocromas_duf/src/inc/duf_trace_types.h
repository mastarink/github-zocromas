#ifndef MAS_DUF_TRACE_TYPES_H
#  define MAS_DUF_TRACE_TYPES_H


typedef enum
{
  DUF_TRACE_MODE_NONE,
  DUF_TRACE_MODE_trace,
  DUF_TRACE_MODE_flags,
  DUF_TRACE_MODE_dry_run,
  DUF_TRACE_MODE_explain,

  DUF_TRACE_MODE_levinfo,
  DUF_TRACE_MODE_pdi,

  DUF_TRACE_MODE_action,
  DUF_TRACE_MODE_any,
  DUF_TRACE_MODE_sccb,
  DUF_TRACE_MODE_sccbh,
  DUF_TRACE_MODE_calls,
  DUF_TRACE_MODE_collect,
  DUF_TRACE_MODE_crc32,
  DUF_TRACE_MODE_temporary,
  DUF_TRACE_MODE_deleted,
  DUF_TRACE_MODE_depth,
  DUF_TRACE_MODE_dirent,
  DUF_TRACE_MODE_dirs,
  DUF_TRACE_MODE_error,
  DUF_TRACE_MODE_errorr,
  DUF_TRACE_MODE_exif,
  DUF_TRACE_MODE_filedata,
  DUF_TRACE_MODE_filenames,
  /* DUF_TRACE_MODE_fill, */
  DUF_TRACE_MODE_fs,
  DUF_TRACE_MODE_insert,
  DUF_TRACE_MODE_integrity,
  DUF_TRACE_MODE_match,
  DUF_TRACE_MODE_md5,
  DUF_TRACE_MODE_sha,
  DUF_TRACE_MODE_mdpath,
  DUF_TRACE_MODE_mime,
  DUF_TRACE_MODE_nonew,
  DUF_TRACE_MODE_options,
  DUF_TRACE_MODE_config,
  DUF_TRACE_MODE_path,
  DUF_TRACE_MODE_mod,
  DUF_TRACE_MODE_sample,
  DUF_TRACE_MODE_sampupd,
  DUF_TRACE_MODE_scan,
  DUF_TRACE_MODE_scan_dir,
  DUF_TRACE_MODE_scan_reg,
  DUF_TRACE_MODE_sd5,
  DUF_TRACE_MODE_select,
  DUF_TRACE_MODE_seq,
  DUF_TRACE_MODE_db,
  DUF_TRACE_MODE_sql,
  DUF_TRACE_MODE_sqlite,
  DUF_TRACE_MODE_statistics,
  DUF_TRACE_MODE_temp,
  DUF_TRACE_MODE_update,
  DUF_TRACE_MODE_io,
  DUF_TRACE_MODE_verbose,

  DUF_TRACE_MODE_MAX
} duf_trace_mode_t;

typedef enum
{
  DUF_TRACE_SUBMODE_SIMPLE,
  DUF_TRACE_SUBMODE_ARROW,
  DUF_TRACE_SUBMODE_MAX
} duf_trace_submode_t;







#endif

/*
vi: ft=c
*/
