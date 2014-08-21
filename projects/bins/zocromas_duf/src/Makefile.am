SUBDIRS= inc
bin_PROGRAMS=zocromas_duf
noinst_PROGRAMS=mtest


zocromas_duf_SOURCES=				     \
	duf.c duf_maindb.c duf_action.c		      \
	duf_dbg.c duf_config.c			       \
	duf_options.c duf_cli_options.c duf_option.c	\
	duf_options_table.c				 \
	duf_create.c 					  \
							   \
	duf_add.c 					    \
	duf_filedata.c duf_path.c duf_file_pathid.c 	     \
							      \
	duf_item_scan1.c duf_item_scan2.c		       \
	duf_uni_scan.c duf_uni_scan1.c duf_uni_scan2.c		\
	duf_sccb.c duf_dir_scan1.c duf_dir_scan2.c		\
	duf_dirent_scan1.c duf_dirent_scan2.c			 \
								  \
	duf_levinfo.c duf_pdi.c					   \
								    \
	duf_utils.c duf_dh.c duf_dirent.c duf_service.c		     \
								      \
	duf_sqlite.c duf_sql.c duf_sql_field.c duf_sql_field1.c	       \
									\
	mod_dir_print_uni.c		mod_tree_print_uni.c		 \
	mod_sample_uni.c		mod_sampupd_uni.c		  \
	mod_statistics_uni.c		mod_mdpath_uni.c		   \
	mod_print_md5_uni.c		mod_integrity_uni.c		    \
	mod_filenames_uni.c		mod_filedata_uni.c		     \
	mod_directories_uni.c						      \
	mod_collect_openat_md5_uni.c	mod_sd5_uni.c			       \
	mod_crc32_uni.c								\
	mod_collect_mime_uni.c		mod_exif_uni.c				 \
	mod_template_uni.c

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
