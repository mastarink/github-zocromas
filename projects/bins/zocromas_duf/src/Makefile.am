SUBDIRS= inc
bin_PROGRAMS=zocromas_duf
noinst_PROGRAMS=mtest

#noinst_HEADERS=....cdef ....cdef

zocromas_duf_SOURCES=				   \
	duf.c 			\
	duf_maindb_info.c duf_maindb.c		    \
	duf_dbg.c duf_config.c duf_status.c	     \
	duf_ufilter.c duf_ufilter_bind.c \
	duf_options.c duf_option.c duf_option_old.c  duf_option_tmpdb.c duf_option_typed.c  \
	duf_option_find.c duf_options_table.c		\
	duf_option_descr.c		\
	duf_options_cli.c duf_options_env.c duf_options_string.c duf_options_file.c duf_options_interactive.c \
	duf_option_help.c duf_option_misc.c		\
	duf_option_extended.c duf_option_sccb.c duf_option_names.c duf_option_cmd.c \
							 \
	duf_filedata.c					   \
	duf_path2db.c    \
	duf_path2dirid.c    \
	duf_tags.c					     \
	duf_selector.c \
	duf_item_scan2.c duf_leaf_scan2.c duf_sel_cb_leaf.c duf_sel_cb_node.c  duf_pstmt_levinfo.c		      \
	duf_eval_at_config.c duf_prepare_actions.c   \
	duf_begfin.c \
	duf_sccb.c duf_sccb_handle.c  duf_sccbh_eval.c duf_pdi_sccb_eval.c duf_sccb_eval.c duf_sccb_eval_std.c duf_sccb_begfin.c \
	duf_sccb_eval_dirs.c  \
	duf_dir_scan2_stages.c	\
	duf_dirent_scan2.c					 \
								  \
	duf_context.c	\
	duf_li_credel.c \
	duf_levinfo.c duf_levinfo_ref.c duf_levinfo_credel.c duf_levinfo_openclose.c duf_levinfo_stat.c \
	duf_levinfo_updown.c duf_levinfo_context.c	\
	duf_pathinfo_ref.c \
	duf_pdi.c duf_pdi_credel.c duf_pdi_ref.c duf_pdi_context.c duf_pdi_stmt.c			\
	duf_dirent.c 						    \
	duf_sys_wrap.c duf_utils.c duf_utils_path.c duf_utils_print.c duf_match.c   \
	duf_errors.c  duf_trace.c				      \
	duf_dh.c duf_service.c duf_print.c duf_printb.c	duf_fileinfo.c	     \
								      \
	duf_sqlite.c duf_sql.c duf_sql_field.c duf_sql_field2.c	       \
									\
	mod_dir_print_uni.c		mod_tree_print_uni.c		 \
	mod_integrity_uni.c						   \
	mod_filenames_uni.c		mod_filedata_uni.c		    \
	mod_directories_uni.c						     \
	mod_md5_uni.c	mod_sd5_uni.c					      \
	mod_crc32_uni.c							       \
	mod_collect_mime_uni.c		mod_exif_uni.c				\
	mod_template_uni.c \
	mod_dummy_uni.c	\
	mod_dumplet.c	\
	mod_tagit.c	\
	mod_save_to.c	\
	std_mod_sets.c \
	sql_beginning_selected.c \
	sql_beginning_common.c \
	sql_beginning_create.c \
	sql_beginning_vacuum.c \
	sql_beginning_tables.c \
	data_optable_flag.c	\
	data_optable_flag_obsolete.c	\
	data_optable_sflag.c	\
	data_optable_str.c	\
	data_optable_redo_and_obsolete.c	\
	data_optable_uplus.c	\
	data_optable_debug.c	\
	data_optable_filter.c	\
	data_optable_help.c	\
	data_optable_trace.c	\
	data_optable_format.c	\
	data_optable_sccb.c \
	data_optable_info.c \
	data_optable_interactive.c \
	data_optable_io.c \
	data_optable_experimental.c \
	data_optable.c

#	duf_item_match2.c
#	duf_file_pathid2.c
#	mod_print_md5_uni.c
#	duf_dirent_scan1.c
#	duf_dir_scan1.c
#	duf_file_pathid1.c
#	duf_item_scan1.c
#	duf_sql_field1.c
#	duf_uni_scan1.c

zocromas_duf_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @GLIB_CFLAGS@
zocromas_duf_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @CRYPTO_LIBS@ @CRYPTO_LIBS@ @SQLITE3_LDFLAGS@ -lexif -lmagic -lz

mtest_SOURCES=duftest.c
mtest_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @GLIB_CFLAGS@
mtest_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @CRYPTO_LIBS@ @CRYPTO_LIBS@ @SQLITE3_LDFLAGS@ -lexif -lmagic


# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@
#   template_LDFLAGS = -Wl,--export-dynamic
