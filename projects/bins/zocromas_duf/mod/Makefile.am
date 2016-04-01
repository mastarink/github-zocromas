SUBDIRS= inc


moduledir=$(libdir)/dufmod
module_LTLIBRARIES = \
	null.la set_dir_priority.la tree.la \
	crc32.la dialog.la dirs.la dumplet.la exif.la filedata.la filenames.la listing.la md5.la mime.la save_to.la sd5.la sha1.la tagit.la

null_la_LDFLAGS          =    -module -avoid-version -no-undefined @AM_LDFLAGS@
null_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
null_la_SOURCES          = mod_null.c

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
mime_la_SOURCES	= mod_mime_uni.c

exif_la_LDFLAGS     =    -module -avoid-version -no-undefined @AM_LDFLAGS@
exif_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@ @LIBEXIF_CFLAGS@
exif_la_SOURCES	= mod_exif_uni.c

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
