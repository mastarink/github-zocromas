#ifndef MAS_DUF_ACTION_TABLE_H
#  define MAS_DUF_ACTION_TABLE_H

/* duf_action_table_t *_duf_action_table( void ) __attribute__ ( ( warn_unused_result ) ); */
duf_scan_callbacks_t *duf_first_sccb( void ) __attribute__ ( ( warn_unused_result ) );

extern duf_scan_callbacks_t     /* */
  /* duf_dirs_callbacks,    (* *) */
  /* duf_filedata_callbacks,       (* *) */
  /* duf_filenames_callbacks,      (* *) */
  /* duf_crc32_callbacks, (* *) */
  /* duf_sd5_callbacks, (* *) */
  /* duf_md5_callbacks, (* *) */
  /* duf_sha1_callbacks,           (* *) */
  /* duf_mime_callbacks,   (* *) */
  /* duf_exif_callbacks,   (* *) */
  /* duf_collect_mdpath_callbacks , */
  /* duf_print_md5_callbacks ,      */
  /* duf_tree_callbacks,     (* *) */
  /* duf_listing_callbacks,      (* *) */
  duf_dummy_callbacks,          /* */
  /* duf_dumplet_callbacks,        (* *) */
  /* duf_dialog_callbacks,         (* *) */
  /* duf_tagit_callbacks,          (* *) */
  /* duf_save_to_callbacks,        (* *) */
  duf_bpbpbpbp_$_pbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbp __attribute( ( weak ) );


#endif
