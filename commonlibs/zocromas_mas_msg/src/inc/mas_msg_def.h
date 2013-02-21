#ifndef MAS_MSG_DEF_H
#  define MAS_MSG_DEF_H

#  include <errno.h>

/* #  define MAS_SEPARATOR_MARKER ":" */
#  define MAS_SEPARATION_LINE "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"

#  define MAS_FIFO_DIR "/tmp"
#  define MAS_FIFO_PREF "masfifo_"
#  define MAS_FIFO_SUFF ".tmp"

#  define MAS_TO_SERVER "mas_to_server"
#  define MAS_TO_CLIENT "mas_to_client"

/* #  define PTR(typ,p) (p?p:(typ)mas_fatal()) */


#  define MAS_MSGFL(...) { if (&mas_msg) { mas_msg(FL, __VA_ARGS__); } }

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
#  ifndef MAS_NOMSG
#    define MSG(...) MAS_MSGFL( (( opts.f.bit.msg_c && !ctrl.is_server ) \
  || ( opts.f.bit.msg_s && ctrl.is_server )),                               0,1,          31,"      %5s     " ,"@@",        NULL, __VA_ARGS__)
#    define tMSG(...)   MAS_MSGFL(  ( opts.f.bit.msg_tr ),                  1,1,          31,"<|    %5s   |>", "trace",     NULL, __VA_ARGS__)
#    define thMSG(...)  MAS_MSGFL(  ( opts.f.bit.msg_th ),                  0,1,          96,"<|    %5s   |>", "TH",        NULL, __VA_ARGS__)
#    define sigMSG(...) MAS_MSGFL(  ( opts.f.bit.msg_sg ),                  0,1,          96,"<|    %5s   |>", "SG",        NULL, __VA_ARGS__)
// #    define memMSG(...) MAS_MSGFL( ( opts.f.bit.msg_mem ),               0,1,          96,"<|    %5s   |>", "MEM",       NULL, __VA_ARGS__)
#    define memMSG(...) if (opts.f.bit.msg_mem){MFP("%s:%u:", FL);MFP(__VA_ARGS__);MFP("\n");}
#    define ioMSG(...)  MAS_MSGFL(  ( opts.f.bit.msg_io ),                  0,1,          96,"<|    %5s   |>", "IO",        NULL, __VA_ARGS__)
#    define mMSG(...)   MAS_MSGFL(  ( opts.f.bit.msg_m ),		    0,1,          96,"<|    %5s   |>", "M",         NULL, __VA_ARGS__)
#    define rMSG(...)   MAS_MSGFL(  ( opts.f.bit.msg_r && ctrl.is_server ), 0,1,          96,"<|    %5s   |>", "R",         NULL, __VA_ARGS__)
#    define lMSG(...)   MAS_MSGFL(  ( opts.f.bit.msg_l && ctrl.is_server ), 0,1,          96,"<|    %5s   |>", "L",         NULL, __VA_ARGS__)
#    define wMSG(...)   MAS_MSGFL(  ( opts.f.bit.msg_w && ctrl.is_server ), 0,1,          96,"<|    %5s   |>", "WAIT",      NULL, __VA_ARGS__)
#    define GMSG(...)   MAS_MSGFL(	 1,			            0,0,          96,"      %5s     ", "G",         NULL, __VA_ARGS__)
#    define cMSG(...)   MAS_MSGFL( ( opts.f.bit.msg_cmd ),	            0,1,          96,"<|    %5s   |>", "CMD",       NULL, __VA_ARGS__)
/* #  define EMSG(...) MAS_MSGFL(  1,                                       0,1,          31,"<  %4s   >" , "E.R.R.O.R", NULL, __VA_ARGS__) */
/* #  define EMSGfl(f,l,...) MAS_MSGFL(l,  1,                               0,1,          31,"<  %4s   >" , "E.R.R.O.R", NULL, __VA_ARGS__) */
#    define IMSG(...) MAS_MSGFL(  1,				            0,0,          0,NULL          , "[*] "   , NULL,              __VA_ARGS__)
#    define GDMSG(...) MAS_MSGFL( 1,				            0,1,          0,NULL          , NULL     , NULL,              __VA_ARGS__)
#    define HMSG(...) MAS_MSGFL(  1,				            0,0,          0,"%-15s", "?"PACKAGE,"                ", __VA_ARGS__)
#  else
#    define MSG(...)
#    define tMSG(...)
#    define thMSG(...)
#    define sigMSG(...)
#    define memMSG(...)
#    define ioMSG(...)
#    define mMSG(...)
#    define rMSG(...)
#    define lMSG(...)
#    define wMSG(...)
#    define GMSG(...)
#    define cMSG(...)
#    define IMSG(...)
#    define GDMSG(...)
#    define HMSG(...)
#  endif
// #  define FMSG(...) MAS_MSGFL(  777,         1,1,NULL,     0,NULL, "           ","             ", __VA_ARGS__)
#  ifndef MAS_NO_THREADS
#    define FMSG(...) {MFP("%s:%u:%lx:", FL, mas_pthread_self());MFP(__VA_ARGS__);MFP("\n");}
#  else
#    define FMSG(...) {MFP("%s:%u:%lx:", FL, 0L);MFP(__VA_ARGS__);MFP("\n");}
#  endif
#  define EMSG(...) { mas_error(FL, errno, __VA_ARGS__); }
#  define EHMSG(cnd, ...) { if(cnd){EMSG(__VA_ARGS__);}else{HMSG(__VA_ARGS__);} }
#  define HEMSG(...) EHMSG(__VA_ARGS__)
#  define EMSG_ONCE(...) { static int shown=0; if (!shown) { mas_error(FL, errno, __VA_ARGS__); shown=1; } }
#  define EMSGfl(f,l,...) { mas_error(f,l, errno, __VA_ARGS__); }
#  define EEMSG(...) {    EMSG(__VA_ARGS__) ; }
#  define FEMSG(...) { ctrl.fatal=1 ; mas_error(FL, errno, __VA_ARGS__); }
#  define P_ERR { mas_perr(FL); }
#  define RP_ERR(arg) { r = arg;if ( r < 0 ) { P_ERR; } }
/* #  include "mas_msg_tools.h" */
#endif
