SUBDIRS= inc
bin_PROGRAMS=zocromas_duf


zocromas_duf_SOURCES=duf.c duf_maindb.c duf_action.c \
	duf_config.c duf_dbg.c duf_cli_options.c duf_option.c duf_create.c duf_add.c duf_update_path.c duf_group.c \
	duf_update_realpath.c \
	duf_dirent.c duf_filedata.c duf_filename.c duf_keydata.c duf_mdpath.c duf_exif.c duf_finddup.c duf_remove.c \
	duf_path.c duf_file.c duf_item_scan.c duf_dir_scan.c \
	duf_levinfo.c duf_pdi.c \
	duf_uni_scan.c \
	duf_file_pathid.c duf_file_md5id.c duf_utils.c duf_service.c \
	duf_dir_print_uni.c duf_tree_print_uni.c duf_fill_uni.c duf_sample_uni.c duf_sampupd_uni.c \
	duf_statistics_uni.c duf_mdpath_uni.c duf_fill_md5_uni.c duf_print_md5_uni.c \
	duf_sqlite.c duf_sql.c duf_sql_field.c

zocromas_duf_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @GLIB_CFLAGS@
zocromas_duf_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @CRYPTO_LIBS@ @CRYPTO_LIBS@ @SQLITE3_LDFLAGS@ -lexif

# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@ 
#   template_LDFLAGS = -Wl,--export-dynamic
