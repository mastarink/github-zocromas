/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the
   <arpa/inet.h,fcntl.h,limits.h,malloc.h,netdb.h,stdlib.h,string.h,sys/ioctl.h,sys/socket.h,sys/time.h,unistd.h>
   header file. */
/* #undef HAVE_ARPA_INET_H_FCNTL_H_LIMITS_H_MALLOC_H_NETDB_H_STDLIB_H_STRING_H_SYS_IOCTL_H_SYS_SOCKET_H_SYS_TIME_H_UNISTD_H */

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
   don't. */
#define HAVE_DECL_STRERROR_R 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_LIBPTHREAD 1

/* Define to 1 if you have the `readline' library (-lreadline). */
#define HAVE_LIBREADLINE 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset,dup2,atexit,' function. */
/* #undef HAVE_MEMSET_DUP2_ATEXIT_ */

/* Define to 1 if you have the
   `mkfifo,socket,strchr,strdup,strndup,strtol,strstr,inet_ntoa,gethostbyname,gettimeofday'
   function. */
#define HAVE_MKFIFO_SOCKET_STRCHR_STRDUP_STRNDUP_STRTOL_STRSTR_INET_NTOA_GETHOSTBYNAME_GETTIMEOFDAY 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror_r' function. */
#define HAVE_STRERROR_R 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* */
#define MAS_CALLOC calloc

/* compile flags */
#define MAS_CFLAGS "-pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe -DMAS_MODULESDIR=\"${pkglibdir}\" -DMAS_LIBDIR=\"${libdir}\" -ltcmalloc"

/* cdate */
#define MAS_C_DATE "20130205104544"

/* Enable additional debugging at the expense of performance and size */
/* #undef MAS_DEBUG */

/* */
#define MAS_FREE free

/* mas_libdir */
/* #undef MAS_LIBDIR */

/* */
#define MAS_MALLOC malloc

/* mas_modulesdir */
/* #undef MAS_MODULESDIR */

/* uname */
#define MAS_MODULES_DIR ""

/* os version */
#define MAS_OSVER "Linux version 3.6.11-gentoo.130126.m.g (root@mastar) (gcc version 4.5.4 (Gentoo 4.5.4 p1.0, pie-0.4.7) ) #1 SMP Sat Jan 26 16:56:11 EET 2013"

/* */
#define MAS_STRDUP strdup

/* sdate */
#define MAS_S_DATE "1360053944"

/* Enable additional tracememing at the expense of performance and size */
#define MAS_TRACEMEM 1

/* uname */
#define MAS_UNAME "Linux mastar 3.6.11-gentoo.130126.m.g #1 SMP Sat Jan 26 16:56:11 EET 2013 x86_64 Intel(R) Core(TM) i3-3240 CPU @ 3.40GHz GenuineIntel GNU/Linux"

/* uuidgen */
#define MAS_UUID "951db63b-3e89-4997-801e-63cc46cb84bb"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
#define PACKAGE "mas_wrap"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "bug-report@mastarink.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "mas_wrap"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "mas_wrap 1.0.1.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "mas_wrap"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.1.3"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if strerror_r returns char *. */
/* #undef STRERROR_R_CHAR_P */

/* Version number of package */
#define VERSION "1.0.1.3"

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */
