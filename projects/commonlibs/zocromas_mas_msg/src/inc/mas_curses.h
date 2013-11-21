#ifndef MAS_CURSES_H
#  define MAS_CURSES_H

#ifdef MAS_USE_CURSES
#  include <curses.h>

extern int use_curses;

/* WINDOW *w_rootwin = NULL, *w_main = NULL, *wmain10 = NULL, *wmain11 = NULL; */
extern WINDOW *w_rootwin, *w_main0, *w_main, *w_listen0, *w_listen, *w_trans0, *w_trans, *w_other0, *w_other;


int mas_init_curses( void );
int mas_close_curses( void );
#endif

/* #include <mastar/types/mas_common_cdefs.h> */
#endif
