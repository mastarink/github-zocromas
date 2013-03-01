#ifndef MAS_MSG_DEF_H
#  define MAS_MSG_DEF_H

#  include <errno.h>

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
  MAS_MSG_FIELD_PIDNAME,
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

#  define MAS_FIFO_DIR "/tmp"
#  define MAS_FIFO_PREF "masfifo_"
#  define MAS_FIFO_SUFF ".tmp"

#  define MAS_TO_SERVER "mas_to_server"
#  define MAS_TO_CLIENT "mas_to_client"

/* #  define PTR(typ,p) (p?p:(typ)mas_fatal()) */


#  ifndef MAS_NOMSG
#    define MAS_MSGFL(...) {  mas_msg(FL, __VA_ARGS__); }
#    define MAS_ERRFL(...) {  mas_error(FL, __VA_ARGS__); }
#  else
#    define MAS_MSGFL(...)
#  endif
#  define MFPZ(...) { \
    if ( ctrl.msgfile ) \
      fprintf( ctrl.msgfile, __VA_ARGS__ ); \
    else if ( ctrl.stderrfile ) \
      fprintf( ctrl.stderrfile, __VA_ARGS__ ); \
   }
#  ifdef MAS_USE_CURSES
#    define MFP(...) { if (use_curses) \
     { mas_pthread_mutex_lock( &ctrl.mfp_mutex ); wprintw( w_win, __VA_ARGS__ );wrefresh(w_win); mas_pthread_mutex_unlock( &ctrl.mfp_mutex );} \
     else \
       MFPZ(__VA_ARGS__) \
  }
#  else
#    define MFP(...) MFPZ(__VA_ARGS__)
#  endif
#  define memMSG(...) if (opts.f.bit.msg_mem){MFP("%s:%u:", FL);MFP(__VA_ARGS__);MFP("\n");}
// *INDENT-OFF*
// #    define memMSG(...) MAS_MSGFL( ( opts.f.bit.msg_mem ),               1, "<|    %5s   |>", "MEM",       NULL, __VA_ARGS__)
//						allow				details		pref_fmt	pref	suff	fmt,...
#define MSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,  ( opts.f.bit.msg_notice	),	1, "%-15s"      , "?"PACKAGE,  NULL,		__VA_ARGS__)
#define HMSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,  ( opts.f.bit.msg_notice	),	0, "%-15s"      , "?"PACKAGE,  "          ",	__VA_ARGS__)
#define FMSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,   1,					0, "%-15s"      , "?"PACKAGE,  "          ",	__VA_ARGS__)
#define WMSG(...)	MAS_MSGFL( MAS_MSG_WATCH,   ( opts.f.bit.msg_watch	),	0, "%-15s"      , "?"PACKAGE,  "          ",	__VA_ARGS__)
#define tMSG(...)	MAS_MSGFL( MAS_MSG_DEBUG,   ( opts.f.bit.msg_trace	),	1, "<|   %-8s|>", "TRACE",	NULL,		__VA_ARGS__)
#define thMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_thread	),	1, "<|   %-8s|>", "TH",		NULL,		__VA_ARGS__)
#define sigMSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,  ( opts.f.bit.msg_signal	),	1, "<|   %-8s|>", "SG",		NULL,		__VA_ARGS__)
#define ioMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_io		),	1, "<|   %-8s|>", "IO",		NULL,		__VA_ARGS__)
#define mMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_main	),	1, "<|   %-8s|>", "M",		NULL,		__VA_ARGS__)
#define rMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_transaction),	1, "<|   %-8s|>", "R",		NULL,		__VA_ARGS__)
#define lMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_listen	),	1, "<|   %-8s|>", "L",		NULL,		__VA_ARGS__)
#define wMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_wait	),	1, "<|   %-8s|>", "WAIT",	NULL,		__VA_ARGS__)
#define GMSG(...)	MAS_MSGFL( MAS_MSG_WARNING,  1,					0, "     %-8s  ", "G",		NULL,		__VA_ARGS__)
#define cMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( opts.f.bit.msg_cmd	),	1, "<|   %-8s|>", "CMD",	NULL,		__VA_ARGS__)
#define IMSG(...)	MAS_MSGFL( MAS_MSG_TITLE,    1,					0, NULL         , "[*] ",	NULL,		__VA_ARGS__)
#define OMSG(...)	MAS_MSGFL( MAS_MSG_SUBTITLE, 1,					0, NULL         , NULL,		NULL,		__VA_ARGS__)
#define GDMSG(...)	MAS_MSGFL( MAS_MSG_WARNING,  1,					1, NULL         , NULL,		NULL,		__VA_ARGS__)
// *INDENT-ON*

#  define EMSG(...) { MAS_ERRFL( errno, __VA_ARGS__); }
/* #  define EHMSG(cnd, ...) { if(cnd){EMSG(__VA_ARGS__);}else{HMSG(__VA_ARGS__);} } */
/* #  define HEMSG(...) EHMSG(__VA_ARGS__) */
#  define EMSG_ONCE(...) { static int shown=0; if (!shown) { MAS_ERRFL( errno, __VA_ARGS__); shown=1; } }
#  define EEMSG(...) {    EMSG(__VA_ARGS__) ; }
#  define FEMSG(...) { ctrl.fatal=1 ; MAS_ERRFL( errno, __VA_ARGS__); }
#  define P_ERR { mas_perr(FL); }
#  define RP_ERR(arg) { r = arg;if ( r < 0 ) { P_ERR; } }
/* #  include "mas_msg_tools.h" */
#endif
