#ifndef MAS_GLOBAL_H
#  define MAS_GLOBAL_H

/*
this:
  mas_global.h
related:
  mas_common.h
  mas_common_types.h

*/

#  include "control/inc/mas_control_types.h"
#  include "init/inc/mas_opts_types.h"

extern unsigned long __MAS_LINK_DATE__;
extern unsigned long __MAS_LINK_TIME__;
extern unsigned long __MAS_LINK_TIMESTAMP__;

extern unsigned long vdate;
extern unsigned long vtime;
extern unsigned long vts;
extern char *vtsc;
extern unsigned long lts;

extern unsigned long memory_allocated;
extern unsigned long memory_allocated_cnt;
extern unsigned long memory_freed;
extern unsigned long memory_freed_cnt;
extern unsigned long memory_balance;
extern unsigned long memory_balance_cnt;


extern mas_control_t ctrl;
extern mas_options_t opts;



#endif
