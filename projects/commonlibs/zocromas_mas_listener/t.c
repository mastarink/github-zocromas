#1 "src/mas_listener.c"
#1 "<built-in>"
#1 "<command-line>"
#1 "src/mas_listener.c"
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/wrap/mas_std_def.h" 1
#2 "src/mas_listener.c" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_common_defs.h" 1
#3 "src/mas_listener.c" 2


#1 "/usr/include/unistd.h" 1 3 4
#26 "/usr/include/unistd.h" 3 4
#1 "/usr/include/features.h" 1 3 4
#358 "/usr/include/features.h" 3 4
#1 "/usr/include/sys/cdefs.h" 1 3 4
#378 "/usr/include/sys/cdefs.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#379 "/usr/include/sys/cdefs.h" 2 3 4
#359 "/usr/include/features.h" 2 3 4
#382 "/usr/include/features.h" 3 4
#1 "/usr/include/gnu/stubs.h" 1 3 4



#1 "/usr/include/bits/wordsize.h" 1 3 4
#5 "/usr/include/gnu/stubs.h" 2 3 4




#1 "/usr/include/gnu/stubs-64.h" 1 3 4
#10 "/usr/include/gnu/stubs.h" 2 3 4
#383 "/usr/include/features.h" 2 3 4
#27 "/usr/include/unistd.h" 2 3 4


#203 "/usr/include/unistd.h" 3 4
#1 "/usr/include/bits/posix_opt.h" 1 3 4
#204 "/usr/include/unistd.h" 2 3 4



#1 "/usr/include/bits/environments.h" 1 3 4
#23 "/usr/include/bits/environments.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#24 "/usr/include/bits/environments.h" 2 3 4
#208 "/usr/include/unistd.h" 2 3 4
#218 "/usr/include/unistd.h" 3 4
#1 "/usr/include/bits/types.h" 1 3 4
#28 "/usr/include/bits/types.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#29 "/usr/include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;







typedef long int __quad_t;
typedef unsigned long int __u_quad_t;

#131 "/usr/include/bits/types.h" 3 4
#1 "/usr/include/bits/typesizes.h" 1 3 4
#132 "/usr/include/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct
{
  int __val[2];
} __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;

typedef int __daddr_t;
typedef long int __swblk_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void *__timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;

typedef long int __ssize_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;

#219 "/usr/include/unistd.h" 2 3 4


typedef __ssize_t ssize_t;





#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#212 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 3 4
typedef long unsigned int size_t;

#228 "/usr/include/unistd.h" 2 3 4





typedef __gid_t gid_t;




typedef __uid_t uid_t;





typedef __off_t off_t;

#256 "/usr/include/unistd.h" 3 4
typedef __useconds_t useconds_t;




typedef __pid_t pid_t;






typedef __intptr_t intptr_t;






typedef __socklen_t socklen_t;

#288 "/usr/include/unistd.h" 3 4
extern int access( __const char *__name, int __type ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#305 "/usr/include/unistd.h" 3 4
extern int faccessat( int __fd, __const char *__file, int __type, int __flag )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );
#335 "/usr/include/unistd.h" 3 4
extern __off_t lseek( int __fd, __off_t __offset, int __whence ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#354 "/usr/include/unistd.h" 3 4
extern int close( int __fd );






extern ssize_t read( int __fd, void *__buf, size_t __nbytes );





extern ssize_t write( int __fd, __const void *__buf, size_t __n );

#377 "/usr/include/unistd.h" 3 4
extern ssize_t pread( int __fd, void *__buf, size_t __nbytes, __off_t __offset );






extern ssize_t pwrite( int __fd, __const void *__buf, size_t __n, __off_t __offset );

#418 "/usr/include/unistd.h" 3 4
extern int pipe( int __pipedes[2] ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#433 "/usr/include/unistd.h" 3 4
extern unsigned int alarm( unsigned int __seconds ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#445 "/usr/include/unistd.h" 3 4
extern unsigned int sleep( unsigned int __seconds );







extern __useconds_t ualarm( __useconds_t __value, __useconds_t __interval ) __attribute__ ( ( __nothrow__, __leaf__ ) );






extern int usleep( __useconds_t __useconds );

#470 "/usr/include/unistd.h" 3 4
extern int pause( void );



extern int chown( __const char *__file, __uid_t __owner, __gid_t __group )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



extern int fchown( int __fd, __uid_t __owner, __gid_t __group ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int lchown( __const char *__file, __uid_t __owner, __gid_t __group )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






extern int fchownat( int __fd, __const char *__file, __uid_t __owner,
                     __gid_t __group, int __flag ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );



extern int chdir( __const char *__path ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



extern int fchdir( int __fd ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#512 "/usr/include/unistd.h" 3 4
extern char *getcwd( char *__buf, size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#526 "/usr/include/unistd.h" 3 4
extern char *getwd( char *__buf )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) ) __attribute__ ( ( __deprecated__ ) );




extern int dup( int __fd ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern int dup2( int __fd, int __fd2 ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#544 "/usr/include/unistd.h" 3 4
extern char **__environ;







extern int execve( __const char *__path, char *__const __argv[],
                   char *__const __envp[] ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




extern int fexecve( int __fd, char *__const __argv[], char *__const __envp[] )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );




extern int execv( __const char *__path, char *__const __argv[] )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



extern int execle( __const char *__path, __const char *__arg, ... )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



extern int execl( __const char *__path, __const char *__arg, ... )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



extern int execvp( __const char *__file, char *__const __argv[] )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




extern int execlp( __const char *__file, __const char *__arg, ... )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#599 "/usr/include/unistd.h" 3 4
extern int nice( int __inc ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern void _exit( int __status ) __attribute__ ( ( __noreturn__ ) );





#1 "/usr/include/bits/confname.h" 1 3 4
#26 "/usr/include/bits/confname.h" 3 4
enum
{
  _PC_LINK_MAX,

  _PC_MAX_CANON,

  _PC_MAX_INPUT,

  _PC_NAME_MAX,

  _PC_PATH_MAX,

  _PC_PIPE_BUF,

  _PC_CHOWN_RESTRICTED,

  _PC_NO_TRUNC,

  _PC_VDISABLE,

  _PC_SYNC_IO,

  _PC_ASYNC_IO,

  _PC_PRIO_IO,

  _PC_SOCK_MAXBUF,

  _PC_FILESIZEBITS,

  _PC_REC_INCR_XFER_SIZE,

  _PC_REC_MAX_XFER_SIZE,

  _PC_REC_MIN_XFER_SIZE,

  _PC_REC_XFER_ALIGN,

  _PC_ALLOC_SIZE_MIN,

  _PC_SYMLINK_MAX,

  _PC_2_SYMLINKS
};


enum
{
  _SC_ARG_MAX,

  _SC_CHILD_MAX,

  _SC_CLK_TCK,

  _SC_NGROUPS_MAX,

  _SC_OPEN_MAX,

  _SC_STREAM_MAX,

  _SC_TZNAME_MAX,

  _SC_JOB_CONTROL,

  _SC_SAVED_IDS,

  _SC_REALTIME_SIGNALS,

  _SC_PRIORITY_SCHEDULING,

  _SC_TIMERS,

  _SC_ASYNCHRONOUS_IO,

  _SC_PRIORITIZED_IO,

  _SC_SYNCHRONIZED_IO,

  _SC_FSYNC,

  _SC_MAPPED_FILES,

  _SC_MEMLOCK,

  _SC_MEMLOCK_RANGE,

  _SC_MEMORY_PROTECTION,

  _SC_MESSAGE_PASSING,

  _SC_SEMAPHORES,

  _SC_SHARED_MEMORY_OBJECTS,

  _SC_AIO_LISTIO_MAX,

  _SC_AIO_MAX,

  _SC_AIO_PRIO_DELTA_MAX,

  _SC_DELAYTIMER_MAX,

  _SC_MQ_OPEN_MAX,

  _SC_MQ_PRIO_MAX,

  _SC_VERSION,

  _SC_PAGESIZE,


  _SC_RTSIG_MAX,

  _SC_SEM_NSEMS_MAX,

  _SC_SEM_VALUE_MAX,

  _SC_SIGQUEUE_MAX,

  _SC_TIMER_MAX,




  _SC_BC_BASE_MAX,

  _SC_BC_DIM_MAX,

  _SC_BC_SCALE_MAX,

  _SC_BC_STRING_MAX,

  _SC_COLL_WEIGHTS_MAX,

  _SC_EQUIV_CLASS_MAX,

  _SC_EXPR_NEST_MAX,

  _SC_LINE_MAX,

  _SC_RE_DUP_MAX,

  _SC_CHARCLASS_NAME_MAX,


  _SC_2_VERSION,

  _SC_2_C_BIND,

  _SC_2_C_DEV,

  _SC_2_FORT_DEV,

  _SC_2_FORT_RUN,

  _SC_2_SW_DEV,

  _SC_2_LOCALEDEF,


  _SC_PII,

  _SC_PII_XTI,

  _SC_PII_SOCKET,

  _SC_PII_INTERNET,

  _SC_PII_OSI,

  _SC_POLL,

  _SC_SELECT,

  _SC_UIO_MAXIOV,

  _SC_IOV_MAX = _SC_UIO_MAXIOV,

  _SC_PII_INTERNET_STREAM,

  _SC_PII_INTERNET_DGRAM,

  _SC_PII_OSI_COTS,

  _SC_PII_OSI_CLTS,

  _SC_PII_OSI_M,

  _SC_T_IOV_MAX,



  _SC_THREADS,

  _SC_THREAD_SAFE_FUNCTIONS,

  _SC_GETGR_R_SIZE_MAX,

  _SC_GETPW_R_SIZE_MAX,

  _SC_LOGIN_NAME_MAX,

  _SC_TTY_NAME_MAX,

  _SC_THREAD_DESTRUCTOR_ITERATIONS,

  _SC_THREAD_KEYS_MAX,

  _SC_THREAD_STACK_MIN,

  _SC_THREAD_THREADS_MAX,

  _SC_THREAD_ATTR_STACKADDR,

  _SC_THREAD_ATTR_STACKSIZE,

  _SC_THREAD_PRIORITY_SCHEDULING,

  _SC_THREAD_PRIO_INHERIT,

  _SC_THREAD_PRIO_PROTECT,

  _SC_THREAD_PROCESS_SHARED,


  _SC_NPROCESSORS_CONF,

  _SC_NPROCESSORS_ONLN,

  _SC_PHYS_PAGES,

  _SC_AVPHYS_PAGES,

  _SC_ATEXIT_MAX,

  _SC_PASS_MAX,


  _SC_XOPEN_VERSION,

  _SC_XOPEN_XCU_VERSION,

  _SC_XOPEN_UNIX,

  _SC_XOPEN_CRYPT,

  _SC_XOPEN_ENH_I18N,

  _SC_XOPEN_SHM,


  _SC_2_CHAR_TERM,

  _SC_2_C_VERSION,

  _SC_2_UPE,


  _SC_XOPEN_XPG2,

  _SC_XOPEN_XPG3,

  _SC_XOPEN_XPG4,


  _SC_CHAR_BIT,

  _SC_CHAR_MAX,

  _SC_CHAR_MIN,

  _SC_INT_MAX,

  _SC_INT_MIN,

  _SC_LONG_BIT,

  _SC_WORD_BIT,

  _SC_MB_LEN_MAX,

  _SC_NZERO,

  _SC_SSIZE_MAX,

  _SC_SCHAR_MAX,

  _SC_SCHAR_MIN,

  _SC_SHRT_MAX,

  _SC_SHRT_MIN,

  _SC_UCHAR_MAX,

  _SC_UINT_MAX,

  _SC_ULONG_MAX,

  _SC_USHRT_MAX,


  _SC_NL_ARGMAX,

  _SC_NL_LANGMAX,

  _SC_NL_MSGMAX,

  _SC_NL_NMAX,

  _SC_NL_SETMAX,

  _SC_NL_TEXTMAX,


  _SC_XBS5_ILP32_OFF32,

  _SC_XBS5_ILP32_OFFBIG,

  _SC_XBS5_LP64_OFF64,

  _SC_XBS5_LPBIG_OFFBIG,


  _SC_XOPEN_LEGACY,

  _SC_XOPEN_REALTIME,

  _SC_XOPEN_REALTIME_THREADS,


  _SC_ADVISORY_INFO,

  _SC_BARRIERS,

  _SC_BASE,

  _SC_C_LANG_SUPPORT,

  _SC_C_LANG_SUPPORT_R,

  _SC_CLOCK_SELECTION,

  _SC_CPUTIME,

  _SC_THREAD_CPUTIME,

  _SC_DEVICE_IO,

  _SC_DEVICE_SPECIFIC,

  _SC_DEVICE_SPECIFIC_R,

  _SC_FD_MGMT,

  _SC_FIFO,

  _SC_PIPE,

  _SC_FILE_ATTRIBUTES,

  _SC_FILE_LOCKING,

  _SC_FILE_SYSTEM,

  _SC_MONOTONIC_CLOCK,

  _SC_MULTI_PROCESS,

  _SC_SINGLE_PROCESS,

  _SC_NETWORKING,

  _SC_READER_WRITER_LOCKS,

  _SC_SPIN_LOCKS,

  _SC_REGEXP,

  _SC_REGEX_VERSION,

  _SC_SHELL,

  _SC_SIGNALS,

  _SC_SPAWN,

  _SC_SPORADIC_SERVER,

  _SC_THREAD_SPORADIC_SERVER,

  _SC_SYSTEM_DATABASE,

  _SC_SYSTEM_DATABASE_R,

  _SC_TIMEOUTS,

  _SC_TYPED_MEMORY_OBJECTS,

  _SC_USER_GROUPS,

  _SC_USER_GROUPS_R,

  _SC_2_PBS,

  _SC_2_PBS_ACCOUNTING,

  _SC_2_PBS_LOCATE,

  _SC_2_PBS_MESSAGE,

  _SC_2_PBS_TRACK,

  _SC_SYMLOOP_MAX,

  _SC_STREAMS,

  _SC_2_PBS_CHECKPOINT,


  _SC_V6_ILP32_OFF32,

  _SC_V6_ILP32_OFFBIG,

  _SC_V6_LP64_OFF64,

  _SC_V6_LPBIG_OFFBIG,


  _SC_HOST_NAME_MAX,

  _SC_TRACE,

  _SC_TRACE_EVENT_FILTER,

  _SC_TRACE_INHERIT,

  _SC_TRACE_LOG,


  _SC_LEVEL1_ICACHE_SIZE,

  _SC_LEVEL1_ICACHE_ASSOC,

  _SC_LEVEL1_ICACHE_LINESIZE,

  _SC_LEVEL1_DCACHE_SIZE,

  _SC_LEVEL1_DCACHE_ASSOC,

  _SC_LEVEL1_DCACHE_LINESIZE,

  _SC_LEVEL2_CACHE_SIZE,

  _SC_LEVEL2_CACHE_ASSOC,

  _SC_LEVEL2_CACHE_LINESIZE,

  _SC_LEVEL3_CACHE_SIZE,

  _SC_LEVEL3_CACHE_ASSOC,

  _SC_LEVEL3_CACHE_LINESIZE,

  _SC_LEVEL4_CACHE_SIZE,

  _SC_LEVEL4_CACHE_ASSOC,

  _SC_LEVEL4_CACHE_LINESIZE,



  _SC_IPV6 = _SC_LEVEL1_ICACHE_SIZE + 50,

  _SC_RAW_SOCKETS,


  _SC_V7_ILP32_OFF32,

  _SC_V7_ILP32_OFFBIG,

  _SC_V7_LP64_OFF64,

  _SC_V7_LPBIG_OFFBIG,


  _SC_SS_REPL_MAX,


  _SC_TRACE_EVENT_NAME_MAX,

  _SC_TRACE_NAME_MAX,

  _SC_TRACE_SYS_MAX,

  _SC_TRACE_USER_EVENT_MAX,


  _SC_XOPEN_STREAMS,


  _SC_THREAD_ROBUST_PRIO_INHERIT,

  _SC_THREAD_ROBUST_PRIO_PROTECT
};


enum
{
  _CS_PATH,


  _CS_V6_WIDTH_RESTRICTED_ENVS,



  _CS_GNU_LIBC_VERSION,

  _CS_GNU_LIBPTHREAD_VERSION,


  _CS_V5_WIDTH_RESTRICTED_ENVS,



  _CS_V7_WIDTH_RESTRICTED_ENVS,



  _CS_LFS_CFLAGS = 1000,

  _CS_LFS_LDFLAGS,

  _CS_LFS_LIBS,

  _CS_LFS_LINTFLAGS,

  _CS_LFS64_CFLAGS,

  _CS_LFS64_LDFLAGS,

  _CS_LFS64_LIBS,

  _CS_LFS64_LINTFLAGS,


  _CS_XBS5_ILP32_OFF32_CFLAGS = 1100,

  _CS_XBS5_ILP32_OFF32_LDFLAGS,

  _CS_XBS5_ILP32_OFF32_LIBS,

  _CS_XBS5_ILP32_OFF32_LINTFLAGS,

  _CS_XBS5_ILP32_OFFBIG_CFLAGS,

  _CS_XBS5_ILP32_OFFBIG_LDFLAGS,

  _CS_XBS5_ILP32_OFFBIG_LIBS,

  _CS_XBS5_ILP32_OFFBIG_LINTFLAGS,

  _CS_XBS5_LP64_OFF64_CFLAGS,

  _CS_XBS5_LP64_OFF64_LDFLAGS,

  _CS_XBS5_LP64_OFF64_LIBS,

  _CS_XBS5_LP64_OFF64_LINTFLAGS,

  _CS_XBS5_LPBIG_OFFBIG_CFLAGS,

  _CS_XBS5_LPBIG_OFFBIG_LDFLAGS,

  _CS_XBS5_LPBIG_OFFBIG_LIBS,

  _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS,


  _CS_POSIX_V6_ILP32_OFF32_CFLAGS,

  _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,

  _CS_POSIX_V6_ILP32_OFF32_LIBS,

  _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS,

  _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,

  _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,

  _CS_POSIX_V6_ILP32_OFFBIG_LIBS,

  _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS,

  _CS_POSIX_V6_LP64_OFF64_CFLAGS,

  _CS_POSIX_V6_LP64_OFF64_LDFLAGS,

  _CS_POSIX_V6_LP64_OFF64_LIBS,

  _CS_POSIX_V6_LP64_OFF64_LINTFLAGS,

  _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,

  _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,

  _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,

  _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS,


  _CS_POSIX_V7_ILP32_OFF32_CFLAGS,

  _CS_POSIX_V7_ILP32_OFF32_LDFLAGS,

  _CS_POSIX_V7_ILP32_OFF32_LIBS,

  _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS,

  _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS,

  _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS,

  _CS_POSIX_V7_ILP32_OFFBIG_LIBS,

  _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS,

  _CS_POSIX_V7_LP64_OFF64_CFLAGS,

  _CS_POSIX_V7_LP64_OFF64_LDFLAGS,

  _CS_POSIX_V7_LP64_OFF64_LIBS,

  _CS_POSIX_V7_LP64_OFF64_LINTFLAGS,

  _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS,

  _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS,

  _CS_POSIX_V7_LPBIG_OFFBIG_LIBS,

  _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS,


  _CS_V6_ENV,

  _CS_V7_ENV
};

#611 "/usr/include/unistd.h" 2 3 4


extern long int pathconf( __const char *__path, int __name )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


extern long int fpathconf( int __fd, int __name ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern long int sysconf( int __name ) __attribute__ ( ( __nothrow__, __leaf__ ) );



extern size_t confstr( int __name, char *__buf, size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern __pid_t getpid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern __pid_t getppid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern __pid_t getpgrp( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#647 "/usr/include/unistd.h" 3 4
extern __pid_t __getpgid( __pid_t __pid ) __attribute__ ( ( __nothrow__, __leaf__ ) );

extern __pid_t getpgid( __pid_t __pid ) __attribute__ ( ( __nothrow__, __leaf__ ) );






extern int setpgid( __pid_t __pid, __pid_t __pgid ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#673 "/usr/include/unistd.h" 3 4
extern int setpgrp( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#690 "/usr/include/unistd.h" 3 4
extern __pid_t setsid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );



extern __pid_t getsid( __pid_t __pid ) __attribute__ ( ( __nothrow__, __leaf__ ) );



extern __uid_t getuid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern __uid_t geteuid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern __gid_t getgid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern __gid_t getegid( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int getgroups( int __size, __gid_t __list[] ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#723 "/usr/include/unistd.h" 3 4
extern int setuid( __uid_t __uid ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int setreuid( __uid_t __ruid, __uid_t __euid ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int seteuid( __uid_t __uid ) __attribute__ ( ( __nothrow__, __leaf__ ) );






extern int setgid( __gid_t __gid ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int setregid( __gid_t __rgid, __gid_t __egid ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int setegid( __gid_t __gid ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#779 "/usr/include/unistd.h" 3 4
extern __pid_t fork( void ) __attribute__ ( ( __nothrow__ ) );







extern __pid_t vfork( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern char *ttyname( int __fd ) __attribute__ ( ( __nothrow__, __leaf__ ) );



extern int ttyname_r( int __fd, char *__buf, size_t __buflen )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );



extern int isatty( int __fd ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern int ttyslot( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int link( __const char *__from, __const char *__to )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




extern int linkat( int __fromfd, __const char *__from, int __tofd,
                   __const char *__to, int __flags ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 4 ) ) );




extern int symlink( __const char *__from, __const char *__to )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




extern ssize_t readlink( __const char *__restrict __path,
                         char *__restrict __buf, size_t __len )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




extern int symlinkat( __const char *__from, int __tofd,
                      __const char *__to ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 3 ) ) );


extern ssize_t readlinkat( int __fd, __const char *__restrict __path,
                           char *__restrict __buf, size_t __len )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 3 ) ) );



extern int unlink( __const char *__name ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



extern int unlinkat( int __fd, __const char *__name, int __flag )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );



extern int rmdir( __const char *__path ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



extern __pid_t tcgetpgrp( int __fd ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern int tcsetpgrp( int __fd, __pid_t __pgrp_id ) __attribute__ ( ( __nothrow__, __leaf__ ) );






extern char *getlogin( void );







extern int getlogin_r( char *__name, size_t __name_len ) __attribute__ ( ( __nonnull__( 1 ) ) );




extern int setlogin( __const char *__name ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#894 "/usr/include/unistd.h" 3 4
#1 "/usr/include/getopt.h" 1 3 4
#59 "/usr/include/getopt.h" 3 4
extern char *optarg;

#73 "/usr/include/getopt.h" 3 4
extern int optind;




extern int opterr;



extern int optopt;

#152 "/usr/include/getopt.h" 3 4
extern int getopt( int ___argc, char *const *___argv, const char *__shortopts ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#895 "/usr/include/unistd.h" 2 3 4







extern int gethostname( char *__name, size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






extern int sethostname( __const char *__name, size_t __len )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



extern int sethostid( long int __id ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern int getdomainname( char *__name, size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
extern int setdomainname( __const char *__name, size_t __len )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern int vhangup( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern int revoke( __const char *__file ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );







extern int profil( unsigned short int *__sample_buffer, size_t __size,
                   size_t __offset, unsigned int __scale )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern int acct( __const char *__name ) __attribute__ ( ( __nothrow__, __leaf__ ) );



extern char *getusershell( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );
extern void endusershell( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );
extern void setusershell( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern int daemon( int __nochdir, int __noclose ) __attribute__ ( ( __nothrow__, __leaf__ ) );






extern int chroot( __const char *__path ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



extern char *getpass( __const char *__prompt ) __attribute__ ( ( __nonnull__( 1 ) ) );

#980 "/usr/include/unistd.h" 3 4
extern int fsync( int __fd );

#994 "/usr/include/unistd.h" 3 4
extern long int gethostid( void );


extern void sync( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern int getpagesize( void ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );




extern int getdtablesize( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#1018 "/usr/include/unistd.h" 3 4
extern int truncate( __const char *__file, __off_t __length )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#1040 "/usr/include/unistd.h" 3 4
extern int ftruncate( int __fd, __off_t __length ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#1061 "/usr/include/unistd.h" 3 4
extern int brk( void *__addr ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern void *sbrk( intptr_t __delta ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#1082 "/usr/include/unistd.h" 3 4
extern long int syscall( long int __sysno, ... ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#1105 "/usr/include/unistd.h" 3 4
extern int lockf( int __fd, int __cmd, __off_t __len );

#1136 "/usr/include/unistd.h" 3 4
extern int fdatasync( int __fildes );

#1165 "/usr/include/unistd.h" 3 4
extern char *ctermid( char *__s ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#1174 "/usr/include/unistd.h" 3 4

#6 "src/mas_listener.c" 2
#1 "/usr/include/stdlib.h" 1 3 4
#33 "/usr/include/stdlib.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#324 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 3 4
typedef int wchar_t;

#34 "/usr/include/stdlib.h" 2 3 4








#1 "/usr/include/bits/waitflags.h" 1 3 4
#43 "/usr/include/stdlib.h" 2 3 4
#1 "/usr/include/bits/waitstatus.h" 1 3 4
#65 "/usr/include/bits/waitstatus.h" 3 4
#1 "/usr/include/endian.h" 1 3 4
#37 "/usr/include/endian.h" 3 4
#1 "/usr/include/bits/endian.h" 1 3 4
#38 "/usr/include/endian.h" 2 3 4
#61 "/usr/include/endian.h" 3 4
#1 "/usr/include/bits/byteswap.h" 1 3 4
#28 "/usr/include/bits/byteswap.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#29 "/usr/include/bits/byteswap.h" 2 3 4
#62 "/usr/include/endian.h" 2 3 4
#66 "/usr/include/bits/waitstatus.h" 2 3 4

union wait
{
  int w_status;
  struct
  {

    unsigned int __w_termsig:7;
    unsigned int __w_coredump:1;
    unsigned int __w_retcode:8;
    unsigned int:16;







  } __wait_terminated;
  struct
  {

    unsigned int __w_stopval:8;
    unsigned int __w_stopsig:8;
    unsigned int:16;






  } __wait_stopped;
};

#44 "/usr/include/stdlib.h" 2 3 4
#68 "/usr/include/stdlib.h" 3 4
typedef union
{
  union wait *__uptr;
  int *__iptr;
} __WAIT_STATUS __attribute__ ( ( __transparent_union__ ) );

#96 "/usr/include/stdlib.h" 3 4


typedef struct
{
  int quot;
  int rem;
} div_t;



typedef struct
{
  long int quot;
  long int rem;
} ldiv_t;







__extension__ typedef struct
{
  long long int quot;
  long long int rem;
} lldiv_t;


#140 "/usr/include/stdlib.h" 3 4
extern size_t __ctype_get_mb_cur_max( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern double atof( __const char *__nptr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

extern int atoi( __const char *__nptr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

extern long int atol( __const char *__nptr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





__extension__ extern long long int atoll( __const char *__nptr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern double strtod( __const char *__restrict __nptr,
                      char **__restrict __endptr ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern float strtof( __const char *__restrict __nptr,
                     char **__restrict __endptr ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

extern long double strtold( __const char *__restrict __nptr,
                            char **__restrict __endptr ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern long int strtol( __const char *__restrict __nptr,
                        char **__restrict __endptr, int __base )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

extern unsigned long int strtoul( __const char *__restrict __nptr,
                                  char **__restrict __endptr, int __base )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );




__extension__
      extern long long int strtoq( __const char *__restrict __nptr,
                                   char **__restrict __endptr, int __base )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

__extension__
      extern unsigned long long int strtouq( __const char *__restrict __nptr,
                                             char **__restrict __endptr, int __base )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





__extension__
      extern long long int strtoll( __const char *__restrict __nptr,
                                    char **__restrict __endptr, int __base )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

__extension__
      extern unsigned long long int strtoull( __const char *__restrict __nptr,
                                              char **__restrict __endptr, int __base )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#311 "/usr/include/stdlib.h" 3 4
extern char *l64a( long int __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern long int a64l( __const char *__s )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );




#1 "/usr/include/sys/types.h" 1 3 4
#28 "/usr/include/sys/types.h" 3 4






typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;

#61 "/usr/include/sys/types.h" 3 4
typedef __dev_t dev_t;

#71 "/usr/include/sys/types.h" 3 4
typedef __mode_t mode_t;




typedef __nlink_t nlink_t;

#105 "/usr/include/sys/types.h" 3 4
typedef __id_t id_t;

#116 "/usr/include/sys/types.h" 3 4
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;

#133 "/usr/include/sys/types.h" 3 4
#1 "/usr/include/time.h" 1 3 4
#58 "/usr/include/time.h" 3 4


typedef __clock_t clock_t;



#74 "/usr/include/time.h" 3 4


typedef __time_t time_t;



#92 "/usr/include/time.h" 3 4
typedef __clockid_t clockid_t;

#104 "/usr/include/time.h" 3 4
typedef __timer_t timer_t;

#134 "/usr/include/sys/types.h" 2 3 4
#147 "/usr/include/sys/types.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#148 "/usr/include/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;

#195 "/usr/include/sys/types.h" 3 4
typedef int int8_t __attribute__ ( ( __mode__( __QI__ ) ) );
typedef int int16_t __attribute__ ( ( __mode__( __HI__ ) ) );
typedef int int32_t __attribute__ ( ( __mode__( __SI__ ) ) );
typedef int int64_t __attribute__ ( ( __mode__( __DI__ ) ) );


typedef unsigned int u_int8_t __attribute__ ( ( __mode__( __QI__ ) ) );
typedef unsigned int u_int16_t __attribute__ ( ( __mode__( __HI__ ) ) );
typedef unsigned int u_int32_t __attribute__ ( ( __mode__( __SI__ ) ) );
typedef unsigned int u_int64_t __attribute__ ( ( __mode__( __DI__ ) ) );

typedef int register_t __attribute__ ( ( __mode__( __word__ ) ) );

#220 "/usr/include/sys/types.h" 3 4
#1 "/usr/include/sys/select.h" 1 3 4
#31 "/usr/include/sys/select.h" 3 4
#1 "/usr/include/bits/select.h" 1 3 4
#23 "/usr/include/bits/select.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#24 "/usr/include/bits/select.h" 2 3 4
#32 "/usr/include/sys/select.h" 2 3 4


#1 "/usr/include/bits/sigset.h" 1 3 4
#24 "/usr/include/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
{
  unsigned long int __val[( 1024 / ( 8 * sizeof( unsigned long int ) ) )];
} __sigset_t;

#35 "/usr/include/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;





#1 "/usr/include/time.h" 1 3 4
#120 "/usr/include/time.h" 3 4
struct timespec
{
  __time_t tv_sec;
  long int tv_nsec;
};

#45 "/usr/include/sys/select.h" 2 3 4

#1 "/usr/include/bits/time.h" 1 3 4
#31 "/usr/include/bits/time.h" 3 4
struct timeval
{
  __time_t tv_sec;
  __suseconds_t tv_usec;
};

#47 "/usr/include/sys/select.h" 2 3 4


typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;

#65 "/usr/include/sys/select.h" 3 4
typedef struct
{






  __fd_mask __fds_bits[1024 / ( 8 * ( int ) sizeof( __fd_mask ) )];


}
fd_set;






typedef __fd_mask fd_mask;

#97 "/usr/include/sys/select.h" 3 4

#107 "/usr/include/sys/select.h" 3 4
extern int select( int __nfds, fd_set * __restrict __readfds,
                   fd_set * __restrict __writefds, fd_set * __restrict __exceptfds, struct timeval *__restrict __timeout );
#119 "/usr/include/sys/select.h" 3 4
extern int pselect( int __nfds, fd_set * __restrict __readfds,
                    fd_set * __restrict __writefds,
                    fd_set * __restrict __exceptfds, const struct timespec *__restrict __timeout, const __sigset_t * __restrict __sigmask );
#132 "/usr/include/sys/select.h" 3 4

#221 "/usr/include/sys/types.h" 2 3 4


#1 "/usr/include/sys/sysmacros.h" 1 3 4
#30 "/usr/include/sys/sysmacros.h" 3 4


__extension__
      extern unsigned int gnu_dev_major( unsigned long long int __dev )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
__extension__
      extern unsigned int gnu_dev_minor( unsigned long long int __dev )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
__extension__
      extern unsigned long long int gnu_dev_makedev( unsigned int __major,
                                                     unsigned int __minor )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
#64 "/usr/include/sys/sysmacros.h" 3 4

#224 "/usr/include/sys/types.h" 2 3 4





typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;

#271 "/usr/include/sys/types.h" 3 4
#1 "/usr/include/bits/pthreadtypes.h" 1 3 4
#23 "/usr/include/bits/pthreadtypes.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#24 "/usr/include/bits/pthreadtypes.h" 2 3 4
#50 "/usr/include/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;


typedef union
{
  char __size[56];
  long int __align;
} pthread_attr_t;



typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;

#76 "/usr/include/bits/pthreadtypes.h" 3 4
typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;

    unsigned int __nusers;



    int __kind;

    int __spins;
    __pthread_list_t __list;
#101 "/usr/include/bits/pthreadtypes.h" 3 4
  } __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;





typedef union
{

  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;
    int __writer;
    int __shared;
    unsigned long int __pad1;
    unsigned long int __pad2;


    unsigned int __flags;
  } __data;
#187 "/usr/include/bits/pthreadtypes.h" 3 4
  char __size[56];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[32];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;

#272 "/usr/include/sys/types.h" 2 3 4



#321 "/usr/include/stdlib.h" 2 3 4






extern long int random( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern void srandom( unsigned int __seed ) __attribute__ ( ( __nothrow__, __leaf__ ) );





extern char *initstate( unsigned int __seed, char *__statebuf,
                        size_t __statelen ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );



extern char *setstate( char *__statebuf ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );







struct random_data
{
  int32_t *fptr;
  int32_t *rptr;
  int32_t *state;
  int rand_type;
  int rand_deg;
  int rand_sep;
  int32_t *end_ptr;
};

extern int random_r( struct random_data *__restrict __buf,
                     int32_t * __restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );

extern int srandom_r( unsigned int __seed, struct random_data *__buf )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );

extern int initstate_r( unsigned int __seed, char *__restrict __statebuf,
                        size_t __statelen,
                        struct random_data *__restrict __buf )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 4 ) ) );

extern int setstate_r( char *__restrict __statebuf,
                       struct random_data *__restrict __buf )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );






extern int rand( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );

extern void srand( unsigned int __seed ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern int rand_r( unsigned int *__seed ) __attribute__ ( ( __nothrow__, __leaf__ ) );







extern double drand48( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );
extern double erand48( unsigned short int __xsubi[3] ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


extern long int lrand48( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );
extern long int nrand48( unsigned short int __xsubi[3] ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


extern long int mrand48( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );
extern long int jrand48( unsigned short int __xsubi[3] ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


extern void srand48( long int __seedval ) __attribute__ ( ( __nothrow__, __leaf__ ) );
extern unsigned short int *seed48( unsigned short int __seed16v[3] )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
extern void lcong48( unsigned short int __param[7] ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





struct drand48_data
{
  unsigned short int __x[3];
  unsigned short int __old_x[3];
  unsigned short int __c;
  unsigned short int __init;
  unsigned long long int __a;
};


extern int drand48_r( struct drand48_data *__restrict __buffer,
                      double *__restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
extern int erand48_r( unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      double *__restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern int lrand48_r( struct drand48_data *__restrict __buffer,
                      long int *__restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
extern int nrand48_r( unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      long int *__restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern int mrand48_r( struct drand48_data *__restrict __buffer,
                      long int *__restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
extern int jrand48_r( unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      long int *__restrict __result ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern int srand48_r( long int __seedval, struct drand48_data *__buffer )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );

extern int seed48_r( unsigned short int __seed16v[3],
                     struct drand48_data *__buffer ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );

extern int lcong48_r( unsigned short int __param[7],
                      struct drand48_data *__buffer ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );









extern void *malloc( size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __malloc__ ) );

extern void *calloc( size_t __nmemb, size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __malloc__ ) );










extern void *realloc( void *__ptr, size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __warn_unused_result__ ) );

extern void free( void *__ptr ) __attribute__ ( ( __nothrow__, __leaf__ ) );




extern void cfree( void *__ptr ) __attribute__ ( ( __nothrow__, __leaf__ ) );



#1 "/usr/include/alloca.h" 1 3 4
#25 "/usr/include/alloca.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#26 "/usr/include/alloca.h" 2 3 4







extern void *alloca( size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) );






#498 "/usr/include/stdlib.h" 2 3 4





extern void *valloc( size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __malloc__ ) );




extern int posix_memalign( void **__memptr, size_t __alignment, size_t __size )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );




extern void abort( void ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __noreturn__ ) );



extern int atexit( void ( *__func ) ( void ) ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#531 "/usr/include/stdlib.h" 3 4





extern int on_exit( void ( *__func ) ( int __status, void *__arg ), void *__arg )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






extern void exit( int __status ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __noreturn__ ) );

#554 "/usr/include/stdlib.h" 3 4






extern void _Exit( int __status ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __noreturn__ ) );






extern char *getenv( __const char *__name ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );




extern char *__secure_getenv( __const char *__name ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern int putenv( char *__string ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern int setenv( __const char *__name, __const char *__value, int __replace )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );


extern int unsetenv( __const char *__name ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






extern int clearenv( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#606 "/usr/include/stdlib.h" 3 4
extern char *mktemp( char *__template ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#620 "/usr/include/stdlib.h" 3 4
extern int mkstemp( char *__template ) __attribute__ ( ( __nonnull__( 1 ) ) );

#642 "/usr/include/stdlib.h" 3 4
extern int mkstemps( char *__template, int __suffixlen ) __attribute__ ( ( __nonnull__( 1 ) ) );

#663 "/usr/include/stdlib.h" 3 4
extern char *mkdtemp( char *__template ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#712 "/usr/include/stdlib.h" 3 4





extern int system( __const char *__command );

#734 "/usr/include/stdlib.h" 3 4
extern char *realpath( __const char *__restrict __name, char *__restrict __resolved ) __attribute__ ( ( __nothrow__, __leaf__ ) );






typedef int ( *__compar_fn_t ) ( __const void *, __const void * );

#752 "/usr/include/stdlib.h" 3 4



extern void *bsearch( __const void *__key, __const void *__base,
                      size_t __nmemb, size_t __size, __compar_fn_t __compar ) __attribute__ ( ( __nonnull__( 1, 2, 5 ) ) );



extern void qsort( void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar ) __attribute__ ( ( __nonnull__( 1, 4 ) ) );

#771 "/usr/include/stdlib.h" 3 4
extern int abs( int __x ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
extern long int labs( long int __x ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );



__extension__ extern long long int llabs( long long int __x ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );







extern div_t div( int __numer, int __denom ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
extern ldiv_t ldiv( long int __numer, long int __denom ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );




__extension__ extern lldiv_t lldiv( long long int __numer,
                                    long long int __denom ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );

#808 "/usr/include/stdlib.h" 3 4
extern char *ecvt( double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4 ) ) );




extern char *fcvt( double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4 ) ) );




extern char *gcvt( double __value, int __ndigit, char *__buf )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3 ) ) );




extern char *qecvt( long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4 ) ) );
extern char *qfcvt( long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4 ) ) );
extern char *qgcvt( long double __value, int __ndigit, char *__buf )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3 ) ) );




extern int ecvt_r( double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign, char *__restrict __buf,
                   size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4, 5 ) ) );
extern int fcvt_r( double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign, char *__restrict __buf,
                   size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4, 5 ) ) );

extern int qecvt_r( long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign,
                    char *__restrict __buf, size_t __len )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4, 5 ) ) );
extern int qfcvt_r( long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign,
                    char *__restrict __buf, size_t __len )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3, 4, 5 ) ) );







extern int mblen( __const char *__s, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern int mbtowc( wchar_t * __restrict __pwc, __const char *__restrict __s, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );


extern int wctomb( char *__s, wchar_t __wchar ) __attribute__ ( ( __nothrow__, __leaf__ ) );



extern size_t mbstowcs( wchar_t * __restrict __pwcs, __const char *__restrict __s, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );

extern size_t wcstombs( char *__restrict __s, __const wchar_t * __restrict __pwcs, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );








extern int rpmatch( __const char *__response ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#896 "/usr/include/stdlib.h" 3 4
extern int getsubopt( char **__restrict __optionp,
                      char *__const * __restrict __tokens,
                      char **__restrict __valuep ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2, 3 ) ) );
#948 "/usr/include/stdlib.h" 3 4
extern int getloadavg( double __loadavg[], int __nelem ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );

#964 "/usr/include/stdlib.h" 3 4

#7 "src/mas_listener.c" 2
#1 "/usr/include/string.h" 1 3 4
#29 "/usr/include/string.h" 3 4





#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#35 "/usr/include/string.h" 2 3 4









extern void *memcpy( void *__restrict __dest,
                     __const void *__restrict __src, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern void *memmove( void *__dest, __const void *__src, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );






extern void *memccpy( void *__restrict __dest, __const void *__restrict __src,
                      int __c, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );





extern void *memset( void *__s, int __c, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


extern int memcmp( __const void *__s1, __const void *__s2, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#95 "/usr/include/string.h" 3 4
extern void *memchr( __const void *__s, int __c, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


#126 "/usr/include/string.h" 3 4


extern char *strcpy( char *__restrict __dest, __const char *__restrict __src )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );

extern char *strncpy( char *__restrict __dest,
                      __const char *__restrict __src, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern char *strcat( char *__restrict __dest, __const char *__restrict __src )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );

extern char *strncat( char *__restrict __dest, __const char *__restrict __src,
                      size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern int strcmp( __const char *__s1, __const char *__s2 )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );

extern int strncmp( __const char *__s1, __const char *__s2, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern int strcoll( __const char *__s1, __const char *__s2 )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );

extern size_t strxfrm( char *__restrict __dest,
                       __const char *__restrict __src, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );






#1 "/usr/include/xlocale.h" 1 3 4
#28 "/usr/include/xlocale.h" 3 4
typedef struct __locale_struct
{

  struct __locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *__locale_t;


typedef __locale_t locale_t;

#163 "/usr/include/string.h" 2 3 4


extern int strcoll_l( __const char *__s1, __const char *__s2, __locale_t __l )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2, 3 ) ) );

extern size_t strxfrm_l( char *__dest, __const char *__src, size_t __n,
                         __locale_t __l ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 4 ) ) );





extern char *strdup( __const char *__s )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __malloc__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






extern char *strndup( __const char *__string, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __malloc__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#210 "/usr/include/string.h" 3 4

#235 "/usr/include/string.h" 3 4
extern char *strchr( __const char *__s, int __c )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#262 "/usr/include/string.h" 3 4
extern char *strrchr( __const char *__s, int __c )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


#281 "/usr/include/string.h" 3 4



extern size_t strcspn( __const char *__s, __const char *__reject )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


extern size_t strspn( __const char *__s, __const char *__accept )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#314 "/usr/include/string.h" 3 4
extern char *strpbrk( __const char *__s, __const char *__accept )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#342 "/usr/include/string.h" 3 4
extern char *strstr( __const char *__haystack, __const char *__needle )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




extern char *strtok( char *__restrict __s, __const char *__restrict __delim )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );




extern char *__strtok_r( char *__restrict __s,
                         __const char *__restrict __delim,
                         char **__restrict __save_ptr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 3 ) ) );

extern char *strtok_r( char *__restrict __s, __const char *__restrict __delim,
                       char **__restrict __save_ptr ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 3 ) ) );
#397 "/usr/include/string.h" 3 4


extern size_t strlen( __const char *__s )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern size_t strnlen( __const char *__string, size_t __maxlen )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





extern char *strerror( int __errnum ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#427 "/usr/include/string.h" 3 4
extern int
strerror_r( int __errnum, char *__buf, size_t __buflen )
__asm__( "" "__xpg_strerror_r" ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );

#445 "/usr/include/string.h" 3 4
     extern char *strerror_l( int __errnum, __locale_t __l ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern void __bzero( void *__s, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern void bcopy( __const void *__src, void *__dest, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern void bzero( void *__s, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int bcmp( __const void *__s1, __const void *__s2, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#489 "/usr/include/string.h" 3 4
     extern char *index( __const char *__s, int __c )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#517 "/usr/include/string.h" 3 4
     extern char *rindex( __const char *__s, int __c )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );




     extern int ffs( int __i ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );

#536 "/usr/include/string.h" 3 4
     extern int strcasecmp( __const char *__s1, __const char *__s2 )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int strncasecmp( __const char *__s1, __const char *__s2, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __pure__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#559 "/usr/include/string.h" 3 4
     extern char *strsep( char **__restrict __stringp,
                          __const char *__restrict __delim )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




     extern char *strsignal( int __sig ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern char *__stpcpy( char *__restrict __dest, __const char *__restrict __src )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
     extern char *stpcpy( char *__restrict __dest, __const char *__restrict __src )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



     extern char *__stpncpy( char *__restrict __dest,
                             __const char *__restrict __src, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
     extern char *stpncpy( char *__restrict __dest,
                           __const char *__restrict __src, size_t __n )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#646 "/usr/include/string.h" 3 4

#8 "src/mas_listener.c" 2



#1 "/usr/include/pthread.h" 1 3 4
#25 "/usr/include/pthread.h" 3 4
#1 "/usr/include/sched.h" 1 3 4
#30 "/usr/include/sched.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#31 "/usr/include/sched.h" 2 3 4



#1 "/usr/include/time.h" 1 3 4
#35 "/usr/include/sched.h" 2 3 4
#43 "/usr/include/sched.h" 3 4
#1 "/usr/include/bits/sched.h" 1 3 4
#74 "/usr/include/bits/sched.h" 3 4
     struct sched_param
     {
       int __sched_priority;
     };


#97 "/usr/include/bits/sched.h" 3 4








     struct __sched_param
     {
       int __sched_priority;
     };
#120 "/usr/include/bits/sched.h" 3 4
     typedef unsigned long int __cpu_mask;






     typedef struct
     {
       __cpu_mask __bits[1024 / ( 8 * sizeof( __cpu_mask ) )];
     }
cpu_set_t;
#203 "/usr/include/bits/sched.h" 3 4


     extern int __sched_cpucount( size_t __setsize, const cpu_set_t * __setp ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern cpu_set_t *__sched_cpualloc( size_t __count ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern void __sched_cpufree( cpu_set_t * __set ) __attribute__ ( ( __nothrow__, __leaf__ ) );


#44 "/usr/include/sched.h" 2 3 4







     extern int sched_setparam( __pid_t __pid, __const struct sched_param *__param ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_getparam( __pid_t __pid, struct sched_param *__param ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_setscheduler( __pid_t __pid, int __policy,
                                    __const struct sched_param *__param ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_getscheduler( __pid_t __pid ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_yield( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_get_priority_max( int __algorithm ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_get_priority_min( int __algorithm ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int sched_rr_get_interval( __pid_t __pid, struct timespec *__t ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#126 "/usr/include/sched.h" 3 4

#26 "/usr/include/pthread.h" 2 3 4
#1 "/usr/include/time.h" 1 3 4
#30 "/usr/include/time.h" 3 4








#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#39 "/usr/include/time.h" 2 3 4



#1 "/usr/include/bits/time.h" 1 3 4
#43 "/usr/include/time.h" 2 3 4
#131 "/usr/include/time.h" 3 4


     struct tm
     {
       int tm_sec;
       int tm_min;
       int tm_hour;
       int tm_mday;
       int tm_mon;
       int tm_year;
       int tm_wday;
       int tm_yday;
       int tm_isdst;


       long int tm_gmtoff;
       __const char *tm_zone;




     };








     struct itimerspec
     {
       struct timespec it_interval;
       struct timespec it_value;
     };


     struct sigevent;

#180 "/usr/include/time.h" 3 4



     extern clock_t clock( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern time_t time( time_t * __timer ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern double difftime( time_t __time1, time_t __time0 ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );


     extern time_t mktime( struct tm *__tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern size_t strftime( char *__restrict __s, size_t __maxsize,
                             __const char *__restrict __format,
                             __const struct tm *__restrict __tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#217 "/usr/include/time.h" 3 4
     extern size_t strftime_l( char *__restrict __s, size_t __maxsize,
                               __const char *__restrict __format,
                               __const struct tm *__restrict __tp, __locale_t __loc ) __attribute__ ( ( __nothrow__, __leaf__ ) );
#230 "/usr/include/time.h" 3 4



     extern struct tm *gmtime( __const time_t * __timer ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern struct tm *localtime( __const time_t * __timer ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern struct tm *gmtime_r( __const time_t * __restrict __timer,
                                 struct tm *__restrict __tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern struct tm *localtime_r( __const time_t * __restrict __timer,
                                    struct tm *__restrict __tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern char *asctime( __const struct tm *__tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern char *ctime( __const time_t * __timer ) __attribute__ ( ( __nothrow__, __leaf__ ) );







     extern char *asctime_r( __const struct tm *__restrict __tp, char *__restrict __buf ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern char *ctime_r( __const time_t * __restrict __timer, char *__restrict __buf ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern char *__tzname[2];
     extern int __daylight;
     extern long int __timezone;




     extern char *tzname[2];



     extern void tzset( void ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern int daylight;
     extern long int timezone;





     extern int stime( __const time_t * __when ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#313 "/usr/include/time.h" 3 4
     extern time_t timegm( struct tm *__tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern time_t timelocal( struct tm *__tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int dysize( int __year ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );

#328 "/usr/include/time.h" 3 4
     extern int nanosleep( __const struct timespec *__requested_time, struct timespec *__remaining );



     extern int clock_getres( clockid_t __clock_id, struct timespec *__res ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int clock_gettime( clockid_t __clock_id, struct timespec *__tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int clock_settime( clockid_t __clock_id, __const struct timespec *__tp ) __attribute__ ( ( __nothrow__, __leaf__ ) );






     extern int clock_nanosleep( clockid_t __clock_id, int __flags, __const struct timespec *__req, struct timespec *__rem );


     extern int clock_getcpuclockid( pid_t __pid, clockid_t * __clock_id ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int timer_create( clockid_t __clock_id,
                              struct sigevent *__restrict __evp,
                              timer_t * __restrict __timerid ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int timer_delete( timer_t __timerid ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int timer_settime( timer_t __timerid, int __flags,
                               __const struct itimerspec *__restrict __value,
                               struct itimerspec *__restrict __ovalue ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int timer_gettime( timer_t __timerid, struct itimerspec *__value ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int timer_getoverrun( timer_t __timerid ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#417 "/usr/include/time.h" 3 4

#27 "/usr/include/pthread.h" 2 3 4


#1 "/usr/include/bits/setjmp.h" 1 3 4
#27 "/usr/include/bits/setjmp.h" 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#28 "/usr/include/bits/setjmp.h" 2 3 4




     typedef long int __jmp_buf[8];

#30 "/usr/include/pthread.h" 2 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#31 "/usr/include/pthread.h" 2 3 4



     enum
     {
       PTHREAD_CREATE_JOINABLE,

       PTHREAD_CREATE_DETACHED
     };



     enum
     {
       PTHREAD_MUTEX_TIMED_NP,
       PTHREAD_MUTEX_RECURSIVE_NP,
       PTHREAD_MUTEX_ERRORCHECK_NP,
       PTHREAD_MUTEX_ADAPTIVE_NP,
       PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_TIMED_NP,
       PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
       PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
       PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
     };




     enum
     {
       PTHREAD_MUTEX_STALLED,
       PTHREAD_MUTEX_STALLED_NP = PTHREAD_MUTEX_STALLED,
       PTHREAD_MUTEX_ROBUST,
       PTHREAD_MUTEX_ROBUST_NP = PTHREAD_MUTEX_ROBUST
     };
#115 "/usr/include/pthread.h" 3 4
     enum
     {
       PTHREAD_RWLOCK_PREFER_READER_NP,
       PTHREAD_RWLOCK_PREFER_WRITER_NP,
       PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
       PTHREAD_RWLOCK_DEFAULT_NP = PTHREAD_RWLOCK_PREFER_READER_NP
     };
#147 "/usr/include/pthread.h" 3 4
     enum
     {
       PTHREAD_INHERIT_SCHED,

       PTHREAD_EXPLICIT_SCHED
     };



     enum
     {
       PTHREAD_SCOPE_SYSTEM,

       PTHREAD_SCOPE_PROCESS
     };



     enum
     {
       PTHREAD_PROCESS_PRIVATE,

       PTHREAD_PROCESS_SHARED
     };
#182 "/usr/include/pthread.h" 3 4
     struct _pthread_cleanup_buffer
     {
       void ( *__routine ) ( void * );
       void *__arg;
       int __canceltype;
       struct _pthread_cleanup_buffer *__prev;
     };


     enum
     {
       PTHREAD_CANCEL_ENABLE,

       PTHREAD_CANCEL_DISABLE
     };
     enum
     {
       PTHREAD_CANCEL_DEFERRED,

       PTHREAD_CANCEL_ASYNCHRONOUS
     };
#220 "/usr/include/pthread.h" 3 4





     extern int pthread_create( pthread_t * __restrict __newthread,
                                __const pthread_attr_t * __restrict __attr,
                                void *( *__start_routine ) ( void * ),
                                void *__restrict __arg ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1, 3 ) ) );





     extern void pthread_exit( void *__retval ) __attribute__ ( ( __noreturn__ ) );







     extern int pthread_join( pthread_t __th, void **__thread_return );

#263 "/usr/include/pthread.h" 3 4
     extern int pthread_detach( pthread_t __th ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern pthread_t pthread_self( void ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );


     extern int pthread_equal( pthread_t __thread1, pthread_t __thread2 ) __attribute__ ( ( __nothrow__, __leaf__ ) );







     extern int pthread_attr_init( pthread_attr_t * __attr ) __attribute__ ( ( __nothrow__, __leaf__ ) )
      __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_attr_destroy( pthread_attr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_attr_getdetachstate( __const pthread_attr_t * __attr,
                                             int *__detachstate )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_setdetachstate( pthread_attr_t * __attr,
                                             int __detachstate )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_attr_getguardsize( __const pthread_attr_t * __attr,
                                           size_t * __guardsize )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_setguardsize( pthread_attr_t * __attr,
                                           size_t __guardsize )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_attr_getschedparam( __const pthread_attr_t * __restrict
                                            __attr,
                                            struct sched_param *__restrict __param )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_setschedparam( pthread_attr_t * __restrict __attr,
                                            __const struct sched_param *__restrict
                                            __param ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_getschedpolicy( __const pthread_attr_t * __restrict
                                             __attr, int *__restrict __policy )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_setschedpolicy( pthread_attr_t * __attr, int __policy )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_attr_getinheritsched( __const pthread_attr_t * __restrict
                                              __attr, int *__restrict __inherit )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_setinheritsched( pthread_attr_t * __attr,
                                              int __inherit )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_attr_getscope( __const pthread_attr_t * __restrict __attr,
                                       int *__restrict __scope )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_attr_setscope( pthread_attr_t * __attr, int __scope )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_attr_getstackaddr( __const pthread_attr_t * __restrict
                                           __attr, void **__restrict __stackaddr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) ) __attribute__ ( ( __deprecated__ ) );





     extern int pthread_attr_setstackaddr( pthread_attr_t * __attr,
                                           void *__stackaddr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) ) __attribute__ ( ( __deprecated__ ) );


     extern int pthread_attr_getstacksize( __const pthread_attr_t * __restrict
                                           __attr, size_t * __restrict __stacksize )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




     extern int pthread_attr_setstacksize( pthread_attr_t * __attr,
                                           size_t __stacksize )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_attr_getstack( __const pthread_attr_t * __restrict __attr,
                                       void **__restrict __stackaddr,
                                       size_t * __restrict __stacksize )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2, 3 ) ) );




     extern int pthread_attr_setstack( pthread_attr_t * __attr, void *__stackaddr,
                                       size_t __stacksize ) __attribute__ ( ( __nothrow__, __leaf__ ) )
      __attribute__ ( ( __nonnull__( 1 ) ) );
#413 "/usr/include/pthread.h" 3 4
     extern int pthread_setschedparam( pthread_t __target_thread, int __policy,
                                       __const struct sched_param *__param )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 3 ) ) );


     extern int pthread_getschedparam( pthread_t __target_thread,
                                       int *__restrict __policy,
                                       struct sched_param *__restrict __param )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2, 3 ) ) );


     extern int pthread_setschedprio( pthread_t __target_thread, int __prio ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#478 "/usr/include/pthread.h" 3 4
     extern int pthread_once( pthread_once_t * __once_control,
                              void ( *__init_routine ) ( void ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#490 "/usr/include/pthread.h" 3 4
     extern int pthread_setcancelstate( int __state, int *__oldstate );



     extern int pthread_setcanceltype( int __type, int *__oldtype );


     extern int pthread_cancel( pthread_t __th );




     extern void pthread_testcancel( void );




     typedef struct
     {
       struct
       {
         __jmp_buf __cancel_jmp_buf;
         int __mask_was_saved;
       } __cancel_jmp_buf[1];
       void *__pad[4];
     } __pthread_unwind_buf_t __attribute__ ( ( __aligned__ ) );

#524 "/usr/include/pthread.h" 3 4
     struct __pthread_cleanup_frame
     {
       void ( *__cancel_routine ) ( void * );
       void *__cancel_arg;
       int __do_it;
       int __cancel_type;
     };
#664 "/usr/include/pthread.h" 3 4
     extern void __pthread_register_cancel( __pthread_unwind_buf_t * __buf );

#676 "/usr/include/pthread.h" 3 4
     extern void __pthread_unregister_cancel( __pthread_unwind_buf_t * __buf );

#717 "/usr/include/pthread.h" 3 4
     extern void __pthread_unwind_next( __pthread_unwind_buf_t * __buf ) __attribute__ ( ( __noreturn__ ) ) __attribute__ ( ( __weak__ ) );



     struct __jmp_buf_tag;
     extern int __sigsetjmp( struct __jmp_buf_tag *__env, int __savemask ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern int pthread_mutex_init( pthread_mutex_t * __mutex,
                                    __const pthread_mutexattr_t * __mutexattr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_mutex_destroy( pthread_mutex_t * __mutex )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_mutex_trylock( pthread_mutex_t * __mutex ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_mutex_lock( pthread_mutex_t * __mutex ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_mutex_timedlock( pthread_mutex_t * __restrict __mutex,
                                         __const struct timespec *__restrict
                                         __abstime ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



     extern int pthread_mutex_unlock( pthread_mutex_t * __mutex ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_mutex_getprioceiling( __const pthread_mutex_t *
                                              __restrict __mutex,
                                              int *__restrict __prioceiling )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



     extern int pthread_mutex_setprioceiling( pthread_mutex_t * __restrict __mutex,
                                              int __prioceiling,
                                              int *__restrict __old_ceiling )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 3 ) ) );




     extern int pthread_mutex_consistent( pthread_mutex_t * __mutex )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#790 "/usr/include/pthread.h" 3 4
     extern int pthread_mutexattr_init( pthread_mutexattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_mutexattr_destroy( pthread_mutexattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_mutexattr_getpshared( __const pthread_mutexattr_t *
                                              __restrict __attr,
                                              int *__restrict __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_mutexattr_setpshared( pthread_mutexattr_t * __attr,
                                              int __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_mutexattr_gettype( __const pthread_mutexattr_t * __restrict
                                           __attr, int *__restrict __kind )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );




     extern int pthread_mutexattr_settype( pthread_mutexattr_t * __attr, int __kind )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_mutexattr_getprotocol( __const pthread_mutexattr_t *
                                               __restrict __attr,
                                               int *__restrict __protocol )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



     extern int pthread_mutexattr_setprotocol( pthread_mutexattr_t * __attr,
                                               int __protocol )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_mutexattr_getprioceiling( __const pthread_mutexattr_t *
                                                  __restrict __attr,
                                                  int *__restrict __prioceiling )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_mutexattr_setprioceiling( pthread_mutexattr_t * __attr,
                                                  int __prioceiling )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_mutexattr_getrobust( __const pthread_mutexattr_t * __attr,
                                             int *__robustness )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );







     extern int pthread_mutexattr_setrobust( pthread_mutexattr_t * __attr,
                                             int __robustness )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#872 "/usr/include/pthread.h" 3 4
     extern int pthread_rwlock_init( pthread_rwlock_t * __restrict __rwlock,
                                     __const pthread_rwlockattr_t * __restrict
                                     __attr ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlock_destroy( pthread_rwlock_t * __rwlock )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlock_rdlock( pthread_rwlock_t * __rwlock )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlock_tryrdlock( pthread_rwlock_t * __rwlock )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_rwlock_timedrdlock( pthread_rwlock_t * __restrict __rwlock,
                                            __const struct timespec *__restrict
                                            __abstime ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



     extern int pthread_rwlock_wrlock( pthread_rwlock_t * __rwlock )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlock_trywrlock( pthread_rwlock_t * __rwlock )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_rwlock_timedwrlock( pthread_rwlock_t * __restrict __rwlock,
                                            __const struct timespec *__restrict
                                            __abstime ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );



     extern int pthread_rwlock_unlock( pthread_rwlock_t * __rwlock )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





     extern int pthread_rwlockattr_init( pthread_rwlockattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlockattr_destroy( pthread_rwlockattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlockattr_getpshared( __const pthread_rwlockattr_t *
                                               __restrict __attr,
                                               int *__restrict __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_rwlockattr_setpshared( pthread_rwlockattr_t * __attr,
                                               int __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_rwlockattr_getkind_np( __const pthread_rwlockattr_t *
                                               __restrict __attr,
                                               int *__restrict __pref )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_rwlockattr_setkind_np( pthread_rwlockattr_t * __attr,
                                               int __pref ) __attribute__ ( ( __nothrow__, __leaf__ ) )
      __attribute__ ( ( __nonnull__( 1 ) ) );







     extern int pthread_cond_init( pthread_cond_t * __restrict __cond,
                                   __const pthread_condattr_t * __restrict
                                   __cond_attr ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_cond_destroy( pthread_cond_t * __cond )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_cond_signal( pthread_cond_t * __cond ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_cond_broadcast( pthread_cond_t * __cond ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






     extern int pthread_cond_wait( pthread_cond_t * __restrict __cond,
                                   pthread_mutex_t * __restrict __mutex ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );
#984 "/usr/include/pthread.h" 3 4
     extern int pthread_cond_timedwait( pthread_cond_t * __restrict __cond,
                                        pthread_mutex_t * __restrict __mutex,
                                        __const struct timespec *__restrict __abstime ) __attribute__ ( ( __nonnull__( 1, 2, 3 ) ) );




     extern int pthread_condattr_init( pthread_condattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_condattr_destroy( pthread_condattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_condattr_getpshared( __const pthread_condattr_t *
                                             __restrict __attr,
                                             int *__restrict __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_condattr_setpshared( pthread_condattr_t * __attr,
                                             int __pshared ) __attribute__ ( ( __nothrow__, __leaf__ ) )
      __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_condattr_getclock( __const pthread_condattr_t *
                                           __restrict __attr,
                                           __clockid_t * __restrict __clock_id )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_condattr_setclock( pthread_condattr_t * __attr,
                                           __clockid_t __clock_id )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#1028 "/usr/include/pthread.h" 3 4
     extern int pthread_spin_init( pthread_spinlock_t * __lock, int __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_spin_destroy( pthread_spinlock_t * __lock )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_spin_lock( pthread_spinlock_t * __lock ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_spin_trylock( pthread_spinlock_t * __lock )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_spin_unlock( pthread_spinlock_t * __lock ) __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );






     extern int pthread_barrier_init( pthread_barrier_t * __restrict __barrier,
                                      __const pthread_barrierattr_t * __restrict
                                      __attr, unsigned int __count )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_barrier_destroy( pthread_barrier_t * __barrier )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_barrier_wait( pthread_barrier_t * __barrier )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int pthread_barrierattr_init( pthread_barrierattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_barrierattr_destroy( pthread_barrierattr_t * __attr )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_barrierattr_getpshared( __const pthread_barrierattr_t *
                                                __restrict __attr,
                                                int *__restrict __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1, 2 ) ) );


     extern int pthread_barrierattr_setpshared( pthread_barrierattr_t * __attr,
                                                int __pshared )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );
#1095 "/usr/include/pthread.h" 3 4
     extern int pthread_key_create( pthread_key_t * __key,
                                    void ( *__destr_function ) ( void * ) )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int pthread_key_delete( pthread_key_t __key ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern void *pthread_getspecific( pthread_key_t __key ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int pthread_setspecific( pthread_key_t __key, __const void *__pointer ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int pthread_getcpuclockid( pthread_t __thread_id,
                                       __clockid_t * __clock_id )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 2 ) ) );
#1129 "/usr/include/pthread.h" 3 4
     extern int pthread_atfork( void ( *__prepare ) ( void ),
                                void ( *__parent ) ( void ), void ( *__child ) ( void ) ) __attribute__ ( ( __nothrow__, __leaf__ ) );
#1143 "/usr/include/pthread.h" 3 4

#12 "src/mas_listener.c" 2
#1 "/usr/include/sys/prctl.h" 1 3 4
#23 "/usr/include/sys/prctl.h" 3 4
#1 "/usr/include/linux/prctl.h" 1 3 4
#24 "/usr/include/sys/prctl.h" 2 3 4




     extern int prctl( int __option, ... ) __attribute__ ( ( __nothrow__, __leaf__ ) );


#13 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/wrap/mas_lib.h" 1



#1 "/usr/include/stdio.h" 1 3 4
#30 "/usr/include/stdio.h" 3 4




#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#35 "/usr/include/stdio.h" 2 3 4
#45 "/usr/include/stdio.h" 3 4
     struct _IO_FILE;



     typedef struct _IO_FILE FILE;





#65 "/usr/include/stdio.h" 3 4
     typedef struct _IO_FILE __FILE;

#75 "/usr/include/stdio.h" 3 4
#1 "/usr/include/libio.h" 1 3 4
#32 "/usr/include/libio.h" 3 4
#1 "/usr/include/_G_config.h" 1 3 4
#15 "/usr/include/_G_config.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#16 "/usr/include/_G_config.h" 2 3 4




#1 "/usr/include/wchar.h" 1 3 4
#83 "/usr/include/wchar.h" 3 4
     typedef struct
     {
       int __count;
       union
       {

         unsigned int __wch;



         char __wchb[4];
       } __value;
     } __mbstate_t;
#21 "/usr/include/_G_config.h" 2 3 4

     typedef struct
     {
       __off_t __pos;
       __mbstate_t __state;
     } _G_fpos_t;
     typedef struct
     {
       __off64_t __pos;
       __mbstate_t __state;
     } _G_fpos64_t;
#53 "/usr/include/_G_config.h" 3 4
     typedef int _G_int16_t __attribute__ ( ( __mode__( __HI__ ) ) );
     typedef int _G_int32_t __attribute__ ( ( __mode__( __SI__ ) ) );
     typedef unsigned int _G_uint16_t __attribute__ ( ( __mode__( __HI__ ) ) );
     typedef unsigned int _G_uint32_t __attribute__ ( ( __mode__( __SI__ ) ) );

#33 "/usr/include/libio.h" 2 3 4
#53 "/usr/include/libio.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stdarg.h" 1 3 4
#40 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stdarg.h" 3 4
     typedef __builtin_va_list __gnuc_va_list;

#54 "/usr/include/libio.h" 2 3 4
#172 "/usr/include/libio.h" 3 4
     struct _IO_jump_t;
     struct _IO_FILE;

#182 "/usr/include/libio.h" 3 4
     typedef void _IO_lock_t;





     struct _IO_marker
     {
       struct _IO_marker *_next;
       struct _IO_FILE *_sbuf;



       int _pos;
#205 "/usr/include/libio.h" 3 4
     };


     enum __codecvt_result
     {
       __codecvt_ok,
       __codecvt_partial,
       __codecvt_error,
       __codecvt_noconv
     };
#273 "/usr/include/libio.h" 3 4
     struct _IO_FILE
     {
       int _flags;




       char *_IO_read_ptr;
       char *_IO_read_end;
       char *_IO_read_base;
       char *_IO_write_base;
       char *_IO_write_ptr;
       char *_IO_write_end;
       char *_IO_buf_base;
       char *_IO_buf_end;

       char *_IO_save_base;
       char *_IO_backup_base;
       char *_IO_save_end;

       struct _IO_marker *_markers;

       struct _IO_FILE *_chain;

       int _fileno;



       int _flags2;

       __off_t _old_offset;



       unsigned short _cur_column;
       signed char _vtable_offset;
       char _shortbuf[1];



       _IO_lock_t *_lock;
#321 "/usr/include/libio.h" 3 4
       __off64_t _offset;
#330 "/usr/include/libio.h" 3 4
       void *__pad1;
       void *__pad2;
       void *__pad3;
       void *__pad4;
       size_t __pad5;

       int _mode;

       char _unused2[15 * sizeof( int ) - 4 * sizeof( void * ) - sizeof( size_t )];

     };


     typedef struct _IO_FILE _IO_FILE;


     struct _IO_FILE_plus;

     extern struct _IO_FILE_plus _IO_2_1_stdin_;
     extern struct _IO_FILE_plus _IO_2_1_stdout_;
     extern struct _IO_FILE_plus _IO_2_1_stderr_;

#366 "/usr/include/libio.h" 3 4
     typedef __ssize_t __io_read_fn( void *__cookie, char *__buf, size_t __nbytes );







     typedef __ssize_t __io_write_fn( void *__cookie, __const char *__buf, size_t __n );







     typedef int __io_seek_fn( void *__cookie, __off64_t * __pos, int __w );


     typedef int __io_close_fn( void *__cookie );

#418 "/usr/include/libio.h" 3 4
     extern int __underflow( _IO_FILE * );
     extern int __uflow( _IO_FILE * );
     extern int __overflow( _IO_FILE *, int );

#462 "/usr/include/libio.h" 3 4
     extern int _IO_getc( _IO_FILE * __fp );
     extern int _IO_putc( int __c, _IO_FILE * __fp );
     extern int _IO_feof( _IO_FILE * __fp ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern int _IO_ferror( _IO_FILE * __fp ) __attribute__ ( ( __nothrow__, __leaf__ ) );

     extern int _IO_peekc_locked( _IO_FILE * __fp );





     extern void _IO_flockfile( _IO_FILE * ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern void _IO_funlockfile( _IO_FILE * ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern int _IO_ftrylockfile( _IO_FILE * ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#492 "/usr/include/libio.h" 3 4
     extern int _IO_vfscanf( _IO_FILE * __restrict, const char *__restrict, __gnuc_va_list, int *__restrict );
     extern int _IO_vfprintf( _IO_FILE * __restrict, const char *__restrict, __gnuc_va_list );
     extern __ssize_t _IO_padn( _IO_FILE *, int, __ssize_t );
     extern size_t _IO_sgetn( _IO_FILE *, void *, size_t );

     extern __off64_t _IO_seekoff( _IO_FILE *, __off64_t, int, int );
     extern __off64_t _IO_seekpos( _IO_FILE *, __off64_t, int );

     extern void _IO_free_backup_area( _IO_FILE * ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#76 "/usr/include/stdio.h" 2 3 4




     typedef __gnuc_va_list va_list;

#109 "/usr/include/stdio.h" 3 4


     typedef _G_fpos_t fpos_t;




#165 "/usr/include/stdio.h" 3 4
#1 "/usr/include/bits/stdio_lim.h" 1 3 4
#166 "/usr/include/stdio.h" 2 3 4



     extern struct _IO_FILE *stdin;
     extern struct _IO_FILE *stdout;
     extern struct _IO_FILE *stderr;







     extern int remove( __const char *__filename ) __attribute__ ( ( __nothrow__, __leaf__ ) );

     extern int rename( __const char *__old, __const char *__new ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int renameat( int __oldfd, __const char *__old, int __newfd, __const char *__new ) __attribute__ ( ( __nothrow__, __leaf__ ) );








     extern FILE *tmpfile( void );

#210 "/usr/include/stdio.h" 3 4
     extern char *tmpnam( char *__s ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern char *tmpnam_r( char *__s ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#228 "/usr/include/stdio.h" 3 4
     extern char *tempnam( __const char *__dir, __const char *__pfx )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __malloc__ ) );








     extern int fclose( FILE * __stream );




     extern int fflush( FILE * __stream );

#253 "/usr/include/stdio.h" 3 4
     extern int fflush_unlocked( FILE * __stream );

#267 "/usr/include/stdio.h" 3 4






     extern FILE *fopen( __const char *__restrict __filename, __const char *__restrict __modes );




     extern FILE *freopen( __const char *__restrict __filename, __const char *__restrict __modes, FILE * __restrict __stream );

#296 "/usr/include/stdio.h" 3 4

#307 "/usr/include/stdio.h" 3 4
     extern FILE *fdopen( int __fd, __const char *__modes ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#320 "/usr/include/stdio.h" 3 4
     extern FILE *fmemopen( void *__s, size_t __len, __const char *__modes ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern FILE *open_memstream( char **__bufloc, size_t * __sizeloc ) __attribute__ ( ( __nothrow__, __leaf__ ) );






     extern void setbuf( FILE * __restrict __stream, char *__restrict __buf ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern int setvbuf( FILE * __restrict __stream, char *__restrict __buf,
                         int __modes, size_t __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern void setbuffer( FILE * __restrict __stream, char *__restrict __buf, size_t __size ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern void setlinebuf( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );








     extern int fprintf( FILE * __restrict __stream, __const char *__restrict __format, ... );




     extern int printf( __const char *__restrict __format, ... );

     extern int sprintf( char *__restrict __s, __const char *__restrict __format, ... ) __attribute__ ( ( __nothrow__ ) );





     extern int vfprintf( FILE * __restrict __s, __const char *__restrict __format, __gnuc_va_list __arg );




     extern int vprintf( __const char *__restrict __format, __gnuc_va_list __arg );

     extern int vsprintf( char *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg ) __attribute__ ( ( __nothrow__ ) );





     extern int snprintf( char *__restrict __s, size_t __maxlen,
                          __const char *__restrict __format, ... )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __format__( __printf__, 3, 4 ) ) );

     extern int vsnprintf( char *__restrict __s, size_t __maxlen,
                           __const char *__restrict __format, __gnuc_va_list __arg )
      __attribute__ ( ( __nothrow__ ) ) __attribute__ ( ( __format__( __printf__, 3, 0 ) ) );

#418 "/usr/include/stdio.h" 3 4
     extern int vdprintf( int __fd, __const char *__restrict __fmt,
                          __gnuc_va_list __arg ) __attribute__ ( ( __format__( __printf__, 2, 0 ) ) );
     extern int dprintf( int __fd, __const char *__restrict __fmt, ... ) __attribute__ ( ( __format__( __printf__, 2, 3 ) ) );








     extern int fscanf( FILE * __restrict __stream, __const char *__restrict __format, ... );




     extern int scanf( __const char *__restrict __format, ... );

     extern int sscanf( __const char *__restrict __s, __const char *__restrict __format, ... ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#449 "/usr/include/stdio.h" 3 4
     extern int fscanf( FILE * __restrict __stream, __const char *__restrict __format, ... ) __asm__( "" "__isoc99_fscanf" );
     extern int scanf( __const char *__restrict __format, ... ) __asm__( "" "__isoc99_scanf" );
     extern int sscanf( __const char *__restrict __s, __const char *__restrict __format, ... ) __asm__( "" "__isoc99_sscanf" )
      __attribute__ ( ( __nothrow__, __leaf__ ) );
#469 "/usr/include/stdio.h" 3 4








     extern int vfscanf( FILE * __restrict __s, __const char *__restrict __format,
                         __gnuc_va_list __arg ) __attribute__ ( ( __format__( __scanf__, 2, 0 ) ) );





     extern int vscanf( __const char *__restrict __format, __gnuc_va_list __arg ) __attribute__ ( ( __format__( __scanf__, 1, 0 ) ) );


     extern int vsscanf( __const char *__restrict __s,
                         __const char *__restrict __format, __gnuc_va_list __arg )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __format__( __scanf__, 2, 0 ) ) );
#500 "/usr/include/stdio.h" 3 4
     extern int vfscanf( FILE * __restrict __s, __const char *__restrict __format, __gnuc_va_list __arg ) __asm__( "" "__isoc99_vfscanf" )
      __attribute__ ( ( __format__( __scanf__, 2, 0 ) ) );
     extern int vscanf( __const char *__restrict __format, __gnuc_va_list __arg ) __asm__( "" "__isoc99_vscanf" )
      __attribute__ ( ( __format__( __scanf__, 1, 0 ) ) );
     extern int vsscanf( __const char *__restrict __s, __const char *__restrict __format,
                         __gnuc_va_list __arg ) __asm__( "" "__isoc99_vsscanf" ) __attribute__ ( ( __nothrow__, __leaf__ ) )
      __attribute__ ( ( __format__( __scanf__, 2, 0 ) ) );
#528 "/usr/include/stdio.h" 3 4









     extern int fgetc( FILE * __stream );
     extern int getc( FILE * __stream );





     extern int getchar( void );

#556 "/usr/include/stdio.h" 3 4
     extern int getc_unlocked( FILE * __stream );
     extern int getchar_unlocked( void );

#567 "/usr/include/stdio.h" 3 4
     extern int fgetc_unlocked( FILE * __stream );











     extern int fputc( int __c, FILE * __stream );
     extern int putc( int __c, FILE * __stream );





     extern int putchar( int __c );

#600 "/usr/include/stdio.h" 3 4
     extern int fputc_unlocked( int __c, FILE * __stream );







     extern int putc_unlocked( int __c, FILE * __stream );
     extern int putchar_unlocked( int __c );






     extern int getw( FILE * __stream );


     extern int putw( int __w, FILE * __stream );








     extern char *fgets( char *__restrict __s, int __n, FILE * __restrict __stream );






     extern char *gets( char *__s );

#662 "/usr/include/stdio.h" 3 4
     extern __ssize_t __getdelim( char **__restrict __lineptr, size_t * __restrict __n, int __delimiter, FILE * __restrict __stream );
     extern __ssize_t getdelim( char **__restrict __lineptr, size_t * __restrict __n, int __delimiter, FILE * __restrict __stream );







     extern __ssize_t getline( char **__restrict __lineptr, size_t * __restrict __n, FILE * __restrict __stream );








     extern int fputs( __const char *__restrict __s, FILE * __restrict __stream );





     extern int puts( __const char *__s );






     extern int ungetc( int __c, FILE * __stream );






     extern size_t fread( void *__restrict __ptr, size_t __size, size_t __n, FILE * __restrict __stream );




     extern size_t fwrite( __const void *__restrict __ptr, size_t __size, size_t __n, FILE * __restrict __s );

#734 "/usr/include/stdio.h" 3 4
     extern size_t fread_unlocked( void *__restrict __ptr, size_t __size, size_t __n, FILE * __restrict __stream );
     extern size_t fwrite_unlocked( __const void *__restrict __ptr, size_t __size, size_t __n, FILE * __restrict __stream );








     extern int fseek( FILE * __stream, long int __off, int __whence );




     extern long int ftell( FILE * __stream );




     extern void rewind( FILE * __stream );

#770 "/usr/include/stdio.h" 3 4
     extern int fseeko( FILE * __stream, __off_t __off, int __whence );




     extern __off_t ftello( FILE * __stream );

#789 "/usr/include/stdio.h" 3 4






     extern int fgetpos( FILE * __restrict __stream, fpos_t * __restrict __pos );




     extern int fsetpos( FILE * __stream, __const fpos_t * __pos );

#812 "/usr/include/stdio.h" 3 4

#821 "/usr/include/stdio.h" 3 4


     extern void clearerr( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );

     extern int feof( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );

     extern int ferror( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern void clearerr_unlocked( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern int feof_unlocked( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );
     extern int ferror_unlocked( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );








     extern void perror( __const char *__s );






#1 "/usr/include/bits/sys_errlist.h" 1 3 4
#27 "/usr/include/bits/sys_errlist.h" 3 4
     extern int sys_nerr;
     extern __const char *__const sys_errlist[];

#851 "/usr/include/stdio.h" 2 3 4




     extern int fileno( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int fileno_unlocked( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#870 "/usr/include/stdio.h" 3 4
     extern FILE *popen( __const char *__command, __const char *__modes );





     extern int pclose( FILE * __stream );





     extern char *ctermid( char *__s ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#910 "/usr/include/stdio.h" 3 4
     extern void flockfile( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern int ftrylockfile( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern void funlockfile( FILE * __stream ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#940 "/usr/include/stdio.h" 3 4

#5 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/wrap/mas_lib.h" 2
#1 "/usr/include/arpa/inet.h" 1 3 4
#23 "/usr/include/arpa/inet.h" 3 4
#1 "/usr/include/netinet/in.h" 1 3 4
#24 "/usr/include/netinet/in.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stdint.h" 1 3 4


#1 "/usr/include/stdint.h" 1 3 4
#27 "/usr/include/stdint.h" 3 4
#1 "/usr/include/bits/wchar.h" 1 3 4
#28 "/usr/include/stdint.h" 2 3 4
#1 "/usr/include/bits/wordsize.h" 1 3 4
#29 "/usr/include/stdint.h" 2 3 4
#49 "/usr/include/stdint.h" 3 4
     typedef unsigned char uint8_t;
     typedef unsigned short int uint16_t;

     typedef unsigned int uint32_t;



     typedef unsigned long int uint64_t;

#66 "/usr/include/stdint.h" 3 4
     typedef signed char int_least8_t;
     typedef short int int_least16_t;
     typedef int int_least32_t;

     typedef long int int_least64_t;






     typedef unsigned char uint_least8_t;
     typedef unsigned short int uint_least16_t;
     typedef unsigned int uint_least32_t;

     typedef unsigned long int uint_least64_t;

#91 "/usr/include/stdint.h" 3 4
     typedef signed char int_fast8_t;

     typedef long int int_fast16_t;
     typedef long int int_fast32_t;
     typedef long int int_fast64_t;

#104 "/usr/include/stdint.h" 3 4
     typedef unsigned char uint_fast8_t;

     typedef unsigned long int uint_fast16_t;
     typedef unsigned long int uint_fast32_t;
     typedef unsigned long int uint_fast64_t;

#123 "/usr/include/stdint.h" 3 4
     typedef unsigned long int uintptr_t;

#135 "/usr/include/stdint.h" 3 4
     typedef long int intmax_t;
     typedef unsigned long int uintmax_t;

#4 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stdint.h" 2 3 4
#25 "/usr/include/netinet/in.h" 2 3 4
#1 "/usr/include/sys/socket.h" 1 3 4
#26 "/usr/include/sys/socket.h" 3 4


#1 "/usr/include/sys/uio.h" 1 3 4
#26 "/usr/include/sys/uio.h" 3 4



#1 "/usr/include/bits/uio.h" 1 3 4
#44 "/usr/include/bits/uio.h" 3 4
     struct iovec
     {
       void *iov_base;
       size_t iov_len;
     };









     extern ssize_t process_vm_readv( pid_t __pid, __const struct iovec *__lvec,
                                      unsigned long int __liovcnt,
                                      __const struct iovec *__rvec,
                                      unsigned long int __riovcnt, unsigned long int __flags ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern ssize_t process_vm_writev( pid_t __pid, __const struct iovec *__lvec,
                                       unsigned long int __liovcnt,
                                       __const struct iovec *__rvec,
                                       unsigned long int __riovcnt, unsigned long int __flags ) __attribute__ ( ( __nothrow__, __leaf__ ) );


#30 "/usr/include/sys/uio.h" 2 3 4
#40 "/usr/include/sys/uio.h" 3 4
     extern ssize_t readv( int __fd, __const struct iovec *__iovec, int __count );

#51 "/usr/include/sys/uio.h" 3 4
     extern ssize_t writev( int __fd, __const struct iovec *__iovec, int __count );

#66 "/usr/include/sys/uio.h" 3 4
     extern ssize_t preadv( int __fd, __const struct iovec *__iovec, int __count, __off_t __offset );

#78 "/usr/include/sys/uio.h" 3 4
     extern ssize_t pwritev( int __fd, __const struct iovec *__iovec, int __count, __off_t __offset );

#121 "/usr/include/sys/uio.h" 3 4

#29 "/usr/include/sys/socket.h" 2 3 4

#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#31 "/usr/include/sys/socket.h" 2 3 4
#40 "/usr/include/sys/socket.h" 3 4
#1 "/usr/include/bits/socket.h" 1 3 4
#29 "/usr/include/bits/socket.h" 3 4
#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stddef.h" 1 3 4
#30 "/usr/include/bits/socket.h" 2 3 4
#40 "/usr/include/bits/socket.h" 3 4
     enum __socket_type
     {
       SOCK_STREAM = 1,


       SOCK_DGRAM = 2,


       SOCK_RAW = 3,

       SOCK_RDM = 4,

       SOCK_SEQPACKET = 5,


       SOCK_DCCP = 6,

       SOCK_PACKET = 10,







       SOCK_CLOEXEC = 02000000,


       SOCK_NONBLOCK = 04000
     };
#177 "/usr/include/bits/socket.h" 3 4
#1 "/usr/include/bits/sockaddr.h" 1 3 4
#29 "/usr/include/bits/sockaddr.h" 3 4
     typedef unsigned short int sa_family_t;

#178 "/usr/include/bits/socket.h" 2 3 4


     struct sockaddr
     {
       sa_family_t sa_family;
       char sa_data[14];
     };
#193 "/usr/include/bits/socket.h" 3 4
     struct sockaddr_storage
     {
       sa_family_t ss_family;
       unsigned long int __ss_align;
       char __ss_padding[( 128 - ( 2 * sizeof( unsigned long int ) ) )];
     };



     enum
     {
       MSG_OOB = 0x01,

       MSG_PEEK = 0x02,

       MSG_DONTROUTE = 0x04,






       MSG_CTRUNC = 0x08,

       MSG_PROXY = 0x10,

       MSG_TRUNC = 0x20,

       MSG_DONTWAIT = 0x40,

       MSG_EOR = 0x80,

       MSG_WAITALL = 0x100,

       MSG_FIN = 0x200,

       MSG_SYN = 0x400,

       MSG_CONFIRM = 0x800,

       MSG_RST = 0x1000,

       MSG_ERRQUEUE = 0x2000,

       MSG_NOSIGNAL = 0x4000,

       MSG_MORE = 0x8000,

       MSG_WAITFORONE = 0x10000,


       MSG_CMSG_CLOEXEC = 0x40000000
     };




     struct msghdr
     {
       void *msg_name;
       socklen_t msg_namelen;

       struct iovec *msg_iov;
       size_t msg_iovlen;

       void *msg_control;
       size_t msg_controllen;




       int msg_flags;
     };
#280 "/usr/include/bits/socket.h" 3 4
     struct cmsghdr
     {
       size_t cmsg_len;




       int cmsg_level;
       int cmsg_type;

       __extension__ unsigned char __cmsg_data[];

     };
#310 "/usr/include/bits/socket.h" 3 4
     extern struct cmsghdr *__cmsg_nxthdr( struct msghdr *__mhdr, struct cmsghdr *__cmsg ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#337 "/usr/include/bits/socket.h" 3 4
     enum
     {
       SCM_RIGHTS = 0x01
     };
#383 "/usr/include/bits/socket.h" 3 4
#1 "/usr/include/asm/socket.h" 1 3 4
#1 "/usr/include/asm-generic/socket.h" 1 3 4



#1 "/usr/include/asm/sockios.h" 1 3 4
#1 "/usr/include/asm-generic/sockios.h" 1 3 4
#1 "/usr/include/asm/sockios.h" 2 3 4
#5 "/usr/include/asm-generic/socket.h" 2 3 4
#1 "/usr/include/asm/socket.h" 2 3 4
#384 "/usr/include/bits/socket.h" 2 3 4
#417 "/usr/include/bits/socket.h" 3 4
     struct linger
     {
       int l_onoff;
       int l_linger;
     };









     extern int recvmmsg( int __fd, struct mmsghdr *__vmessages, unsigned int __vlen, int __flags, __const struct timespec *__tmo );





     extern int sendmmsg( int __fd, struct mmsghdr *__vmessages, unsigned int __vlen, int __flags );


#41 "/usr/include/sys/socket.h" 2 3 4




     struct osockaddr
     {
       unsigned short int sa_family;
       unsigned char sa_data[14];
     };




     enum
     {
       SHUT_RD = 0,

       SHUT_WR,

       SHUT_RDWR
     };
#105 "/usr/include/sys/socket.h" 3 4
     extern int socket( int __domain, int __type, int __protocol ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern int socketpair( int __domain, int __type, int __protocol, int __fds[2] ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int bind( int __fd, __const struct sockaddr *__addr, socklen_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int getsockname( int __fd, struct sockaddr *__restrict __addr,
                             socklen_t * __restrict __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );
#129 "/usr/include/sys/socket.h" 3 4
     extern int connect( int __fd, __const struct sockaddr *__addr, socklen_t __len );



     extern int getpeername( int __fd, struct sockaddr *__restrict __addr,
                             socklen_t * __restrict __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );






     extern ssize_t send( int __fd, __const void *__buf, size_t __n, int __flags );






     extern ssize_t recv( int __fd, void *__buf, size_t __n, int __flags );






     extern ssize_t sendto( int __fd, __const void *__buf, size_t __n, int __flags, __const struct sockaddr *__addr, socklen_t __addr_len );

#166 "/usr/include/sys/socket.h" 3 4
     extern ssize_t recvfrom( int __fd, void *__restrict __buf, size_t __n,
                              int __flags, struct sockaddr *__restrict __addr, socklen_t * __restrict __addr_len );







     extern ssize_t sendmsg( int __fd, __const struct msghdr *__message, int __flags );






     extern ssize_t recvmsg( int __fd, struct msghdr *__message, int __flags );





     extern int getsockopt( int __fd, int __level, int __optname,
                            void *__restrict __optval, socklen_t * __restrict __optlen ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int setsockopt( int __fd, int __level, int __optname,
                            __const void *__optval, socklen_t __optlen ) __attribute__ ( ( __nothrow__, __leaf__ ) );





     extern int listen( int __fd, int __n ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#214 "/usr/include/sys/socket.h" 3 4
     extern int accept( int __fd, struct sockaddr *__restrict __addr, socklen_t * __restrict __addr_len );

#232 "/usr/include/sys/socket.h" 3 4
     extern int shutdown( int __fd, int __how ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int sockatmark( int __fd ) __attribute__ ( ( __nothrow__, __leaf__ ) );







     extern int isfdtype( int __fd, int __fdtype ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#254 "/usr/include/sys/socket.h" 3 4

#26 "/usr/include/netinet/in.h" 2 3 4






     enum
     {
       IPPROTO_IP = 0,

       IPPROTO_HOPOPTS = 0,

       IPPROTO_ICMP = 1,

       IPPROTO_IGMP = 2,

       IPPROTO_IPIP = 4,

       IPPROTO_TCP = 6,

       IPPROTO_EGP = 8,

       IPPROTO_PUP = 12,

       IPPROTO_UDP = 17,

       IPPROTO_IDP = 22,

       IPPROTO_TP = 29,

       IPPROTO_DCCP = 33,

       IPPROTO_IPV6 = 41,

       IPPROTO_ROUTING = 43,

       IPPROTO_FRAGMENT = 44,

       IPPROTO_RSVP = 46,

       IPPROTO_GRE = 47,

       IPPROTO_ESP = 50,

       IPPROTO_AH = 51,

       IPPROTO_ICMPV6 = 58,

       IPPROTO_NONE = 59,

       IPPROTO_DSTOPTS = 60,

       IPPROTO_MTP = 92,

       IPPROTO_ENCAP = 98,

       IPPROTO_PIM = 103,

       IPPROTO_COMP = 108,

       IPPROTO_SCTP = 132,

       IPPROTO_UDPLITE = 136,

       IPPROTO_RAW = 255,

       IPPROTO_MAX
     };



     typedef uint16_t in_port_t;


     enum
     {
       IPPORT_ECHO = 7,
       IPPORT_DISCARD = 9,
       IPPORT_SYSTAT = 11,
       IPPORT_DAYTIME = 13,
       IPPORT_NETSTAT = 15,
       IPPORT_FTP = 21,
       IPPORT_TELNET = 23,
       IPPORT_SMTP = 25,
       IPPORT_TIMESERVER = 37,
       IPPORT_NAMESERVER = 42,
       IPPORT_WHOIS = 43,
       IPPORT_MTP = 57,

       IPPORT_TFTP = 69,
       IPPORT_RJE = 77,
       IPPORT_FINGER = 79,
       IPPORT_TTYLINK = 87,
       IPPORT_SUPDUP = 95,


       IPPORT_EXECSERVER = 512,
       IPPORT_LOGINSERVER = 513,
       IPPORT_CMDSERVER = 514,
       IPPORT_EFSSERVER = 520,


       IPPORT_BIFFUDP = 512,
       IPPORT_WHOSERVER = 513,
       IPPORT_ROUTESERVER = 520,


       IPPORT_RESERVED = 1024,


       IPPORT_USERRESERVED = 5000
     };



     typedef uint32_t in_addr_t;
     struct in_addr
     {
       in_addr_t s_addr;
     };
#198 "/usr/include/netinet/in.h" 3 4
     struct in6_addr
     {
       union
       {
         uint8_t __u6_addr8[16];

         uint16_t __u6_addr16[8];
         uint32_t __u6_addr32[4];

       } __in6_u;





     };

     extern const struct in6_addr in6addr_any;
     extern const struct in6_addr in6addr_loopback;

#225 "/usr/include/netinet/in.h" 3 4
     struct sockaddr_in
     {
       sa_family_t sin_family;
       in_port_t sin_port;
       struct in_addr sin_addr;


       unsigned char sin_zero[sizeof( struct sockaddr ) -
                              ( sizeof( unsigned short int ) ) - sizeof( in_port_t ) - sizeof( struct in_addr )];
     };


     struct sockaddr_in6
     {
       sa_family_t sin6_family;
       in_port_t sin6_port;
       uint32_t sin6_flowinfo;
       struct in6_addr sin6_addr;
       uint32_t sin6_scope_id;
     };




     struct ip_mreq
     {

       struct in_addr imr_multiaddr;


       struct in_addr imr_interface;
     };

     struct ip_mreq_source
     {

       struct in_addr imr_multiaddr;


       struct in_addr imr_interface;


       struct in_addr imr_sourceaddr;
     };




     struct ipv6_mreq
     {

       struct in6_addr ipv6mr_multiaddr;


       unsigned int ipv6mr_interface;
     };




     struct group_req
     {

       uint32_t gr_interface;


       struct sockaddr_storage gr_group;
     };

     struct group_source_req
     {

       uint32_t gsr_interface;


       struct sockaddr_storage gsr_group;


       struct sockaddr_storage gsr_source;
     };



     struct ip_msfilter
     {

       struct in_addr imsf_multiaddr;


       struct in_addr imsf_interface;


       uint32_t imsf_fmode;


       uint32_t imsf_numsrc;

       struct in_addr imsf_slist[1];
     };





     struct group_filter
     {

       uint32_t gf_interface;


       struct sockaddr_storage gf_group;


       uint32_t gf_fmode;


       uint32_t gf_numsrc;

       struct sockaddr_storage gf_slist[1];
     };
#356 "/usr/include/netinet/in.h" 3 4
#1 "/usr/include/bits/in.h" 1 3 4
#99 "/usr/include/bits/in.h" 3 4
     struct ip_opts
     {
       struct in_addr ip_dst;
       char ip_opts[40];
     };


     struct ip_mreqn
     {
       struct in_addr imr_multiaddr;
       struct in_addr imr_address;
       int imr_ifindex;
     };


     struct in_pktinfo
     {
       int ipi_ifindex;
       struct in_addr ipi_spec_dst;
       struct in_addr ipi_addr;
     };
#357 "/usr/include/netinet/in.h" 2 3 4
#365 "/usr/include/netinet/in.h" 3 4
     extern uint32_t ntohl( uint32_t __netlong ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
     extern uint16_t ntohs( uint16_t __netshort ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
     extern uint32_t htonl( uint32_t __hostlong ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );
     extern uint16_t htons( uint16_t __hostshort ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );




#1 "/usr/include/bits/byteswap.h" 1 3 4
#377 "/usr/include/netinet/in.h" 2 3 4
#492 "/usr/include/netinet/in.h" 3 4
     extern int bindresvport( int __sockfd, struct sockaddr_in *__sock_in ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern int bindresvport6( int __sockfd, struct sockaddr_in6 *__sock_in ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#617 "/usr/include/netinet/in.h" 3 4

#24 "/usr/include/arpa/inet.h" 2 3 4











     extern in_addr_t inet_addr( __const char *__cp ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern in_addr_t inet_lnaof( struct in_addr __in ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern struct in_addr inet_makeaddr( in_addr_t __net, in_addr_t __host ) __attribute__ ( ( __nothrow__, __leaf__ ) );


     extern in_addr_t inet_netof( struct in_addr __in ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern in_addr_t inet_network( __const char *__cp ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern char *inet_ntoa( struct in_addr __in ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int inet_pton( int __af, __const char *__restrict __cp, void *__restrict __buf ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern __const char *inet_ntop( int __af, __const void *__restrict __cp,
                                     char *__restrict __buf, socklen_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );






     extern int inet_aton( __const char *__cp, struct in_addr *__inp ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern char *inet_neta( in_addr_t __net, char *__buf, size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern char *inet_net_ntop( int __af, __const void *__cp, int __bits,
                                 char *__buf, size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int inet_net_pton( int __af, __const char *__cp, void *__buf, size_t __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern unsigned int inet_nsap_addr( __const char *__cp, unsigned char *__buf, int __len ) __attribute__ ( ( __nothrow__, __leaf__ ) );



     extern char *inet_nsap_ntoa( int __len, __const unsigned char *__cp, char *__buf ) __attribute__ ( ( __nothrow__, __leaf__ ) );



#6 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/wrap/mas_lib.h" 2






     int mas_accept( int sockfd, struct sockaddr *addr, socklen_t * addrlen );
     int mas_connect( int sockfd, const struct sockaddr *addr, socklen_t addrlen );
     int mas_bind( int sockfd, const struct sockaddr *addr, socklen_t addrlen );
     int mas_getsockopt( int sockfd, int level, int optname, void *optval, socklen_t * optlen );
     int mas_setsockopt( int sockfd, int level, int optname, const void *optval, socklen_t optlen );
     const char *mas_inet_ntop( int af, const void *src, char *dst, socklen_t size );
     char *mas_inet_ntoa( struct in_addr in );
     int mas_inet_pton( int af, const char *src, void *dst );

     int mas_listen( int sockfd, int backlog );
     int mas_mkfifo( const char *pathname, mode_t mode );


     FILE *mas_fopen( const char *path, const char *mode );
     int mas_fclose( FILE * fp );

     ssize_t mas_read( int fd, void *buf, size_t count );
     ssize_t mas_write( int fd, const void *buf, size_t count );


     void mas_nanosleep( double s );
     pid_t mas_gettid( void );

#15 "src/mas_listener.c" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/wrap/mas_lib_thread.h" 1






     int mas_pthread_cancel( pthread_t th );

     int mas_pthread_create( pthread_t * thread, const pthread_attr_t * attr, void *( *start_routine ) ( void * ), void *arg );



     int mas_pthread_mutex_lock( pthread_mutex_t * mutex );
     int mas_pthread_mutex_trylock( pthread_mutex_t * mutex );
     int mas_pthread_mutex_unlock( pthread_mutex_t * mutex );

     pthread_t mas_pthread_self( void );

     int mas_pthread_join( pthread_t thread, void **retval );
     int mas_xpthread_join( pthread_t thread );

     int mas_pthread_tryjoin_np( pthread_t thread, void **retval );
     int mas_xpthread_tryjoin_np( pthread_t thread );
     int mas_pthread_timedjoin_np( pthread_t thread, void **retval, const struct timespec *abstime );
     int mas_xpthread_timedjoin_np( pthread_t thread, const struct timespec *abstime );

     int mas_pthread_attr_init( pthread_attr_t * attr );
     int mas_pthread_attr_destroy( pthread_attr_t * attr );

     int mas_pthread_detach( pthread_t thread );
     void mas_pthread_exit( void *retval );

#16 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_def.h" 1



#1 "/usr/include/errno.h" 1 3 4
#32 "/usr/include/errno.h" 3 4




#1 "/usr/include/bits/errno.h" 1 3 4
#25 "/usr/include/bits/errno.h" 3 4
#1 "/usr/include/linux/errno.h" 1 3 4
#1 "/usr/include/asm/errno.h" 1 3 4
#1 "/usr/include/asm-generic/errno.h" 1 3 4



#1 "/usr/include/asm-generic/errno-base.h" 1 3 4
#5 "/usr/include/asm-generic/errno.h" 2 3 4
#1 "/usr/include/asm/errno.h" 2 3 4
#1 "/usr/include/linux/errno.h" 2 3 4
#26 "/usr/include/bits/errno.h" 2 3 4
#47 "/usr/include/bits/errno.h" 3 4
     extern int *__errno_location( void ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __const__ ) );

#37 "/usr/include/errno.h" 2 3 4
#59 "/usr/include/errno.h" 3 4

#5 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_def.h" 2

     enum mas_msg_type_e
     {
       MAS_MSG_NONE,
       MAS_MSG_TITLE,
       MAS_MSG_SUBTITLE,
       MAS_MSG_INFO,
       MAS_MSG_NOTICE,
       MAS_MSG_WATCH,
       MAS_MSG_WARNING,
       MAS_MSG_TRAP,
       MAS_MSG_DEBUG,

       MAS_MSG_ERR,
       MAS_MSG_CRIT,
       MAS_MSG_ALERT,
       MAS_MSG_EMERG,
       MAS_MSG_MAX
     };
     typedef enum mas_msg_type_e mas_msg_type_t;
     enum mas_msg_field_type_e
     {
       MAS_MSG_FIELD_ELAPSED,
       MAS_MSG_FIELD_ERRCODE,
       MAS_MSG_FIELD_CONSUME,
       MAS_MSG_FIELD_PREFIX,
       MAS_MSG_FIELD_CODEPOS,
       MAS_MSG_FIELD_MESSAGE,
       MAS_MSG_FIELD_SUFFIX,
       MAS_MSG_FIELD_PID,
       MAS_MSG_FIELD_TID,
       MAS_MSG_FIELD_PIDNAME,
       MAS_MSG_FIELD_TIDNAME,
       MAS_MSG_FIELD_THREAD_INFO,
       MAS_MSG_FIELD_THREAD_TYPE_NAME,

       MAS_MSG_FIELD_THREAD_TYPE_NAME_MAIN,
       MAS_MSG_FIELD_THREAD_TYPE_NAME_MASTER,
       MAS_MSG_FIELD_THREAD_TYPE_NAME_LISTENER,
       MAS_MSG_FIELD_THREAD_TYPE_NAME_TRANSACTION,
       MAS_MSG_FIELD_THREAD_TYPE_NAME_TICKER,
       MAS_MSG_FIELD_THREAD_TYPE_NAME_WATCHER,
       MAS_MSG_FIELD_THREAD_TYPE_NAME_LOGGER,

       MAS_MSG_FIELD_THREAD_STATUS,
       MAS_MSG_FIELD_EOL,
     };
     typedef enum mas_msg_field_type_e mas_msg_field_type_t;
     enum mas_msg_color_e
     {
       MAS_MSG_COLOR_NONE,
       MAS_MSG_COLOR_BLACK,
       MAS_MSG_COLOR_RED,
       MAS_MSG_COLOR_GREEN,
       MAS_MSG_COLOR_YELLOW,
       MAS_MSG_COLOR_BLUE,
       MAS_MSG_COLOR_MAGENTA,
       MAS_MSG_COLOR_CYAN,
       MAS_MSG_COLOR_WHITE,
       MAS_MSG_COLOR_BLACKP,
       MAS_MSG_COLOR_REDP,
       MAS_MSG_COLOR_GREENP,
       MAS_MSG_COLOR_YELLOWP,
       MAS_MSG_COLOR_BLUEP,
       MAS_MSG_COLOR_MAGENTAP,
       MAS_MSG_COLOR_CYANP,
       MAS_MSG_COLOR_WHITEP,
     };
     typedef enum mas_msg_color_e mas_msg_color_t;

#87 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_def.h"
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_msgfl_def.h" 1
#88 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_def.h" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_mfp_def.h" 1
#89 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_def.h" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_misc_def.h" 1





#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_opts_types.h" 1




#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_common_types.h" 1



#1 "/usr/include/sys/time.h" 1 3 4
#29 "/usr/include/sys/time.h" 3 4
#1 "/usr/include/bits/time.h" 1 3 4
#30 "/usr/include/sys/time.h" 2 3 4
#39 "/usr/include/sys/time.h" 3 4

#57 "/usr/include/sys/time.h" 3 4
     struct timezone
     {
       int tz_minuteswest;
       int tz_dsttime;
     };

     typedef struct timezone *__restrict __timezone_ptr_t;

#73 "/usr/include/sys/time.h" 3 4
     extern int gettimeofday( struct timeval *__restrict __tv,
                              __timezone_ptr_t __tz ) __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );




     extern int settimeofday( __const struct timeval *__tv,
                              __const struct timezone *__tz )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );





     extern int adjtime( __const struct timeval *__delta, struct timeval *__olddelta ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     enum __itimer_which
     {

       ITIMER_REAL = 0,


       ITIMER_VIRTUAL = 1,



       ITIMER_PROF = 2
     };



     struct itimerval
     {

       struct timeval it_interval;

       struct timeval it_value;
     };






     typedef int __itimer_which_t;




     extern int getitimer( __itimer_which_t __which, struct itimerval *__value ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int setitimer( __itimer_which_t __which,
                           __const struct itimerval *__restrict __new,
                           struct itimerval *__restrict __old ) __attribute__ ( ( __nothrow__, __leaf__ ) );




     extern int utimes( __const char *__file, __const struct timeval __tvp[2] )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );



     extern int lutimes( __const char *__file, __const struct timeval __tvp[2] )
      __attribute__ ( ( __nothrow__, __leaf__ ) ) __attribute__ ( ( __nonnull__( 1 ) ) );


     extern int futimes( int __fd, __const struct timeval __tvp[2] ) __attribute__ ( ( __nothrow__, __leaf__ ) );

#191 "/usr/include/sys/time.h" 3 4

#5 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_common_types.h" 2

#1 "/usr/lib/gcc/x86_64-pc-linux-gnu/4.6.3/include/stdarg.h" 1 3 4
#7 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_common_types.h" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_list_def.h" 1



#1 "/usr/include/sys/queue.h" 1 3 4
#5 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_list_def.h" 2
#9 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_common_types.h" 2

     typedef int ( *mas_error_handler_fun_t ) ( const char *func, int line, int issys, int rcode, int ierrno, int *perrno, int *pserrno,
                                                const char *fmt, const char *xmsg );
     typedef void ( *mas_voidp_fun_t ) ( void *arg );

     typedef struct mas_string_csetv_s
     {
       int c;
       char *const *v;
     } mas_string_csetv_t;

     typedef struct mas_string_setv_s
     {
       int c;
       char **v;
     } mas_string_setv_t;


     typedef enum
     {
       MAS_STATUS_NONE,

       MAS_STATUS_START,

       MAS_STATUS_BIRTH,
       MAS_STATUS_INIT,

       MAS_STATUS_SERV_LOOP,

       MAS_STATUS_OPEN,
       MAS_STATUS_NEED_DATA,
       MAS_STATUS_OPENB,

       MAS_STATUS_PROTO,

       MAS_STATUS_WORK,

       MAS_STATUS_CLOSE,
       MAS_STATUS_STOP,
       MAS_STATUS_END,
       MAS_STATUS_DEATH,
     } mas_status_t;
     typedef enum
     {
       MAS_SUBSTATUS_NONE,
       MAS_SUBSTATUS_INIT,
       MAS_SUBSTATUS_PARSE,
       MAS_SUBSTATUS_MAKE,
       MAS_SUBSTATUS_MAKEA,
       MAS_SUBSTATUS_END,
     } mas_substatus_t;


     struct mas_stamp_s
     {
       char vtsc[16];



       uint64_t lts;
       uint64_t first_lts;
       uint64_t prev_lts;
       uint64_t start_time;
     };
     typedef struct mas_stamp_s mas_stamp_t;

     typedef enum
     {
       MSG_BIN_NONE,
       MSG_BIN_EMPTY_COMMAND,
       MSG_BIN_UNKNOWN_COMMAND,
       MSG_BIN_ERROR_IN_COMMAND,
       MSG_BIN_OPTS,
       MSG_BIN_QUIT,
       MSG_BIN_EXIT,
       MSG_BIN_RESTART,
       MSG_BIN_DISCONNECT,
     } bin_type_t;



     typedef struct mas_ocontrol_list_s
     {
       struct mas_ocontrol_s *stqh_first;
       struct mas_ocontrol_s **stqh_last;
     } mas_ocontrol_list_head_t;
     typedef struct
     {
       struct mas_ocontrol_s *stqe_next;
     } mas_ocontrol_list_entry_t;


     typedef struct mas_ocontrol_s
     {


       unsigned long serial;

       pthread_t thread;
       pid_t tid;

       struct mas_channel_s *pchannel;
       mas_status_t status;
       struct timeval activity_time;
       mas_substatus_t substatus;
       unsigned subpoint;
       unsigned long subresult1;
       unsigned long subresult2;
     } mas_ocontrol_t;


     typedef struct mas_ucontrol_list_s
     {
       struct mas_ucontrol_s *stqh_first;
       struct mas_ucontrol_s **stqh_last;
     } mas_ucontrol_list_head_t;
     typedef struct
     {
       struct mas_ucontrol_s *stqe_next;
     } mas_ucontrol_list_entry_t;


     typedef struct mas_ucontrol_s
     {
       mas_ucontrol_list_entry_t next;
       struct mas_ocontrol_s h;
     } mas_ucontrol_t;


     typedef size_t( *mas_xvsnprintf_t ) ( char *str, size_t size, const char *format, va_list args );

#6 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_opts_types.h" 2





     typedef void ( *mas_opts_func_t ) ( const void *env, const char *section, const char *sectval, const char *name, const char *value );

     typedef union mas_msg_options_u
     {
       unsigned long word;
       struct mas_optflags_s
       {
         unsigned msg_notice:1;
         unsigned msg_watch:1;

         unsigned msg_main:1;

         unsigned msg_io:1;

         unsigned msg_listen:1;

         unsigned msg_transaction:1;

         unsigned msg_wait:1;
         unsigned msg_thread:1;

         unsigned msg_signal:1;

         unsigned msg_trace:1;
         unsigned msg_trace_main:1;
         unsigned msg_trace_listener:1;
         unsigned msg_trace_transaction:1;
         unsigned msg_cmd:1;
         unsigned msg_mem:1;
         unsigned msg_funline:1;
       } bit;
     } mas_msg_options_t;
     typedef struct mas_dirs_s
     {
       char *pids;
       char *mods;
       char *proto;
       char *log;
       char *history;
       char *post;
       char *config;
     } mas_dirs_t;
     typedef struct mas_thnames_s
     {
       char *parent_masterm;
       char *daemon_masterm;
       char *parent_masterx;
       char *daemon_masterx;
       char *parent_masterth;
       char *daemon_masterth;
       char *parent_masterthx;
       char *daemon_masterthx;
       char *parent_bunchm;
       char *daemon_bunchm;
       char *parent_bunchx;
       char *daemon_bunchx;
       char *parent_bunchi;
       char *daemon_bunchi;
       char *listenin;
       char *listenw;
       char *listent;
       char *listenm;
       char *listen_close;
       char *listen_exit;
     } mas_thnames_t;

     struct mas_options_s
     {
       mas_msg_options_t f;
       unsigned test:1;
       unsigned quit:1;
       unsigned info:1;
       unsigned single_instance:1;
       unsigned single_child:1;
       unsigned listener_single:1;
       unsigned transaction_single:1;
       unsigned nomessages_parent:1;
       unsigned nomessages_child:1;
       unsigned nomessages:1;

       unsigned nodaemon:1;
       unsigned noclose_std:1;
       unsigned noredirect_std:1;

       unsigned nologger:1;
       unsigned nolog:1;
       unsigned nowatcher:1;
       unsigned disconnect_prompt:1;
       unsigned wait_server:1;
       unsigned make_master_thread:1;
       unsigned noticker:1;
       unsigned read_user_opts:1;
       unsigned read_user_opts_plus:1;
       unsigned save_user_opts:1;
       unsigned save_user_opts_plus:1;
       unsigned overwrite_user_opts:1;
       unsigned overwrite_user_opts_plus:1;
       unsigned ticker_mode;
       unsigned nomaster;
       unsigned nolistener;
       unsigned nolisten;
       unsigned exitsleep;
       unsigned default_port;
       char env_optsname[256];
       char env_hostname[256];



       mas_string_setv_t argvv;



       mas_string_setv_t protosv;



       mas_string_setv_t hostsv;



       mas_string_setv_t commandsv;

       unsigned client_attempts;
       char *uuid;


       mas_dirs_t dir;
       mas_thnames_t thname;
       char *msgfilename;
       char *stderr_filename;
       char *stdout_filename;
       char *configfilename;
       char *saved_configfilename;
       unsigned max_config_backup;
       double restart_sleep;



     };
     typedef struct mas_options_s mas_options_t;


     typedef enum mas_optiov_value_type_e
     {
       MAS_OPT_TYPE_NONE,
       MAS_OPT_TYPE_INT,
       MAS_OPT_TYPE_DOUBLE,
       MAS_OPT_TYPE_STR,
       MAS_OPT_TYPE_PSTR,
       MAS_OPT_TYPE_FUNC,
     } mas_optiov_value_type_t;

     struct mas_option_parse_s
     {
       const char *section;
       const char *name;
       mas_optiov_value_type_t type;
       size_t offset;
       size_t size;
       mas_opts_func_t func;
       const char *dv;
     };
     typedef struct mas_option_parse_s mas_option_parse_t;
     typedef int ( *mas_init_fun_t ) ( mas_options_t * popts, const char **message );

#7 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_misc_def.h" 2
     extern mas_options_t gopts __attribute__ ( ( weak ) );

#90 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_def.h" 2
#18 "src/mas_listener.c" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/msg/mas_msg_tools.h" 1



     int mas_msg_set_file( const char *path, int force );





     int mas_msg( const char *func, int line, mas_msg_type_t msgt, int allow, int details, const char *prefix_fmt, const char *prefix,
                  const char *suffix, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 9, 10 ) ) ) __attribute__ ( ( weak ) );
     void *mas_fatal( void );
     int mas_error( const char *func, int line, int merrno, int *perrno, int *psaveerrno, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 6, 7 ) ) ) __attribute__ ( ( weak ) );
     void _mas_perr( const char *func, int line, int merrno, int *perrno ) __attribute__ ( ( weak ) );

#19 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/channel/mas_channel.h" 1



#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_channel_types.h" 1





#1 "/usr/include/sys/un.h" 1 3 4
#27 "/usr/include/sys/un.h" 3 4



     struct sockaddr_un
     {
       sa_family_t sun_family;
       char sun_path[108];
     };
#45 "/usr/include/sys/un.h" 3 4

#7 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_channel_types.h" 2


     typedef enum
     {
       CHN_NONE,
       CHN_RFILE,
       CHN_WFILE,
       CHN_SOCKET,
     } chn_type_t;

     typedef union
     {
       struct sockaddr_un path;
       struct sockaddr_in addr;
     } mas_serv_addr_t;

     typedef enum
     {
       MSG_PROTO_UNKNOWN,
       MSG_PROTO_MASSPC,
       MSG_PROTO_HTTP
     } mas_proto_t;

     struct mas_channel_buffer_s
     {
       size_t maxread;
       size_t size;
       ssize_t length;
       char *buffer;
       int fd_copy;
       ssize_t iptr;
       unsigned endfile:1;
     };
     typedef struct mas_channel_buffer_s mas_channel_buffer_t;

     struct mas_channel_s
     {

       chn_type_t type;
       int fd_socket;
       int fd_io;



       mas_serv_addr_t serv;
       mas_serv_addr_t serv_instance;
       mas_serv_addr_t cli;
       int port;
       char *host;
       char *error_func;
       int error_line;
       int error_errno;
       mas_proto_t proto;
       mas_channel_buffer_t buffer;
       unsigned is_server:1;
       unsigned listening:1;
       unsigned opened:1;
       unsigned cloned:1;
       unsigned error:1;
     };
     typedef struct mas_channel_s mas_channel_t;

#5 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/channel/mas_channel.h" 2

     int mas_channel_fd( const mas_channel_t * pchannel );





     mas_channel_t *mas_channel_clone( mas_channel_t * pchannel_src );


     int mas_channel_writef( const mas_channel_t * pchannel, const char *fmt, ... );
     int mas_channel_vwritef( const mas_channel_t * pchannel, const char *fmt, va_list args );

     int mas_channel_write_string( const mas_channel_t * pchannel, const char *cbuf, int z );

     int mas_channel_write( const mas_channel_t * pchannel, const char *cbuf, size_t sz );

     char *mas_channel_ip_string( mas_channel_t * pchannel );

     int mas_channel_read_some( mas_channel_t * pchannel );
     int mas_channel_read_some_new( mas_channel_t * pchannel );
     int mas_channel_read_remainder( mas_channel_t * pchannel );

#21 "src/mas_listener.c" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/channel/mas_channel_object.h" 1





     mas_channel_t *mas_channel_create( void );
     int mas_channel_reset( mas_channel_t * pchannel, int toclose, int todeaf );
     int mas_channel_delete( mas_channel_t * pchannel, int toclose, int todeaf );

     int mas_channel_init( mas_channel_t * pchannel, int is_server, chn_type_t type, const char *host, size_t hostlen, int port );
     int mas_channel_test( const mas_channel_t * pchannel );

#22 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/log/mas_log.h" 1




#1 "/usr/include/errno.h" 1 3 4
#6 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/log/mas_log.h" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/log/mas_log_types.h" 1




#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_th_types.h" 1



     typedef enum
     {
       MAS_THREAD_NONE,
       MAS_THREAD_MAIN,
       MAS_THREAD_MASTER,
       MAS_THREAD_LISTENER,
       MAS_THREAD_TRANSACTION,
       MAS_THREAD_TICKER,
       MAS_THREAD_WATCHER,
       MAS_THREAD_LOGGER,
     } th_type_t;
#6 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/log/mas_log_types.h" 2



#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_listener_control_types.h" 1



#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_varset_types.h" 1







#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_varvec_types.h" 1
#10 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_varvec_types.h"
     typedef struct iovec mas_varvec_element_t;
     typedef struct mas_varvec_s
     {
       char *name;
       unsigned long id;
       unsigned has_head:1;
       unsigned has_tail:1;
       unsigned el_cnt;
       mas_varvec_element_t *vec;
       mas_varvec_element_t *use_var;
     } mas_varvec_t;
#9 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_varset_types.h" 2

     typedef struct mas_varset_s
     {
       void *classes;
       unsigned long nclasses;
     } mas_varset_t;
#5 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_listener_control_types.h" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_autoset_types.h" 1
#9 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_autoset_types.h"
     typedef struct mas_autoset_s
     {
       void *instances;
       unsigned long ninstances;
       char *docroot;
     } mas_autoset_t;
#6 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_listener_control_types.h" 2
#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_transaction_control_types.h" 1
#21 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_transaction_control_types.h"
     typedef struct mas_rcontrol_s mas_rcontrol_t;
     typedef struct mas_transaction_protodesc_s mas_transaction_protodesc_t;
     typedef int ( *mas_transaction_fun_t ) ( mas_rcontrol_t * prcontrol, const void *pvoid );

     struct mas_transaction_protodesc_s
     {
       char *name;
       unsigned proto_id;
       mas_transaction_fun_t func;
       mas_varset_t *variables;
     };



     typedef struct mas_rcontrol_list_s
     {
       struct mas_rcontrol_s *stqh_first;
       struct mas_rcontrol_s **stqh_last;
     } mas_rcontrol_list_head_t;
     typedef struct
     {
       struct mas_rcontrol_s *stqe_next;
     } mas_rcontrol_list_entry_t;
#54 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_transaction_control_types.h"
     struct mas_rcontrol_s
     {
       char signature[2];
       mas_rcontrol_list_entry_t next;
       struct mas_ocontrol_s h;

       double start_time;
       struct mas_lcontrol_s *plcontrol;


       bin_type_t qbin;



       unsigned nc;
       unsigned xch_cnt;
       char *uuid;
       const mas_transaction_protodesc_t *proto_desc;
       pthread_cond_t waitchan_cond;
       pthread_mutex_t waitchan_mutex;
       unsigned complete:1;
       unsigned connection_keep_alive:1;



       unsigned waitchan_waiting:1;
       unsigned stop:1;
     };
#7 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_listener_control_types.h" 2
#17 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_listener_control_types.h"
     typedef struct mas_lcontrol_list_s
     {
       struct mas_lcontrol_s *stqh_first;
       struct mas_lcontrol_s **stqh_last;
     } mas_lcontrol_list_head_t;
     typedef struct
     {
       struct mas_lcontrol_s *stqe_next;
     } mas_lcontrol_list_entry_t;


     typedef struct mas_lcontrol_s
     {
       char signature[2];
       mas_lcontrol_list_entry_t next;
       struct mas_ocontrol_s h;

       mas_rcontrol_list_head_t *transaction_controls_list;

       const struct mas_options_s *popts;

       char *host;
       size_t hostlen;
       unsigned port;


       mas_autoset_t *autoset;
       pthread_mutex_t autoset_mutex;
       pthread_rwlock_t transaction_rwlock;
       pthread_rwlock_t variables_rwlock;
       unsigned long transaction_serial;
       unsigned long clients_came;
       unsigned long clients_gone;
       mas_varset_t *variables;
       unsigned in_list:1;
     } mas_lcontrol_t;
#10 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/log/mas_log_types.h" 2



     typedef struct mas_loginfo_list_s
     {
       struct mas_loginfo_s *stqh_first;
       struct mas_loginfo_s **stqh_last;
     } mas_loginfo_list_head_t;
     typedef struct
     {
       struct mas_loginfo_s *stqe_next;
     } mas_loginfo_list_entry_t;


     struct mas_loginfo_s
     {
       th_type_t thtype;
       pid_t pid;
       pid_t tid;

       pthread_t pth;



       const char *func;

       int line;
       char *message;
       mas_loginfo_list_entry_t next;
       const mas_channel_t *pchannel;
       const mas_lcontrol_t *plcontrol;
       const mas_rcontrol_t *prcontrol;
       unsigned long lserial;
       unsigned lstatus;
       unsigned long rserial;
       unsigned long serial;
       unsigned rstatus;
       double logtime;
       int lerrno;
       double delta_thread;
     };
     typedef struct mas_loginfo_s mas_loginfo_t;

#8 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/log/mas_log.h" 2




     int mas_log( const char *func, int line, int merrno, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 4, 5 ) ) ) __attribute__ ( ( weak ) );
#24 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/thtools/mas_thread_tools.h" 1




#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_thread_variables_types.h" 1
#9 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_thread_variables_types.h"
     struct mas_thdata_s
     {
       th_type_t type;
       mas_lcontrol_t *plcontrol;
       mas_rcontrol_t *prcontrol;
       double double_time;

       mas_varset_t *variables;
     };
     typedef struct mas_thdata_s mas_thdata_t;

     struct mas_thread_id_s
     {


       int l;

       int r;
       mas_status_t status;
     };
     typedef struct mas_thread_id_s mas_thread_id_t;

#6 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/thtools/mas_thread_tools.h" 2

     void mas_thread_make_key( void );
     extern pthread_once_t mas_thread_key_once;
     extern pthread_key_t mas_thread_key;






     mas_thdata_t *mas_in_thread( th_type_t thtype, mas_lcontrol_t * plcontrol, mas_rcontrol_t * prcontrol );
     void mas_in_thread_end( void );


     th_type_t mas_thself_type( void );
     const char *mas_thread_self_type_name( void );

     double mas_thself_double_time( void );
     void mas_thself_set_double_time( double double_time );

     mas_lcontrol_t *mas_thself_plcontrol( void );
     mas_rcontrol_t *mas_thself_prcontrol( void );
     mas_channel_t *mas_thself_pchannel( void );

     int mas_xxpthread_join( pthread_t thread, long nanos );
     int mas_xxspthread_join( pthread_t thread, long sec, long nanos );
     int mas_xxtspthread_join( pthread_t thread, struct timespec *jwait );



     const char *mas_thread_type_name( th_type_t typ );

#26 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/transaction/mas_transaction.h" 1
#26 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/transaction/mas_transaction.h"
     int mas_transaction_start( mas_lcontrol_t * plcontrol );

     int mas_transaction_cancel( mas_rcontrol_t * prcontrol );





     void mas_transaction_cleanup( void *arg );

     int mas_transaction_write( const mas_rcontrol_t * prcontrol, char *cbuf, size_t sz );
     int mas_transaction_vwritef( const mas_rcontrol_t * prcontrol, const char *fmt, va_list args );

#28 "src/mas_listener.c" 2

#1 "/home/mastar/.mas/develop/autotools/zoc/projects/commonlibs/zocromas_mas_listener/src/inc/mas_listener_wait_client.h" 1




     int mas_listener_wait_client( mas_lcontrol_t * plcontrol );

#30 "src/mas_listener.c" 2
#1 "/home/mastar/.mas/develop/autotools/zoc/projects/commonlibs/zocromas_mas_listener/src/inc/mas_lcontrol_object.h" 1







     mas_lcontrol_t *mas_lcontrol_create( void );


     mas_lcontrol_t *mas_lcontrol_make( const mas_options_t * popts, const char *host, unsigned port );
     int mas_lcontrol_init( mas_lcontrol_t * plcontrol, const mas_options_t * popts, const char *host, unsigned port );
     int mas_lcontrol_register( mas_lcontrol_t * plcontrol );
     int mas_lcontrol_remove_delete( mas_lcontrol_t * plcontrol );

#31 "src/mas_listener.c" 2
#1 "/home/mastar/.mas/develop/autotools/zoc/projects/commonlibs/zocromas_mas_listener/src/inc/mas_listener_control.h" 1






#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_autoobject_types.h" 1






#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_evaluate_types.h" 1







     typedef enum mas_cmdtype_e
     {
       MAS_CMD_PCHAR,
       MAS_CMD_CONST_PCHAR,
       MAS_CMD_FILE_LOAD,
       MAS_CMD_CONST_FNAME_LOAD,
     } mas_cmdtype_t;

     typedef struct mas_evaluated_s mas_evaluated_t;
     struct mas_evaluated_s
     {
       mas_cmdtype_t type;
       void *data;
     };

     typedef mas_evaluated_t *( *data_loader_t ) ( const mas_options_t * popts, const char *root, const char *tail, size_t size,
                                                   size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );


     typedef enum mas_content_type_e
     {
       MAS_CONTENT_BAD = -1,
       MAS_CONTENT_NONE = 0,
       MAS_CONTENT_HTML,
       MAS_CONTENT_TEXT,
       MAS_CONTENT_GIF,
       MAS_CONTENT_JPEG,
       MAS_CONTENT_FORM_DATA,
     } mas_content_type_t;


     struct content_type_details_s
     {
       mas_content_type_t ctype;
       char *mtype;
       char *dtype;
     };
     typedef struct content_type_details_s content_type_details_t;



     typedef struct content_type_ext_s content_type_ext_t;
     struct content_type_ext_s
     {
       mas_content_type_t ctype;
       char *extset;
     };
#8 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_autoobject_types.h" 2

     typedef enum mas_iaccess_type_e
     {
       MAS_IACCESS_BAD = -1,
       MAS_IACCESS_CHAR = 0,
       MAS_IACCESS_FCHAR,
       MAS_IACCESS_SENDFILE,
       MAS_IACCESS_SENDFILEL,
       MAS_IACCESS_SPLICE,
       MAS_IACCESS_FILE,
     } mas_iaccess_type_t;


     typedef struct mas_autoobject_s
     {
       mas_iaccess_type_t iaccess_type;
       unsigned long cat_cnt;
       unsigned long pass;
       unsigned long load_cnt;
       unsigned long reopen_cnt;
       unsigned long close_cnt;
       unsigned long stat_cnt;
       unsigned long lseek_cnt;
       unsigned long sendfile_cnt;
       unsigned long splice_cnt;
       char *docroot;
       char *name;
       unsigned long id;
       size_t size;
       union
       {
         FILE *f;
         void *v;
         unsigned long lu;
         int i;
       } handler;
       char *data;

       mas_content_type_t icontent_type;
       ino_t inode;
       mode_t mode;
       time_t time;
       char *gtime;
       char *etag;
     } mas_autoobject_t;
#8 "/home/mastar/.mas/develop/autotools/zoc/projects/commonlibs/zocromas_mas_listener/src/inc/mas_listener_control.h" 2

     unsigned mas_lcontrols_cleaning_transactions__( int removeit, long jwait );
     unsigned mas_lcontrol_cleaning_transactions__( mas_lcontrol_t * plcontrol, int removeit, long jwait );




     int mas_lcontrol_variable_create_text( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name, const char *txt );
     int mas_lcontrol_variable_set_text( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name, const char *txt );
     mas_varvec_element_t *mas_lcontrol_variables_find( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name );



     mas_autoobject_t *mas_lcontrol_get_autoobject( mas_lcontrol_t * plcontrol, const char *docroot, const char *uri );

#32 "src/mas_listener.c" 2

#1 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_control_types.h" 1
#19 "/mnt/new_misc/develop/autotools/zoc/admin/install/default/include/mastar/types/mas_control_types.h"
     typedef struct mas_pthread_globals_s mas_pthread_globals_t;
     struct mas_pthread_globals_s
     {
       pthread_mutex_t malloc_mutex;

       pthread_rwlock_t lcontrols_list_rwlock;
       pthread_rwlock_t modules_list_rwlock;
       pthread_mutex_t cleanup_transactions_mutex;
       pthread_mutex_t msg_mutex;
       pthread_mutex_t emsg_mutex;
       pthread_mutex_t mfp_mutex;
       pthread_mutex_t cnttr1_mutex;
       pthread_mutex_t cnttr2_mutex;
       pthread_mutex_t cnttr3_mutex;

       pthread_mutex_t logger_mutex;
       pthread_mutex_t logger_wait_mutex;
       pthread_cond_t logger_wait_cond;

       pthread_attr_t custom_attr;
       cpu_set_t *master_set;
       pthread_attr_t master_attr;
       pthread_attr_t ticker_attr;
       pthread_attr_t watcher_attr;
       pthread_attr_t logger_attr;
       cpu_set_t *listener_set;
       pthread_attr_t listener_attr;
       cpu_set_t *transaction_set;
       pthread_attr_t transaction_attr;
     };

     typedef struct mas_thread_info_s mas_thread_info_t;
     struct mas_thread_info_s
     {
       pid_t pid;
       pid_t tid;
       pthread_t thread;
     };
     typedef struct mas_std_threads_set_s
     {
       mas_thread_info_t ticker;
       mas_thread_info_t watcher;
       mas_thread_info_t logger;
       mas_thread_info_t master;
       mas_thread_info_t daemon;
       mas_thread_info_t main;
     } mas_std_threads_set_t;
     struct mas_control_s
     {
       union
       {
         mas_std_threads_set_t n;
         mas_thread_info_t a[sizeof( mas_std_threads_set_t ) / sizeof( mas_thread_info_t )];
       }
       threads;
       mas_thread_info_t *pserver_thread;

       char *binname;
       char *progname;
       char *pkgname;
       char *exepath;
       char *exename;
       mas_error_handler_fun_t error_handler;

       void **loaded_modules;
       unsigned loaded_modules_cnt;

       double start_time;
       double transactions_time;
       unsigned long watch_cnt;
       unsigned long tick_cnt;

       int old_stderr;
       FILE *old_stderrfile;
       FILE *stderrfile;
       int old_stdout;
       FILE *msgfile;

       unsigned int_cnt;
       unsigned term_cnt;
       unsigned try_cnt;
       unsigned restart_cnt;
       unsigned in_pipe;
       unsigned fatal:1;
       unsigned main_exit:1;
       unsigned dead_beaf:1;
       unsigned in_client:1;
       unsigned is_client:1;
       unsigned is_server:1;
       unsigned is_parent:1;
       unsigned is_child:1;


       unsigned messages:1;
       unsigned messages_set:1;
       unsigned messages_child:1;
       unsigned messages_parent:1;
       unsigned inited:1;

       unsigned daemon:1;
       unsigned close_std:1;
       unsigned redirect_std:1;


       unsigned keep_listening:1;
       unsigned keep_logging:1;
       unsigned stop_listeners:1;

       unsigned disconnect:1;
       unsigned restart:1;
       unsigned exit:1;
       unsigned do_exit:1;

       unsigned forget_transactions:1;
       unsigned opts_saved:1;
       unsigned opts_saved_plus:1;
       unsigned watcher_hide:1;
       unsigned watcher_stop:1;

       unsigned log:1;
       unsigned log_disabled:1;

       unsigned log_offmem:1;
       unsigned ticker_hide:1;
       unsigned ticker_mode;

       mas_pthread_globals_t thglob;

       unsigned long listener_serial;


       mas_lcontrol_list_head_t *lcontrols_list;




       mas_string_csetv_t launcherv;
       int argv_nonoptind;


       mas_string_csetv_t launchervv;
       mas_string_csetv_t launcherev;



       mas_string_setv_t loaded_optsv;

       mas_string_setv_t pidfilesv;

       size_t protos_num;
       mas_transaction_protodesc_t *proto_descs;

       mas_stamp_t stamp;

       mas_status_t status;
       unsigned long log_q_came;
       unsigned long log_q_gone;
       unsigned long log_q_mem;
       unsigned long clients_created;
       unsigned long clients_removed;
       unsigned long clients_came0;
       unsigned long clients_came;
       unsigned long clients_gone;
       unsigned long clients_gone2;
       unsigned long xch_cnt;
       unsigned long maxclients;
       double maxtrtime;
       unsigned long sigint_time;
       FILE *pidfile;
       int pidfd;
       char *logpath;
       FILE *logfile;



       mas_string_setv_t commandsv;

       pthread_key_t mas_thread_key;
       pthread_once_t mas_thread_key_once;
     };
     typedef struct mas_control_s mas_control_t;

#34 "src/mas_listener.c" 2



#1 "/home/mastar/.mas/develop/autotools/zoc/projects/commonlibs/zocromas_mas_listener/src/inc/mas_listener.h" 1



     int mas_listener( mas_lcontrol_t * plcontrol );
     void *mas_listener_th( void *arg );

     int mas_listener_start( const mas_options_t * popts, char *host, unsigned port );
     int mas_listener_cancel( mas_lcontrol_t * plcontrol );
     int mas_listener_wait( mas_lcontrol_t * plcontrol );

#38 "src/mas_listener.c" 2
#70 "src/mas_listener.c"
     int mas_listener_wait( mas_lcontrol_t * plcontrol )
{
  int r = -1;
  pthread_t pth;


  if ( plcontrol && plcontrol->h.thread )
  {
    {
      if ( &mas_log )
        mas_log( __func__, 79, ( *__errno_location(  ) ), "to wait for listener [%lx] to stop", plcontrol->h.thread );
    };
  }
  if ( plcontrol && ( pth = plcontrol->h.thread ) && 0 == mas_xpthread_join( pth ) )
  {
    {
      if ( &mas_log )
        mas_log( __func__, 83, ( *__errno_location(  ) ), "stopped listener [%lx]", pth );
    };
    plcontrol->h.thread = ( pthread_t ) 0;

    r = 0;
    {
      if ( &mas_msg )
        mas_msg( __func__, 87, MAS_MSG_WATCH, ( ( &gopts ? gopts.f.bit.msg_watch : 0 ) ), 0, "%-15s", "?" PACKAGE, "          ",
                 "LISTENER %lu STOPPED", plcontrol->h.serial );
    };
  }
  return r;
}

#112 "src/mas_listener.c"
static size_t listener_stacksize = 0;
static void *listener_stackaddr = ( ( void * ) 0 );





int
mas_listener_start( const mas_options_t * popts, char *host_port, unsigned port )
{
  extern mas_control_t ctrl __attribute__ ( ( weak ) );
  int r = 0;


  if ( host_port )
  {
    mas_lcontrol_t *plcontrol;

    plcontrol = mas_lcontrol_make( popts, host_port, port );
    if ( plcontrol )
    {
      {
        extern mas_control_t ctrl __attribute__ ( ( weak ) );

        ( void ) pthread_attr_getstack( &ctrl.thglob.listener_attr, &listener_stackaddr, &listener_stacksize );
        {
          if ( &mas_msg )
            mas_msg( __func__, 137, MAS_MSG_WARNING, ( ( &gopts ? gopts.f.bit.msg_listen : 0 ) ), 1, "<|   %-8s|>", "L", ( ( void * ) 0 ),
                     "cr. listener th. stack:%lu @ %p", ( unsigned long ) listener_stacksize, listener_stackaddr );
        };
      }
      if ( popts->listener_single )
      {

        mas_listener( plcontrol );
      }
      else
      {
        extern mas_control_t ctrl __attribute__ ( ( weak ) );

        {
          if ( &mas_log )
            mas_log( __func__, 147, ( *__errno_location(  ) ), "cr'ing ls. th; plc=%p #%lu", ( void * ) plcontrol, plcontrol->h.serial );
        };


        ( ( r = ( pthread_create( &plcontrol->h.thread, &ctrl.thglob.listener_attr, mas_listener_th, ( void * ) plcontrol ) ) ), r =
          ( r
            ? ( r =
                ( &ctrl
                  ? ( ctrl.error_handler ) ( __func__, 150, 0, -1, r, ( ( void * ) 0 ), ( ( void * ) 0 ), ( ( void * ) 0 ),
                                             ( ( void * ) 0 ) ) : -1 ) ) : r ) );
        if ( plcontrol->h.thread )
        {
          {
            if ( &mas_log )
              mas_log( __func__, 153, ( *__errno_location(  ) ), "cr'ed ls. th; plc=%p [%lx] #%lu", ( void * ) plcontrol,
                       plcontrol->h.thread, plcontrol->h.serial );
          };
        }
      }
      if ( r == 0 )
      {
        {
          if ( &mas_msg )
            mas_msg( __func__, 158, MAS_MSG_WARNING, ( ( &gopts ? gopts.f.bit.msg_listen : 0 ) ), 1, "<|   %-8s|>", "L", ( ( void * ) 0 ),
                     "<C l/th L%lu:%u for %s:%u", plcontrol->h.serial, plcontrol->h.status, plcontrol->host, plcontrol->port );
        };

      }
    }
  }
  else
  {
    {
      {
        if ( &mas_error )
          mas_error( __func__, 165, ( *__errno_location(  ) ), &( *__errno_location(  ) ), ( ( void * ) 0 ), "no host defined" );
      };
    };
    ( ( r =
        ( r >= 0 ? ( -1 ) : r ) ), ( r < 0 ? ( r =
                                               ( &ctrl
                                                 ? ( ctrl.error_handler ) ( __func__, 166, 0, r, ( *__errno_location(  ) ),
                                                                            &( *__errno_location(  ) ), ( ( void * ) 0 ), "-1",
                                                                            ( ( void * ) 0 ) ) : -1 ) ) : r ) );
  }
  return r;
}


int
mas_listener_cancel( mas_lcontrol_t * plcontrol )
{
  int r = -1;

#193 "src/mas_listener.c"
  {
    if ( &mas_msg )
      mas_msg( __func__, 193, MAS_MSG_WATCH, ( ( &gopts ? gopts.f.bit.msg_watch : 0 ) ), 0, "%-15s", "?" PACKAGE, "          ",
               "CANCEL L%lu:%u (plcontrol:%p) th:%lx", plcontrol->h.serial, plcontrol->h.status, ( void * ) plcontrol,
               plcontrol->h.thread );
  };
  {
    if ( &mas_log )
      mas_log( __func__, 194, ( *__errno_location(  ) ), "cancelling L%lu:%u", plcontrol->h.serial, plcontrol->h.status );
  };
  if ( plcontrol->h.thread )
  {
    mas_pthread_cancel( plcontrol->h.thread );
    r = 0;
  }
  return r;
}

static void
mas_listener_cleanup( void *arg )
{
  mas_lcontrol_t *plcontrol = ( ( void * ) 0 );

  if ( ( plcontrol = ( mas_lcontrol_t * ) arg ) )
  {
    {
      if ( &mas_log )
        mas_log( __func__, 210, ( *__errno_location(  ) ), "listener cleanup plc:%p", ( void * ) plcontrol );
    };
    plcontrol->h.status = MAS_STATUS_END;
    {
      if ( &mas_log )
        mas_log( __func__, 212, ( *__errno_location(  ) ), "listener cleanup : to delete channel" );
    };
    mas_channel_delete( plcontrol->h.pchannel, 0, 1 );
    plcontrol->h.pchannel = ( ( void * ) 0 );

    {
      if ( &mas_log )
        mas_log( __func__, 216, ( *__errno_location(  ) ), "listener cleanup : to end transactons" );
    };


    {
      if ( &mas_log )
        mas_log( __func__, 219, ( *__errno_location(  ) ), "listener cleanup : transactons closed" );
    };
  }
  else
  {
    {
      {
        if ( &mas_error )
          mas_error( __func__, 223, ( *__errno_location(  ) ), &( *__errno_location(  ) ), ( ( void * ) 0 ),
                     "listener cleanup - NO plcontrol" );
      };
    };
    {
      if ( &mas_log )
        mas_log( __func__, 224, ( *__errno_location(  ) ), "listener cleanup - NO plcontrol" );
    };
  }



  {
    if ( &mas_log )
      mas_log( __func__, 229, ( *__errno_location(  ) ), "listener cleanup; to delete plc:%p in list:%d", ( void * ) plcontrol,
               plcontrol->in_list );
  };

  mas_lcontrol_remove_delete( plcontrol );
}

int
mas_listener( mas_lcontrol_t * plcontrol )
{
  extern mas_control_t ctrl __attribute__ ( ( weak ) );
  extern mas_control_t ctrl __attribute__ ( ( weak ) );
  int r = -1, rn = 0;

  if ( plcontrol )
  {
    plcontrol->h.status = MAS_STATUS_INIT;

    {
      if ( &mas_log )
        mas_log( __func__, 245, ( *__errno_location(  ) ), "l/th loop" );
    };
    while ( !ctrl.fatal && 0 == ( r = mas_listener_wait_client( plcontrol ) ) )
    {
      {
        if ( &mas_log )
          mas_log( __func__, 248, ( *__errno_location(  ) ), "listener next client ..." );
      };
      {
        if ( &mas_msg )
          mas_msg( __func__, 249, MAS_MSG_NOTICE, ( ( &gopts ? gopts.f.bit.msg_notice : 0 ) ), 0, "%-15s", "?" PACKAGE, "          ",
                   "LISTENER next client ..." );
      };
    }
    if ( plcontrol->popts->thname.listen_close )
      ( ( rn =
          ( rn >= 0 ? ( prctl( 15, ( unsigned long ) plcontrol->popts->thname.listen_close ) ) : rn ) ), ( rn < 0 ? ( rn =
                                                                                                                      ( &ctrl
                                                                                                                        ? ( ctrl.
                                                                                                                            error_handler )
                                                                                                                        ( __func__, 252, 0,
                                                                                                                          rn,
                                                                                                                          ( *__errno_location
                                                                                                                            (  ) ),
                                                                                                                          &
                                                                                                                          ( *__errno_location
                                                                                                                            (  ) ),
                                                                                                                          ( ( void * ) 0 ),
                                                                                                                          "prctl( PR_SET_NAME, ( unsigned long ) plcontrol->popts->thname.listen_close )",
                                                                                                                          ( ( void * ) 0 ) )
                                                                                                                        : -1 ) ) : rn ) );
    plcontrol->h.status = MAS_STATUS_CLOSE;

    {
      if ( &mas_log )
        mas_log( __func__, 255, ( *__errno_location(  ) ), "exiting listening (stopped listening) r:%d", r );
    };

  }
  return r;
}


void *
mas_listener_th( void *tlcontrol )
{
  extern mas_control_t ctrl __attribute__ ( ( weak ) );
  int rn = 0;
  mas_lcontrol_t *plcontrol = ( ( void * ) 0 );

  plcontrol = ( mas_lcontrol_t * ) tlcontrol;
  if ( plcontrol->popts->thname.listenm )
    ( ( rn =
        ( rn >= 0 ? ( prctl( 15, ( unsigned long ) plcontrol->popts->thname.listenm ) ) : rn ) ), ( rn < 0 ? ( rn =
                                                                                                               ( &ctrl
                                                                                                                 ? ( ctrl.
                                                                                                                     error_handler )
                                                                                                                 ( __func__, 271, 0, rn,
                                                                                                                   ( *__errno_location
                                                                                                                     (  ) ),
                                                                                                                   &( *__errno_location
                                                                                                                      (  ) ),
                                                                                                                   ( ( void * ) 0 ),
                                                                                                                   "prctl( PR_SET_NAME, ( unsigned long ) plcontrol->popts->thname.listenm )",
                                                                                                                   ( ( void * ) 0 ) ) :
                                                                                                                 -1 ) ) : rn ) );
#281 "src/mas_listener.c"
  if ( plcontrol->popts->nolisten )
  {
    {
      if ( &mas_log )
        mas_log( __func__, 283, ( *__errno_location(  ) ), "listener th. -> sleep started %s:%u", plcontrol->host, plcontrol->port );
    };
    sleep( plcontrol->popts->nolisten );
  }
  else if ( plcontrol )
  {
    {
      if ( &mas_log )
        mas_log( __func__, 288, ( *__errno_location(  ) ), "listener th. started %s:%u", plcontrol->host, plcontrol->port );
    };
    mas_in_thread( MAS_THREAD_LISTENER, plcontrol, ( ( void * ) 0 ) );

    plcontrol->h.tid = mas_gettid(  );
    if ( !plcontrol->host )
    {
      {
        {
          if ( &mas_error )
            mas_error( __func__, 294, ( *__errno_location(  ) ), &( *__errno_location(  ) ), ( ( void * ) 0 ), "host not set" );
        };
      };
      {
        if ( &mas_log )
          mas_log( __func__, 295, ( *__errno_location(  ) ), "listener: host not set" );
      };
    }
    plcontrol->h.status = MAS_STATUS_START;
#307 "src/mas_listener.c"
    {



      {
        if ( &mas_log )
          mas_log( __func__, 311, ( *__errno_location(  ) ), "setting cleanup for L %s:%u", plcontrol->host, plcontrol->port );
      };
      do
      {
        __pthread_unwind_buf_t __cancel_buf;
        void ( *__cancel_routine ) ( void * ) = ( mas_listener_cleanup );
        void *__cancel_arg = ( plcontrol );
        int __not_first_call = __sigsetjmp( ( struct __jmp_buf_tag * ) ( void * ) __cancel_buf.__cancel_jmp_buf, 0 );

        if ( __builtin_expect( __not_first_call, 0 ) )
        {
          __cancel_routine( __cancel_arg );
          __pthread_unwind_next( &__cancel_buf );
        }
        __pthread_register_cancel( &__cancel_buf );
        do
        {;




          ( void ) mas_listener( plcontrol );

          do
          {
          }
          while ( 0 );
        }
        while ( 0 );
        __pthread_unregister_cancel( &__cancel_buf );
        if ( 1 )
          __cancel_routine( __cancel_arg );
      }
      while ( 0 );
    }
  }

  {
    if ( &mas_log )
      mas_log( __func__, 323, ( *__errno_location(  ) ), "listener th. ending" );
  };



  {
    {
      if ( &mas_error )
        mas_error( __func__, 327, ( *__errno_location(  ) ), &( *__errno_location(  ) ), ( ( void * ) 0 ), "LISTENER STOP" );
    };
  };
  if ( plcontrol->popts->thname.listen_exit )
    ( ( rn =
        ( rn >= 0 ? ( prctl( 15, ( unsigned long ) plcontrol->popts->thname.listen_exit ) ) : rn ) ), ( rn < 0 ? ( rn =
                                                                                                                   ( &ctrl
                                                                                                                     ? ( ctrl.
                                                                                                                         error_handler )
                                                                                                                     ( __func__, 329, 0, rn,
                                                                                                                       ( *__errno_location
                                                                                                                         (  ) ),
                                                                                                                       &( *__errno_location
                                                                                                                          (  ) ),
                                                                                                                       ( ( void * ) 0 ),
                                                                                                                       "prctl( PR_SET_NAME, ( unsigned long ) plcontrol->popts->thname.listen_exit )",
                                                                                                                       ( ( void * ) 0 ) ) :
                                                                                                                     -1 ) ) : rn ) );
  mas_pthread_exit( ( ( void * ) 0 ) );
  return ( ( void * ) 0 );
}
