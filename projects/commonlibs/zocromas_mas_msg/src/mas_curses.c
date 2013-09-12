#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef MAS_USE_CURSES
#  include <curses.h>
#endif

#include <mastar/tools/mas_tools.h>


#include "mas_curses.h"

#ifdef MAS_USE_CURSES
static SCREEN *screen;
#endif

int
mas_init_curses( void )
{
  int r = 0;

#ifdef MAS_USE_CURSES

//if ( ( w_rootwin = initscr(  ) ) == NULL )
//{
//  fprintf( stderr, "Error initialising ncurses.\n" );
//  r = -__LINE__;
//} else
  if ( use_curses )
  {
    r = -__LINE__;
  }
  else if ( ( screen = newterm( NULL, stderr, stdin ) ) == NULL )
  {
    r = -__LINE__;
  }
  else if ( set_term( screen ) == NULL )
  {
    r = -__LINE__;
  }
  else
  {
    use_curses = 1;
    w_rootwin = newwin( 0, 0, 0, 0 );
    if ( !w_rootwin )
    {
      r = -__LINE__;
    }
    if ( r >= 0 )
      start_color(  );          /*  Initialize colours  */
    /* mvaddstr( 6, 32, " Hello, world! " ); */
    if ( r >= 0 && has_colors(  ) && COLOR_PAIRS >= 4 )
    {
      init_pair( 1, COLOR_WHITE, COLOR_BLACK );
      init_pair( 2, COLOR_RED, COLOR_BLACK );
      init_pair( 3, COLOR_GREEN, COLOR_BLACK );
      init_pair( 4, COLOR_YELLOW, COLOR_BLACK );
      init_pair( 5, COLOR_BLUE, COLOR_BLACK );
    }
    /* wborder( w_rootwin, 0, 0, 0, 0, 0, 0, 0, 0 ); */
    if ( r >= 0 )
    {
      int w_main_height, w_main_width, w_main_line, w_main_x, w_listen_height, w_listen_width, w_listen_line, w_listen_x,
            w_other_height, w_other_width, w_other_line, w_other_x, w_trans_height, w_trans_width, w_trans_line, w_trans_x;

      w_main_height = LINES / 3;
      w_main_width = COLS / 2;
      w_main_line = 0;
      w_main_x = 0;
      if ( w_main_line + w_main_height > LINES )
        r = -__LINE__;

      w_listen_height = LINES / 3;
      w_listen_width = COLS / 2;
      w_listen_line = w_main_line + w_main_height;
      w_listen_x = 0;
      if ( w_listen_line + w_listen_height > LINES )
        r = -__LINE__;

      w_other_height = LINES / 3;
      w_other_width = COLS / 2;
      w_other_line = w_listen_line + w_listen_height;
      w_other_x = 0;
      if ( w_other_line + w_other_height > LINES )
        r = -__LINE__;

      w_trans_height = LINES;
      w_trans_width = COLS / 2;
      w_trans_x = COLS / 2;
      w_trans_line = 0;

      if ( w_trans_line + w_trans_height > LINES )
        r = -__LINE__;
      /* r = -( 10000 + w_trans_line + w_trans_height ); */

      if ( r >= 0 )
        refresh(  );

      if ( r >= 0 )
      {
        w_main0 = subwin( w_rootwin, w_main_height, w_main_width, w_main_line, w_main_x );
        if ( !w_main0 )
          r = -__LINE__;
      }
      if ( r >= 0 )
      {
        w_listen0 = subwin( w_rootwin, w_listen_height, w_listen_width, w_listen_line, w_listen_x );
        if ( !w_listen0 )
          r = -__LINE__;
      }

      if ( r >= 0 )
      {
        w_other0 = subwin( w_rootwin, w_other_height, w_other_width, w_other_line, w_other_x );
        if ( !w_other0 )
          r = -__LINE__;
      }

      if ( r >= 0 )
      {
        w_trans0 = subwin( w_rootwin, w_trans_height, w_trans_width, w_trans_line, w_trans_x );
        if ( !w_trans0 )
          r = -__LINE__;
      }

      if ( r >= 0 )
      {
        if ( !( w_main0 && w_listen0 && w_trans0 ) )
          r = -__LINE__;
      }

      if ( r >= 0 && w_main0 )
      {
        w_main = subwin( w_main0, w_main_height - 2, w_main_width - 2, w_main_line + 1, w_main_x + 1 );
        wborder( w_main0, 0, 0, 0, 0, 0, 0, 0, 0 );
        wrefresh( w_main0 );
        wrefresh( w_main );
        scrollok( w_main, 1 );
      }
      if ( r >= 0 && w_listen0 )
      {
        w_listen = subwin( w_listen0, w_listen_height - 2, w_listen_width - 2, w_listen_line + 1, w_listen_x + 1 );
        wborder( w_listen0, 0, 0, 0, 0, 0, 0, 0, 0 );
        wrefresh( w_listen0 );
        wrefresh( w_listen );
        scrollok( w_listen, 1 );
      }
      if ( r >= 0 && w_other0 )
      {
        w_other = subwin( w_other0, w_other_height - 2, w_other_width - 2, w_other_line + 1, w_other_x + 1 );
        wborder( w_other0, 0, 0, 0, 0, 0, 0, 0, 0 );
        wrefresh( w_other0 );
        wrefresh( w_other );
        scrollok( w_other, 1 );
      }
      if ( r >= 0 && w_trans0 )
      {
        w_trans = subwin( w_trans0, w_trans_height - 2, w_trans_width - 2, w_trans_line + 1, w_trans_x + 1 );
        wborder( w_trans0, 0, 0, 0, 0, 0, 0, 0, 0 );
        wrefresh( w_trans0 );
        wrefresh( w_trans );
        scrollok( w_trans, 1 );
      }
    }
    use_curses = 1;
    /* HMSG( "@@@@@@@@@@@@@@@@@@@@@@@@@@@" ); */
  }
  if ( r < 0 && use_curses )
  {
    mas_close_curses(  );
    fprintf( stderr, "ERROR curses %d\n", r );
    /* exit( 33 ); */
  }
#endif
  return r;
}

int
mas_close_curses( void )
{
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    use_curses = 0;
    if ( w_trans )
      delwin( w_trans );
    w_trans = NULL;
    if ( w_trans0 )
      delwin( w_trans0 );
    w_trans0 = NULL;
    if ( w_listen )
      delwin( w_listen );
    w_listen = NULL;
    if ( w_listen0 )
      delwin( w_listen0 );
    w_listen0 = NULL;
    if ( w_main )
      delwin( w_main );
    w_main = NULL;
    if ( w_main0 )
      delwin( w_main0 );
    w_main0 = NULL;
    refresh(  );
    /* sleep( 3 ); */
    if ( w_rootwin )
      delwin( w_rootwin );
    endwin(  );
    delscreen( screen );
    refresh(  );
  }
#endif
  return 0;
}

__attribute__ ( ( constructor( 10001 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "-------------------- CONSTRUCTOr %s\n", __FILE__ ); */
  mas_common_constructor( IL, 0 );
}
