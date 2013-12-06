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
  MAS_MSG_FIELD_NONE,
  MAS_MSG_FIELD_STAGE,
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

/* #  define MAS_FIFO_DIR "/tmp"           */
/* #  define MAS_FIFO_PREF "masfifo_"      */
/* #  define MAS_FIFO_SUFF ".tmp"          */
/*                                         */
/* #  define MAS_TO_SERVER "mas_to_server" */
/* #  define MAS_TO_CLIENT "mas_to_client" */

/* #  define PTR(typ,p) (p?p:(typ)mas_fatal()) */


#  include "mas_msg_msgfl_def.h"
#  include "mas_msg_mfp_def.h"
#  include "mas_msg_misc_def.h"

#  define EMSG(...) { MAS_ERRFL( errno, &errno, NULL, __VA_ARGS__); }
#  define EMSGE(psavederrno, ...) { MAS_ERRFL( errno, &errno, psavederrno, __VA_ARGS__); }
/* #  define EHMSG(cnd, ...) { if(cnd){EMSG(__VA_ARGS__);}else{HMSG(__VA_ARGS__);} } */
/* #  define HEMSG(...) EHMSG(__VA_ARGS__) */
#  define EMSG_ONCE(...) { static int shown=0; if (!shown) { MAS_ERRFL( errno, &errno, NULL, __VA_ARGS__); shown=1; } }
#  define EEMSG(...) {    EMSG(__VA_ARGS__) ; }
/* #  define FEMSG(...) { ctrl.fatal=1 ; MAS_ERRFL( errno, &errno, NULL, __VA_ARGS__); } */
#  define P_ERR { extern int errno; if (&_mas_perr) _mas_perr(FL, errno, &errno); }
#  define RP_ERR(arg) { r = arg;if ( r < 0 ) { P_ERR; } }


/* #  include "mas_msg_tools.h" */
#endif
