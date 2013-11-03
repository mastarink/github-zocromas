SUBDIRS= inc
bin_PROGRAMS=zocromas_duf


zocromas_duf_SOURCES=duf.c duf_config.c duf_cli_options.c duf_create.c duf_update.c duf_update_path.c \
	duf_update_realpath.c duf_update_pathid.c duf_update_pathentries.c \
	duf_insert.c duf_dirent.c duf_md5.c duf_exif.c duf_finddup.c duf_remove.c \
	duf_path.c duf_file.c duf_file_scan.c duf_dir_scan.c duf_file_pathid.c duf_file_md5id.c duf_utils.c \
	duf_sql.c

zocromas_duf_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @GLIB_CFLAGS@
zocromas_duf_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @CRYPTO_LIBS@ @SQLITE3_LDFLAGS@ -lexif

# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@ 
#   template_LDFLAGS = -Wl,--export-dynamic
