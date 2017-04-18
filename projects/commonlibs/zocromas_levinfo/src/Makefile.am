SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_levinfo.la
libzocromas_levinfo_la_SOURCES = \
	masxfs_levinfo_base.c \
	masxfs_levinfo_io.c \
	masxfs_levinfo_fs_dir.c \
	masxfs_levinfo_io_dir.c \
	masxfs_levinfo_fs.c \
	masxfs_levinfo_db.c \
	masxfs_levinfo_path.c \
	masxfs_levinfo_ref.c \
	masxfs_levinfo.c \
	masxfs_levinfo_scan.c \
	masxfs_levinfo_format.c \
	masxfs_levinfo_digest.c \
	masxfs_levinfo_tools.c

libzocromas_levinfo_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@
libzocromas_levinfo_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@ @DEPSTEST_LIBS@ 
mastest_LDADD=lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@ @DEPSTEST_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c

else

endif
