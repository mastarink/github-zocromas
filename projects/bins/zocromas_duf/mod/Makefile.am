SUBDIRS= inc

# pkglib_LTLIBRARIES = foo_example_plugin.la
# foo_example_plugin_la_SOURCES = example.c
# foo_example_plugin_la_LDFLAGS = -avoid-version -module -shared -export-dynamic

# -module
#
#     Ignore the restriction about the lib- prefix for the plug-in file name, allowing free-form names. 
# -avoid-version
#
#     Allow the target to not provide any version information, removing the need to provide it. Almost all the plug-in systems don't use the library version to decide whether to load the objects, and rely instead on the path they find. 
# -shared
#
#     Disable entirely the build of the static version of the object, this reduces the number of installed files, as well as avoiding the double-build that would be needed for all the systems where static libraries and shared objects have different build requirements. 

AM_LDFLAGS = -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
AM_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@

moduledir=$(libdir)/dufmod
module_LTLIBRARIES = \
	ot_main.la ot_format.la ot_io.la ot_info.la ot_flag.la ot_flag_allow.la ot_str.la ot_sccb.la ot_trace.la ot_help.la ot_db.la ot_filter.la ot_test.la \
	null.la set_dir_priority.la tree.la \
	crc32.la dialog.la dirs.la dumplet.la exif.la filedata.la filenames.la listing.la packlist.la md5.la mime.la save_to.la sd5.la sha1.la tagit.la

# -shared 63 files (.la, .so); without: 94 files (.la, .so, .a)

# ot_format_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_format_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_format_la_SOURCES          = mod_data_optable_format.c

# ot_io_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_io_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_io_la_SOURCES          = mod_data_optable_io.c

# ot_main_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_main_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_main_la_SOURCES          = mod_data_optable.c

# ot_flag_allow_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_flag_allow_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_flag_allow_la_SOURCES          = mod_data_optable_flag_allow.c

# ot_trace_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_trace_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_trace_la_SOURCES          = mod_data_optable_trace.c

# ot_sccb_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_sccb_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_sccb_la_SOURCES          = mod_data_optable_sccb.c

# ot_help_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_help_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_help_la_SOURCES          = mod_data_optable_help.c

# ot_str_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_str_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_str_la_SOURCES          = mod_data_optable_str.c

# ot_db_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_db_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_db_la_SOURCES          = mod_data_optable_db.c

# ot_flag_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_flag_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_flag_la_SOURCES          = mod_data_optable_flag.c

# ot_test_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_test_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_test_la_SOURCES          = mod_data_optable_test.c

# ot_filter_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_filter_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_filter_la_SOURCES          = mod_data_optable_filter.c

# ot_info_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# ot_info_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
ot_info_la_SOURCES          = mod_data_optable_info.c

# null_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# null_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
null_la_SOURCES          = mod_null.c

# crc32_la_LDFLAGS          =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# crc32_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
crc32_la_SOURCES          = mod_crc32_uni.c

# set_dir_priority_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# set_dir_priority_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
set_dir_priority_la_SOURCES	= mod_dir_priority.c

# tree_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# tree_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
tree_la_SOURCES	= mod_tree_print_uni.c

# listing_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# listing_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
listing_la_SOURCES	= mod_dir_print_uni.c

# packlist_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# packlist_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
packlist_la_SOURCES	= mod_dir_print_pack.c

# filenames_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# filenames_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
filenames_la_SOURCES	= mod_filenames_uni.c

# filedata_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# filedata_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
filedata_la_SOURCES	= mod_filedata_uni.c

# dirs_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# dirs_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dirs_la_SOURCES	= mod_directories_uni.c

# md5_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# md5_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
md5_la_SOURCES	= mod_md5_uni.c

# sd5_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# sd5_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
sd5_la_SOURCES	= mod_sd5_uni.c

# sha1_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# sha1_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
sha1_la_SOURCES	= mod_sha_uni.c

# mime_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# mime_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
mime_la_SOURCES	= mod_mime_uni.c

# exif_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# exif_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@ @LIBEXIF_CFLAGS@
exif_la_CFLAGS = @AM_CFLAGS@ @LIBEXIF_CFLAGS@
exif_la_SOURCES	= mod_exif_uni.c

# dumplet_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# dumplet_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dumplet_la_SOURCES	= mod_dumplet.c

# dialog_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# dialog_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
dialog_la_SOURCES	= mod_dialog.c

# tagit_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# tagit_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
tagit_la_SOURCES	= mod_tagit.c

# save_to_la_LDFLAGS     =    -module -avoid-version -no-undefined -shared @AM_LDFLAGS@
# save_to_la_CFLAGS = @AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
save_to_la_SOURCES	= mod_save_to.c





# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@
#   template_LDFLAGS = -Wl,--export-dynamic
