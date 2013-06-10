#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>


#ifndef MAS_NO_CTRL
#  include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;
#endif

#ifndef MAS_NO_THTOOLS
#  include <mastar/thtools/mas_thread_tools.h>
#endif

#include "mas_msg_def.h"
#ifdef MAS_USE_CURSES
#  include <curses.h>
#  include "mas_curses.h"
#endif

#include "mas_msg_tools.h"

#define MAS_DROP_PREFIX "?zocromas_"

/*
this:
  mas_msg_tools.c
related:
  mas_thread_tools.c
  mas_tools_def.h
  mas_common_types.h
more:
  mas_control.c
  mas_opts.c
  mas_log.c

*/

int fg_palette[] = { 30, 31, 32, 33, 34, 35, 36, 37, 90, 91, 92, 93, 94, 95, 96, 97 };
int bg_palette[] = { 40, 41, 42, 43, 44, 45, 46, 47, 100, 101, 102, 103, 104, 105, 106, 107 };

typedef struct mas_msg_field_s
{
  unsigned hide_with_details:1;
  unsigned show_no_details:1;
  unsigned bold:1;
  unsigned fg;
  unsigned bg;
} mas_msg_field_t;
typedef struct msg_options_s
{
  mas_msg_field_t def_field;
  mas_msg_field_t elapsed;
  mas_msg_field_t consume;
  mas_msg_field_t errcode;
  mas_msg_field_t prefix;
  mas_msg_field_t message;
  mas_msg_field_t codepos;
  mas_msg_field_t suffix;
  mas_msg_field_t pid;
  mas_msg_field_t pidname;
  mas_msg_field_t thread_info;
  mas_msg_field_t thread_type_name;
  mas_msg_field_t thread_type_name_main;
  mas_msg_field_t thread_type_name_master;
  mas_msg_field_t thread_type_name_listener;
  mas_msg_field_t thread_type_name_transaction;
  mas_msg_field_t thread_type_name_ticker;
  mas_msg_field_t thread_type_name_watcher;
  mas_msg_field_t thread_type_name_logger;

  mas_msg_field_t thread_status;
  mas_msg_field_t eol;
} msg_options_t;

/* gvim :  /\<def_field\>.*fg\s*=\s*[A-Z_]\+_COLOR_/e+1 */

static msg_options_t msg_options[MAS_MSG_MAX] = {
  /* MAS_MSG_NONE,    */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_TITLE,    */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_CYAN,.fg = MAS_MSG_COLOR_BLUE,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_CYAN,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_SUBTITLE,    */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_CYAN,.fg = MAS_MSG_COLOR_BLUE,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 1,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_CYAN,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_INFO,    */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_NOTICE,  */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 0,.hide_with_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_WATCH,   */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 0},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_WARNING, */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.hide_with_details = 1},
   .errcode = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.hide_with_details = 1},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_REDP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .codepos = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 0,.hide_with_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_TRAP,    */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_DEBUG,   */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_ERR,     */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.hide_with_details = 1},
   .errcode = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_REDP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .codepos = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 0,.hide_with_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_CRIT,    */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_ALERT,   */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
  /* MAS_MSG_EMERG,   */
  {
   .elapsed = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .consume = {.bold = 1,.bg = MAS_MSG_COLOR_BLUEP,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .prefix = {.bold = 1,.bg = MAS_MSG_COLOR_YELLOW,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .message = {.bold = 0,.bg = MAS_MSG_COLOR_BLACK,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .suffix = {.bold = 1,.bg = MAS_MSG_COLOR_GREEN,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .pid = {.bold = 1,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .pidname = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 1},
   .thread_info = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .thread_type_name = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_main = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_master = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_listener = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_transaction = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_ticker = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_watcher = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_type_name_logger = {.bold = 0,.bg = MAS_MSG_COLOR_GREENP,.fg = MAS_MSG_COLOR_BLACK,.show_no_details = 0},
   .thread_status = {.bold = 1,.bg = MAS_MSG_COLOR_BLUE,.fg = MAS_MSG_COLOR_YELLOW,.show_no_details = 0},
   .eol = {.bold = 0,.bg = MAS_MSG_COLOR_RED,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   .def_field = {.bold = 1,.bg = MAS_MSG_COLOR_MAGENTA,.fg = MAS_MSG_COLOR_WHITEP,.show_no_details = 1},
   },
};

unsigned
mas_bg_color( mas_msg_type_t msgt, mas_msg_field_type_t field )
{
  int index = 0;

  switch ( field )
  {
  case MAS_MSG_FIELD_ELAPSED:
    index = msg_options[msgt].elapsed.bg;
    break;
  case MAS_MSG_FIELD_CONSUME:
    index = msg_options[msgt].consume.bg;
    break;
  case MAS_MSG_FIELD_ERRCODE:
    index = msg_options[msgt].errcode.bg;
    break;
  case MAS_MSG_FIELD_PREFIX:
    index = msg_options[msgt].prefix.bg;
    break;
  case MAS_MSG_FIELD_MESSAGE:
    index = msg_options[msgt].message.bg;
    break;
  case MAS_MSG_FIELD_CODEPOS:
    index = msg_options[msgt].codepos.bg;
    break;
  case MAS_MSG_FIELD_SUFFIX:
    index = msg_options[msgt].suffix.bg;
    break;
  case MAS_MSG_FIELD_PID:
    index = msg_options[msgt].pid.bg;
    break;
  case MAS_MSG_FIELD_PIDNAME:
    index = msg_options[msgt].pidname.bg;
    break;
  case MAS_MSG_FIELD_THREAD_INFO:
    index = msg_options[msgt].thread_info.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME:
    index = msg_options[msgt].thread_type_name.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MAIN:
    index = msg_options[msgt].thread_type_name_main.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MASTER:
    index = msg_options[msgt].thread_type_name_master.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LISTENER:
    index = msg_options[msgt].thread_type_name_listener.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TRANSACTION:
    index = msg_options[msgt].thread_type_name_transaction.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TICKER:
    index = msg_options[msgt].thread_type_name_ticker.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_WATCHER:
    index = msg_options[msgt].thread_type_name_watcher.bg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LOGGER:
    index = msg_options[msgt].thread_type_name_logger.bg;
    break;
  case MAS_MSG_FIELD_THREAD_STATUS:
    index = msg_options[msgt].thread_status.bg;
    break;
  case MAS_MSG_FIELD_EOL:
    index = msg_options[msgt].eol.bg;
    break;
  default:
    index = msg_options[msgt].def_field.bg;
    break;
  }
  if ( index == MAS_MSG_COLOR_NONE )
    index = msg_options[msgt].def_field.bg;
  if ( index == MAS_MSG_COLOR_NONE )
    index = MAS_MSG_COLOR_REDP;
  return bg_palette[index - 1];
}

unsigned
mas_fg_color( mas_msg_type_t msgt, mas_msg_field_type_t field )
{
  int index = 0;

  switch ( field )
  {
  case MAS_MSG_FIELD_ELAPSED:
    index = msg_options[msgt].elapsed.fg;
    break;
  case MAS_MSG_FIELD_CONSUME:
    index = msg_options[msgt].consume.fg;
    break;
  case MAS_MSG_FIELD_ERRCODE:
    index = msg_options[msgt].errcode.fg;
    break;
  case MAS_MSG_FIELD_PREFIX:
    index = msg_options[msgt].prefix.fg;
    break;
  case MAS_MSG_FIELD_MESSAGE:
    index = msg_options[msgt].message.fg;
    break;
  case MAS_MSG_FIELD_CODEPOS:
    index = msg_options[msgt].codepos.fg;
    break;
  case MAS_MSG_FIELD_SUFFIX:
    index = msg_options[msgt].suffix.fg;
    break;
  case MAS_MSG_FIELD_PID:
    index = msg_options[msgt].pid.fg;
    break;
  case MAS_MSG_FIELD_PIDNAME:
    index = msg_options[msgt].pidname.fg;
    break;
  case MAS_MSG_FIELD_THREAD_INFO:
    index = msg_options[msgt].thread_info.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME:
    index = msg_options[msgt].thread_type_name.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MAIN:
    index = msg_options[msgt].thread_type_name_main.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MASTER:
    index = msg_options[msgt].thread_type_name_master.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LISTENER:
    index = msg_options[msgt].thread_type_name_listener.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TRANSACTION:
    index = msg_options[msgt].thread_type_name_transaction.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TICKER:
    index = msg_options[msgt].thread_type_name_ticker.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_WATCHER:
    index = msg_options[msgt].thread_type_name_watcher.fg;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LOGGER:
    index = msg_options[msgt].thread_type_name_logger.fg;
    break;
  case MAS_MSG_FIELD_THREAD_STATUS:
    index = msg_options[msgt].thread_status.fg;
    break;
  case MAS_MSG_FIELD_EOL:
    index = msg_options[msgt].eol.fg;
    break;
  default:
    index = msg_options[msgt].def_field.fg;
    break;
  }
  if ( index == MAS_MSG_COLOR_NONE )
    index = msg_options[msgt].def_field.fg;
  if ( index == MAS_MSG_COLOR_NONE )
    index = MAS_MSG_COLOR_BLACKP;
  return fg_palette[index - 1];
}

unsigned
mas_bold( mas_msg_type_t msgt, mas_msg_field_type_t field )
{
  switch ( field )
  {
  case MAS_MSG_FIELD_ELAPSED:
    return msg_options[msgt].elapsed.bold;
  case MAS_MSG_FIELD_CONSUME:
    return msg_options[msgt].consume.bold;
  case MAS_MSG_FIELD_ERRCODE:
    return msg_options[msgt].errcode.bold;
  case MAS_MSG_FIELD_PREFIX:
    return msg_options[msgt].prefix.bold;
  case MAS_MSG_FIELD_MESSAGE:
    return msg_options[msgt].message.bold;
  case MAS_MSG_FIELD_CODEPOS:
    return msg_options[msgt].codepos.bold;
  case MAS_MSG_FIELD_SUFFIX:
    return msg_options[msgt].suffix.bold;
  case MAS_MSG_FIELD_PID:
    return msg_options[msgt].pid.bold;
  case MAS_MSG_FIELD_PIDNAME:
    return msg_options[msgt].pidname.bold;
  case MAS_MSG_FIELD_THREAD_INFO:
    return msg_options[msgt].thread_info.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME:
    return msg_options[msgt].thread_type_name.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MAIN:
    return msg_options[msgt].thread_type_name_main.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MASTER:
    return msg_options[msgt].thread_type_name_master.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LISTENER:
    return msg_options[msgt].thread_type_name_listener.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TRANSACTION:
    return msg_options[msgt].thread_type_name_transaction.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TICKER:
    return msg_options[msgt].thread_type_name_ticker.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_WATCHER:
    return msg_options[msgt].thread_type_name_watcher.bold;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LOGGER:
    return msg_options[msgt].thread_type_name_logger.bold;
  case MAS_MSG_FIELD_THREAD_STATUS:
    return msg_options[msgt].thread_status.bold;
  case MAS_MSG_FIELD_EOL:
    return msg_options[msgt].eol.bold;
  default:
    return msg_options[msgt].def_field.bold;
  }
}

unsigned
mas_show( mas_msg_type_t msgt, int fdetails, mas_msg_field_type_t field )
{
  unsigned hide_with_details = 0;
  unsigned show_no_details = 0;

  switch ( field )
  {
  case MAS_MSG_FIELD_ELAPSED:
    hide_with_details = msg_options[msgt].elapsed.hide_with_details;
    show_no_details = msg_options[msgt].elapsed.show_no_details;
    break;
  case MAS_MSG_FIELD_CONSUME:
    hide_with_details = msg_options[msgt].consume.hide_with_details;
    show_no_details = msg_options[msgt].consume.show_no_details;
    break;
  case MAS_MSG_FIELD_ERRCODE:
    hide_with_details = msg_options[msgt].errcode.hide_with_details;
    show_no_details = msg_options[msgt].errcode.show_no_details;
    break;
  case MAS_MSG_FIELD_PREFIX:
    hide_with_details = msg_options[msgt].prefix.hide_with_details;
    show_no_details = msg_options[msgt].prefix.show_no_details;
    break;
  case MAS_MSG_FIELD_MESSAGE:
    hide_with_details = msg_options[msgt].message.hide_with_details;
    show_no_details = msg_options[msgt].message.show_no_details;
    break;
  case MAS_MSG_FIELD_CODEPOS:
    hide_with_details = msg_options[msgt].codepos.hide_with_details;
    show_no_details = msg_options[msgt].codepos.show_no_details;
    break;
  case MAS_MSG_FIELD_SUFFIX:
    hide_with_details = msg_options[msgt].suffix.hide_with_details;
    show_no_details = msg_options[msgt].suffix.show_no_details;
    break;
  case MAS_MSG_FIELD_PID:
    hide_with_details = msg_options[msgt].pid.hide_with_details;
    show_no_details = msg_options[msgt].pid.show_no_details;
    break;
  case MAS_MSG_FIELD_PIDNAME:
    hide_with_details = msg_options[msgt].pidname.hide_with_details;
    show_no_details = msg_options[msgt].pidname.show_no_details;
    break;
  case MAS_MSG_FIELD_THREAD_INFO:
    hide_with_details = msg_options[msgt].thread_info.hide_with_details;
    show_no_details = msg_options[msgt].thread_info.show_no_details;
    break;
  case MAS_MSG_FIELD_THREAD_TYPE_NAME:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MAIN:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_MASTER:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LISTENER:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TRANSACTION:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_TICKER:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_WATCHER:
  case MAS_MSG_FIELD_THREAD_TYPE_NAME_LOGGER:
    hide_with_details = msg_options[msgt].thread_type_name.hide_with_details;
    show_no_details = msg_options[msgt].thread_type_name.show_no_details;
    break;
  case MAS_MSG_FIELD_THREAD_STATUS:
    hide_with_details = msg_options[msgt].thread_status.hide_with_details;
    show_no_details = msg_options[msgt].thread_status.show_no_details;
    break;
  case MAS_MSG_FIELD_EOL:
    hide_with_details = msg_options[msgt].eol.hide_with_details;
    show_no_details = msg_options[msgt].eol.show_no_details;
    break;
  default:
    hide_with_details = msg_options[msgt].def_field.hide_with_details;
    show_no_details = msg_options[msgt].def_field.show_no_details;
    break;
  }
  return fdetails ? !hide_with_details : show_no_details;
}

void
mas_set_color( mas_msg_type_t msgt, mas_msg_field_type_t field )
{
  MFP( "\x1b[%u;%u;%um", mas_bold( msgt, field ) ? 1 : 22, mas_bg_color( msgt, field ), mas_fg_color( msgt, field ) );
}

void
mas_reset_color( void )
{
  MFP( "\x1b[0m" );
}

#ifndef MAS_NO_CTRL
int
mas_msg_set_file( const char *path )
{
  int r = 0;

  if ( ctrl.msgfile )
  {
    if ( ctrl.msgfile != ctrl.stderrfile && EOF == fclose( ctrl.msgfile ) )
    {
      IEVAL( r, -1 );
    }
    ctrl.msgfile = NULL;
  }
  r = 0;
  if ( path )
  {
    ctrl.msgfile = fopen( path, "a" );
    /* ffcntl( ctrl.msgfile, F_SETFL, O_SYNC ); */
    if ( ctrl.msgfile )
    {
      /* for /dev/pts/... : ERROR{29:Illegal seek} */
      /* errno = 0; */
      IEVAL( r, setvbuf( ctrl.msgfile, NULL, _IONBF, 0 ) );
    }
    else
    {
      YEVALM( r, -1, "(%d) file:%s", path );
    }
  }
  return r;
}
#endif
static int
__mas_msg_prefix( mas_msg_type_t msgt, int fdetails, const char *prefix_fmt, const char *prefix )
{
  const char *qprefix;

  qprefix = prefix;
  {
    int lqp = strlen( MAS_DROP_PREFIX );

    if ( qprefix && *qprefix && 0 == strncmp( prefix, MAS_DROP_PREFIX, lqp ) )
      qprefix = qprefix + lqp;
  }

#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    if ( MAS_CTRL_IS_SERVER && prefix && *prefix )
    {
      /* MFP( prefix_fmt ? prefix_fmt : " %-6s ", prefix ); */
    }
  }
  else
#endif
  {
    if (  /* MAS_CTRL_IS_SERVER && */ qprefix && *qprefix )
    {
      mas_set_color( msgt, MAS_MSG_FIELD_PREFIX );
      MFP( prefix_fmt ? prefix_fmt : "-- %-5s", qprefix );
      mas_reset_color(  );
    }
  }
  return 0;
}

static int
__mas_msg_pid( mas_msg_type_t msgt, int fdetails, pid_t pid )
{
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
#  ifndef MAS_NO_CTRL
    if ( pid == ctrl.main.pid )
    {
      wattron( w_win, A_BOLD );
      MFP( "+" );
      wattroff( w_win, A_BOLD );
      /* wcolor_set( w_win, 2, NULL ); */
      /* MFP( ":%5u", pid );           */
      wcolor_set( w_win, 1, NULL );
    }
    else
#  endif
    {
      MFP( ">%5u:", pid );
    }
  }
  else
#endif
  {
    MFP( ":" );
    mas_set_color( msgt, MAS_MSG_FIELD_PID );
    MFP( "%5u ", pid );
    mas_reset_color(  );
  }
  return 0;
}

static int
__mas_msg_pidname( mas_msg_type_t msgt, int fdetails, pid_t pid )
{
  const char *pidname;

#ifndef MAS_NO_CTRL
  if ( pid == ctrl.threads.n.main.pid )
    pidname = "Main";
  else if ( pid == ctrl.threads.n.daemon.pid )
    pidname = "Daemon";
  else
#endif
    pidname = "-";
  mas_set_color( msgt, MAS_MSG_FIELD_PIDNAME );
  MFP( "%-5s", pidname );
  mas_reset_color(  );
  return 0;
}

#ifndef MAS_NO_THTOOLS
static int
__mas_msg_thread_info( mas_msg_type_t msgt, int fdetails )
{
  th_type_t thtype;

  thtype = mas_thself_type(  );
  pthread_t pth = 0;

  pth = pthread_self(  );
  mas_lcontrol_t *plcontrol = NULL;
  mas_rcontrol_t *prcontrol = NULL;

  plcontrol = mas_thself_plcontrol(  );
  prcontrol = mas_thself_prcontrol(  );
#  ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    unsigned color = 0;

    wattron( w_win, A_BOLD );
    /* MFP( "pth:" ); */
    wcolor_set( w_win, color, NULL );
    switch ( thtype )
    {
    case MAS_THREAD_MASTER:
      MFP( "M0:%u ", MAS_CTRL_STATUS );
      break;
    case MAS_THREAD_LISTENER:
      MFP( "L%lu:%u ", plcontrol ? plcontrol->h.serial : 0xffffffff, plcontrol ? plcontrol->h.status : 999 );
      break;
    case MAS_THREAD_TRANSACTION:
      MFP( "R%lu:%u @ L%lu:%u ", prcontrol ? prcontrol->h.serial : 0xffffffff, prcontrol ? prcontrol->h.status : 999,
           prcontrol->plcontrol ? prcontrol->plcontrol->h.serial : 0xffffffff,
           prcontrol->plcontrol ? prcontrol->plcontrol->h.status : 999 );
      break;
    case MAS_THREAD_LOGGER:
    case MAS_THREAD_WATCHER:
    case MAS_THREAD_TICKER:
      MFP( "%s[%lx]", mas_thread_type_name( thtype ), pth );
      break;
    default:
      MFP( "[%lx]", pth );
      break;
    }
    wcolor_set( w_win, 1, NULL );
    MFP( ":" );
    wattroff( w_win, A_BOLD );
  }
  else
#  endif
  {
    /* mas_set_color( msgt, MAS_MSG_FIELD_THREAD_INFO ); */
    mas_set_color( msgt, MAS_MSG_FIELD_THREAD_TYPE_NAME + thtype );
    MFP( "(%s)", mas_thread_type_name( thtype ) );
    mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
    switch ( thtype )
    {
    case MAS_THREAD_MASTER:
      mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
      MFP( " M-:%u:", MAS_CTRL_STATUS );
      break;
    case MAS_THREAD_MAIN:
      mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
      MFP( " Z-:%u:", MAS_CTRL_STATUS );
      break;
    case MAS_THREAD_TICKER:
      mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
      MFP( " T-:%u:", MAS_CTRL_STATUS );
      break;
    case MAS_THREAD_WATCHER:
      mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
      MFP( " W-:%u:", MAS_CTRL_STATUS );
      break;
    case MAS_THREAD_LOGGER:
      mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
      MFP( " G-:%u:", MAS_CTRL_STATUS );
      break;
    case MAS_THREAD_LISTENER:
      mas_set_color( msgt, MAS_MSG_FIELD_THREAD_STATUS );
      MFP( " L%lu:%u:", plcontrol ? plcontrol->h.serial : 0xffffffff, plcontrol ? plcontrol->h.status : 999 );
      break;
    case MAS_THREAD_TRANSACTION:
      MFP( " R%lu:%u @ L%lu:%u:", prcontrol ? prcontrol->h.serial : 0xffffffff, prcontrol ? prcontrol->h.status : 999,
           prcontrol->plcontrol ? prcontrol->plcontrol->h.serial : 0xffffffff,
           prcontrol->plcontrol ? prcontrol->plcontrol->h.status : 999 );
      /* MFP( "pth:\x1b[1;%dm(%s) R%lu:%u @ L%lu:%u\x1b[0m:", 35, mas_thread_type_name( thtype ),  */
      /*      prcontrol ? prcontrol->h.serial : 0xffffffff, prcontrol ? prcontrol->h.status : 999, */
      /*      prcontrol->plcontrol ? prcontrol->plcontrol->h.serial : 0xffffffff,                  */
      /*      prcontrol->plcontrol ? prcontrol->plcontrol->h.status : 999 );                       */
      break;
    default:
      MFP( "[%lx]\x1b[0m:", pth );
      break;
    }
    mas_reset_color(  );
  }
  return 0;
}
#endif

static int
__mas_msg_code_position( mas_msg_type_t msgt, int fdetails, const char *func, int line )
{
  if ( MAS_MSG_BIT( msg_funline ) )
  {
    mas_set_color( msgt, MAS_MSG_FIELD_CODEPOS );
    MFP( " L%03d:%-25s:", line, func );
    mas_reset_color(  );
    /* MFP( "\t" ); */
  }
  else
  {
    MFP( "<>" );
  }
  return 0;
}

int
__mas_msg_message( mas_msg_type_t msgt, int fdetails, const char *fmt, va_list args )
{
  int r = 0;
  size_t len;
  char *message = NULL;
  size_t buffer_size = 4096;

  message = mas_malloc( buffer_size );
  vsnprintf( ( char * ) message, buffer_size, fmt, args );
  len = strlen( message );
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    MFP( fdetails ? " %-15s" : " %-50s", message );
  }
  else
#endif
  {
    MFP( "\x1b[K" );
    mas_set_color( msgt, MAS_MSG_FIELD_MESSAGE );
    MFP( fdetails ? " %-15s" : " %-50s", message );
    mas_reset_color(  );
  }
  if ( message )
    mas_free( message );
  r = ( len < ( fdetails ? 15 : 50 ) ) ? 0 : -1;
  return r;
}

int
__mas_msg_suffix( mas_msg_type_t msgt, int fdetails, const char *suffix )
{
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    MFP( "   %s", suffix ? suffix : "" );
  }
  else
#endif
  {
    MFP( "   " );
    mas_set_color( msgt, MAS_MSG_FIELD_SUFFIX );
    MFP( "%s", suffix ? suffix : "" );
    mas_reset_color(  );
  }
  return 0;
}

int
__mas_msg_elapsed( mas_msg_type_t msgt, int fdetails )
{
#ifndef MAS_NO_CTRL
  if ( ctrl.stamp.start_time )
  {
    unsigned long elapsed_time;
    unsigned long cur_time = ( unsigned long ) time( NULL );

    elapsed_time = cur_time - ctrl.stamp.start_time;
    mas_set_color( msgt, MAS_MSG_FIELD_ELAPSED );
    MFP( "+%5lus ", elapsed_time );
    mas_reset_color(  );
  }
  else
#endif
  {
    MFP( " %5s  ", "--" );
  }
  return 0;
}

int
__mas_msg_errcode( mas_msg_type_t msgt, int fdetails )
{
  MFP( "{e%03d}", errno );
  return 0;
}

int
__mas_msg_consume( mas_msg_type_t msgt, int fdetails )
{
  extern unsigned long memory_balance;

  MFP( " (m%5lu) ", memory_balance );
  return 0;
}


static int
__mas_vmsg( const char *func, int line, mas_msg_type_t msgt, int details, const char *prefix_fmt, const char *prefix, const char *suffix,
            const char *fmt, va_list args )
{
  int r = 0;

#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    MFP( "\n*" );
  }
  else
#endif
  {
    MFP( "\r" );
  }
  {
    pid_t pid;

    pid = getpid(  );
    if ( mas_show( msgt, details, MAS_MSG_FIELD_ELAPSED ) )
      r = __mas_msg_elapsed( msgt, details );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_ERRCODE ) )
      r = __mas_msg_errcode( msgt, details );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_CONSUME ) )
      r = __mas_msg_consume( msgt, details );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_PREFIX ) )
      r = __mas_msg_prefix( msgt, details, prefix_fmt, prefix );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_PIDNAME ) )
      r = __mas_msg_pidname( msgt, details, pid );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_PID ) )
      r = __mas_msg_pid( msgt, details, pid );

#ifndef MAS_NO_THTOOLS
    if ( MAS_CTRL_IS_SERVER && mas_show( msgt, details, MAS_MSG_FIELD_THREAD_INFO ) )
      r = __mas_msg_thread_info( msgt, details );
#endif
    if ( mas_show( msgt, details, MAS_MSG_FIELD_CODEPOS ) )
      r = __mas_msg_code_position( msgt, details, func, line );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_MESSAGE ) )
      r = __mas_msg_message( msgt, details, fmt, args );

    if ( !( r < 0 ) && mas_show( msgt, details, MAS_MSG_FIELD_SUFFIX ) )
      r = __mas_msg_suffix( msgt, details, suffix );

    if ( mas_show( msgt, details, MAS_MSG_FIELD_EOL ) )
    {
      mas_set_color( msgt, MAS_MSG_FIELD_EOL );
      MFP( "\x1b[K" );
      mas_reset_color(  );
    }

#ifdef MAS_USE_CURSES
    if ( use_curses )
    {
    }
    else
#endif
    {
      MFP( "\n" );
    }
  }
  return r;
}

int
mas_msg( const char *func, int line, mas_msg_type_t msgt, int allow, int details,
         const char *prefix_fmt, const char *prefix, const char *suffix, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  /* mas_channel_t *pchannel = NULL; */
  va_start( args, fmt );
  if ( allow )
  {
    {
      int can = 0;

      can = MAS_CTRL_MESSAGES;
      if ( can )
      {
#ifndef MAS_NO_THTOOLS
        th_type_t thtype;

        thtype = mas_thself_type(  );
        switch ( thtype )
        {
        case MAS_THREAD_MASTER:
          can = MAS_MSG_BIT( msg_trace_main );
          break;
        case MAS_THREAD_LISTENER:
          can = MAS_MSG_BIT( msg_trace_listener );
          break;
        case MAS_THREAD_TRANSACTION:
          can = MAS_MSG_BIT( msg_trace_transaction );
          break;
        default:
          can = 1;
          break;
        }
#else
        can = 1;
#endif
      }
      if ( allow == 777 )
        can = 1;
      if ( can )
      {
#ifndef MAS_NO_CTRL
        pthread_mutex_lock( &ctrl.thglob.msg_mutex );
#endif
        r = __mas_vmsg( func, line, msgt, details, prefix_fmt, prefix, suffix, fmt, args );
#ifndef MAS_NO_CTRL
        pthread_mutex_unlock( &ctrl.thglob.msg_mutex );
#endif
      }
    }
  }
  va_end( args );
  return r;
}

static int
mas_verror( const char *func, int line, int merrno, int *perrno, const char *fmt, va_list args )
{
  int r = 0;
  mas_msg_type_t msgt = MAS_MSG_ERR;

#ifdef MAS_USE_CURSES
  WINDOW *w_win;

  w_win = w_other;
#endif


#ifndef MAS_NO_CTRL
  pthread_mutex_lock( &ctrl.thglob.emsg_mutex );
#endif
  if ( merrno )
  {
    char pref[512];
    char errbuf[1024];
    char *se = NULL;

    se = strerror_r( merrno, errbuf, sizeof( errbuf ) );
#ifndef MAS_NO_CTRL
    snprintf( pref, sizeof( pref ), "(%u:%s) i/s:%u:i/c:%u", merrno, se ? se : NULL, ctrl.keep_listening, MAS_CTRL_IN_CLIENT );
#else
    snprintf( pref, sizeof( pref ), "(%u:%s)", merrno, se ? se : NULL );
#endif
    r = __mas_vmsg( func, line, msgt, 1, "{  %4s   }", pref, NULL, fmt, args );
  }
  else
  {
    r = __mas_vmsg( func, line, msgt, 1, "{  %4s  }", "E-R-R-O-R", NULL, fmt, args );
  }
#ifndef MAS_NO_CTRL
  pthread_mutex_unlock( &ctrl.thglob.emsg_mutex );
#endif
  switch ( merrno )
  {
  case EINTR:
  case ECONNRESET:
  case ECONNREFUSED:
    /* off : 20121223 *//* ctrl.keep_listening = 0; */
    /* off : 20130102 *//* ctrl.in_client = 0; */
    /* OR ???? : ctrl.fatal = 1; */
    break;
  }
  if ( perrno )
    *perrno = 0;
  return r;
}

int
mas_error( const char *func, int line, int merrno, int *perrno, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mas_verror( func, line, merrno, perrno, fmt, args );
  va_end( args );
  return r;
}

void
_mas_perr( const char *func, int line, int merrno, int *perrno )
{
  if ( merrno )
  {
    static int errcnt = 0;
    char errbuf[1024];
    char *se;

    /* MAS_MSG_BIT(msg_tr) = 1; */
    errcnt++;
    se = strerror_r( merrno, errbuf, sizeof( errbuf ) );
#ifndef MAS_NO_CTRL
    mas_error( func, line, merrno, perrno, "[error %d] %s (i/c:%d; i/s:%d; fatal:%d)", merrno, se, MAS_CTRL_IN_CLIENT, ctrl.keep_listening,
               ctrl.fatal );
#else
    mas_error( func, line, merrno, perrno, "[error %d] %s", merrno, se );
#endif
    if ( errcnt < 10 )
    {
    }
    else if ( errcnt < 20 )
      sleep( 1 );
    else
    {
      sleep( 5 );
      errcnt = 0;
    }
    /* sleep(10); */
  }
}

void *
mas_fatal( void )
{
  MFPZ( "pointer error" );
  exit( 11 );
}
