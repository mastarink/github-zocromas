#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "tools.h"
#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
#include "mulconfnt_error.h"

#include "mastest.h"

int
test_3q( int argc _uUu_, const char _uUu_ * argv[], int _uUu_ nseries, const char _uUu_ * series_suffix , int do_fprintf _uUu_)
{
  mastest_next_group(  );
  {
    const char *strings[] = {
      "'quoted1'", "quoted1",
      "'quoted2", "'quoted2",
      "'quoted3'tail", "quoted3tail",
      "test='quoted4'", "test=quoted4",
      "test='quoted5", "test='quoted5",
      "\"quoted1\"", "quoted1",
      "\"quoted1", "\"quoted1",
      "test=\"quoted1\"", "test=quoted1",
      "test=\"quoted1", "test=\"quoted1",
      "\"quoted1\"tail", "quoted1tail",

      "'quo\"ted1'", "quo\"ted1",
      "'quo\"ted1", "'quo\"ted1",
      "'quo\"ted1'tail", "quo\"ted1tail",
      "test='quo\"ted1'", "test=quo\"ted1",
      "test='quo\"ted1", "test='quo\"ted1",
      "\"quo'ted1\"", "quo'ted1",
      "\"quo'ted1", "\"quo'ted1",
      "test=\"quo'ted1\"", "test=quo'ted1",
      "test=\"quo'ted1", "test=\"quo'ted1",
      "\"quo'ted1\"tail", "quo'ted1tail",

      "'quo\\'ted20'", "quo'ted20",
      "'quo\\'ted21", "'quo\\'ted21",                                /* ? */
      "'quo\\'ted22'tail", "quo'ted22tail",
      "test='quo\\'ted23'", "test=quo\'ted23",
      "test='quo\\'ted24", "test='quo\\'ted24",
      "\"quo\\\"ted25\"", "quo\"ted25",
      "\"quo\\\"ted26", "\"quo\\\"ted26",

      "'quo\"t\"ed27'", "quo\"t\"ed27",
      "'quo\"t\"ed1", "'quo\"t\"ed1",
      "'quo\"t\"ed1'tail", "quo\"t\"ed1tail",
      "test='quo\"t\"ed1'", "test=quo\"t\"ed1",
      "test='quo\"t\"ed1", "test='quo\"t\"ed1",
      "\"quo't'ed1\"", "quo't'ed1",
      "\"quo't'ed1", "\"quo't'ed1",
      "test=\"quo't'ed1\"", "test=quo't'ed1",
      "test=\"quo't'ed1", "test=\"quo't'ed1",
      "\"quo't'ed1\"tail", "quo't'ed1tail",

      "'", "'",
      "\"", "\"",
    };
    for ( unsigned istr = 0; istr < sizeof( strings ) / sizeof( strings[0] ); istr += 2 )
    {
      char *s;

      s = mulconfnt_config_option_unquote( strings[istr], "'\"" );
      mastest_exam( __LINE__, 0 == mas_strcmp( s, strings[istr + 1] ), "OK", "Error", "%s ? %s\t\t(%d)", s, strings[istr + 1], istr / 2 );
      mas_free( s );
    }
  }
  return 0;
}
