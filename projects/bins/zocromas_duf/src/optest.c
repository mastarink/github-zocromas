#include <assert.h>
#include <stddef.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/error/mas_errors_maintenance.h>

#include "duf_maintenance_options.h"


#include "duf_defs.h"

#include "duf_printn_defs_preset.h"
#include "duf_tracen_defs_preset.h"

#include "duf_errorn_defs.h"
#include "duf_tracen_defs.h"
#include "duf_printn_defs.h"

#include "duf_se.h"
#include "duf_dodefs.h"

#include "duf_option_types.h"
#include "duf_options_all_stages.h"
#include "duf_option_types.h"
#include "duf_option_clarify_new.h"

typedef struct
{
  unsigned one:1;
  unsigned two:1;
} something_bits1_t;
typedef struct
{
  something_bits1_t set1;
} something1_t;

#define DOO_Q(_name) .o.name =_name
#define DOO_BFL(_vt, _prf, _loc, _fld) .flag_bitnum=DUF_FLAG_ ## _loc ## _ ## _fld, .can_no=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

const duf_longval_extended_table_t optable_test = { /* zzzzzz */
  .name = "test",
/* DO_AT_STAGE( SETUP ), */
  .xlist = {
            {
             .o.name = "test-disable-nobflag",
             .flag_bitnum = 2,
             .can_no = 1,
             .o.has_arg = no_argument,
             .vtype = DUF_OPTION_VTYPE_NOBSFLAG,
             .m_hasoff = 1,
             .m_offset = offsetof( something1_t, set1 ),
             .oclass = DUF_OPTION_CLASS_TEST,
             .help = "..." /* */
             },

            {.o.name = NULL}
            }
};

something1_t som1 = { {0, 0} };

SR( TOP, main, int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) )
{
  CR( treat_option_stage_ne, DUF_OPTION_STAGE_DEBUG, NULL, NULL, NULL ); /* here to be before following DUF_TRACE's */
  CR( treat_option_stage_ne, DUF_OPTION_STAGE_BOOT, NULL, NULL, NULL );

  return 0;
  ER( TOP, main, int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) );
}

int
main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) )
{
  int r;

  r = duf_main( argc, argv );
  return r == 0 ? r : 34;
}
