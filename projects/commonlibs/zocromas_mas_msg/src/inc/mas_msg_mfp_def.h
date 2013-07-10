#ifndef MAS_MSG_MFP_DEF_H
#  define MAS_MSG_MFP_DEF_H


#ifndef MAS_NO_CTRL

#  define MFPL(s) { \
    if ( ctrl.msgfile ) \
      fputs( s, ctrl.msgfile ); \
    else if ( ctrl.stderrfile ) \
      fputs( s, ctrl.stderrfile ); \
   }

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

#else


#  define MFPZ(...) { fprintf( stderr, __VA_ARGS__ ); }


#  ifdef MAS_USE_CURSES
#    define MFP(...) { if (use_curses) \
     {  wprintw( w_win, __VA_ARGS__ );wrefresh(w_win); } \
     else \
       MFPZ(__VA_ARGS__) \
  }
#  else
#    define MFP(...) MFPZ(__VA_ARGS__)
#  endif

#endif

#endif
