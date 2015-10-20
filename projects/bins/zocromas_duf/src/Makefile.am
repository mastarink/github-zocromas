SUBDIRS= inc
bin_PROGRAMS=zocromas_duf
noinst_PROGRAMS=mtest mjtest

#noinst_HEADERS=....cdef ....cdef

zocromas_duf_SOURCES=				   \
	duf.c duf_experiment.c 			\
	duf_maindb_info.c duf_maindb.c		    \
	duf_dbg.c duf_config.c duf_config_util.c duf_status.c	     \
	duf_cli.c \
	duf_ufilter.c duf_ufilter_ref.c duf_ufilter_bind.c \
	duf_options_config.c duf_options.c duf_option.c  duf_option_tmpdb.c duf_option_typed.c  \
	duf_option_find.c duf_options_table.c		\
	duf_option_descr.c		\
	duf_options_cli.c duf_options_env.c duf_options_string.c duf_options_file.c duf_options_interactive.c \
	duf_option_help.c duf_option_misc.c		\
	duf_option_extended.c duf_option_sccb.c duf_option_fs.c duf_option_names.c duf_option_cmd.c \
							 \
	duf_filedata.c					   \
	duf_path2db.c    \
	duf_path2dirid.c    \
	duf_tags.c					     \
	duf_sccbh_eval_leaf.c duf_sel_cb_leaf.c duf_sel_cb_node.c  duf_pstmt_levinfo.c		      \
	duf_action_table.c \
	evsql_begfin.c evsql_selector.c \
	duf_sccb.c duf_sccb_handle.c  duf_sccbh_eval.c duf_sccbh_eval_fs.c duf_sccbh_eval_sql_set.c duf_sccb_scanstage.c \
	duf_pdi_sccb_eval.c \
	duf_sccb_eval.c duf_sccb_eval_std.c duf_sccb_begfin.c \
	duf_sccbh_eval_all.c  \
	duf_dir_scan2_stages.c	\
	\
	duf_atable_sccb.c duf_pdi_atable_sccb_eval.c duf_atable_sccb_eval.c \
	\
	duf_context.c	\
	duf_li_credel.c \
	duf_levinfo.c duf_levinfo_ref.c duf_levinfo_credel.c duf_levinfo_openclose.c duf_levinfo_stat.c \
	duf_levinfo_updown.c duf_levinfo_context.c	\
	duf_pathinfo_credel.c duf_pathinfo_ref.c \
	duf_pdi.c duf_pdi_reinit.c duf_pdi_attach.c duf_pdi_credel.c duf_pdi_ref.c duf_pdi_context.c duf_pdi_stmt.c			\
	duf_dirent.c 						    \
	duf_sys_wrap.c duf_utils.c duf_utils_path.c duf_match.c   \
	duf_error_clear.c duf_error_base.c duf_error_regfind.c duf_error_reporting.c duf_error_names.c       \
	duf_dh.c duf_service.c duf_print.c duf_printb.c	duf_fileinfo.c	     \
								      \
	duf_sql_error.c \
	duf_sql_open.c duf_sql_positional.c duf_sql_prepared.c duf_sql_field.c duf_sql_bind.c 	       \
									\
	mod_dummy_uni.c	\
	std_fieldsets.c std_selectors.c \
	std_mod_sets.c \
	sql_beginning_selected.c \
	sql_update_selected.c \
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
	data_optable_fs.c \
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

zocromas_duf_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@  @LIBEXIF_CFLAGS@
zocromas_duf_LDFLAGS=@AM_LDFLAGS@        @DEPS_LIBS@    @LIBCRYPTO_LIBS@    @SQLITE3_LIBS@    @LIBEXIF_LIBS@    @LIBMAGIC_LIBS@ @ZLIB_LIBS@ @READLINE_LIBS@ -ldl

mtest_SOURCES=duftest.c 
mtest_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
mtest_LDFLAGS=@AM_LDFLAGS@        @DEPS_LIBS@    @LIBCRYPTO_LIBS@    @SQLITE3_LIBS@   @LIBMAGIC_LIBS@

mjtest_SOURCES=dufjtest.c
mjtest_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @LIBEXIF_CFLAGS@
mjtest_LDFLAGS=@AM_LDFLAGS@                     @LIBEXIF_LIBS@




moduledir=$(libdir)/dufmod
module_LTLIBRARIES = \
	set_dir_priority.la tree.la \
	crc32.la dialog.la dirs.la dummy.la dumplet.la exif.la filedata.la filenames.la listing.la md5.la mime.la save_to.la sd5.la sha1.la tagit.la

crc32_la_LDFLAGS          =    -module -avoid-version -no-undefined @AM_LDFLAGS@
crc32_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
crc32_la_SOURCES          = mod_crc32_uni.c

set_dir_priority_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
set_dir_priority_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
set_dir_priority_la_SOURCES	= mod_dir_priority.c

tree_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
tree_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
tree_la_SOURCES	= mod_tree_print_uni.c

listing_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
listing_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
listing_la_SOURCES	= mod_dir_print_uni.c

filenames_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
filenames_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
filenames_la_SOURCES	= mod_filenames_uni.c

filedata_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
filedata_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
filedata_la_SOURCES	= mod_filedata_uni.c

dirs_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
dirs_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dirs_la_SOURCES	= mod_directories_uni.c

md5_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
md5_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
md5_la_SOURCES	= mod_md5_uni.c

sd5_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
sd5_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
sd5_la_SOURCES	= mod_sd5_uni.c

sha1_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
sha1_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
sha1_la_SOURCES	= mod_sha_uni.c

mime_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
mime_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
mime_la_SOURCES	= mod_collect_mime_uni.c

exif_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
exif_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@ @LIBEXIF_CFLAGS@
exif_la_SOURCES	= mod_exif_uni.c

dummy_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
dummy_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dummy_la_SOURCES	= mod_dummy_uni.c

dumplet_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
dumplet_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dumplet_la_SOURCES	= mod_dumplet.c

dialog_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
dialog_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dialog_la_SOURCES	= mod_dialog.c

tagit_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
tagit_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
tagit_la_SOURCES	= mod_tagit.c

save_to_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
save_to_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
save_to_la_SOURCES	= mod_save_to.c





# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@
#   template_LDFLAGS = -Wl,--export-dynamic
