/* #undef MAS_TRACING */
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
#include "duf_option_config.h"
#include "duf_option_config_credel.h"
#include "duf_options_all_stages.h"
#include "duf_option_types.h"
#include "duf_option_clarify_new.h"

typedef struct
{
  unsigned one:1;
  unsigned two:1;
  unsigned three:1;
  unsigned four:31;
} something_bits1_t;
typedef union
{
  something_bits1_t flags;
  unsigned bits;
} something_bits1_combo_t;
typedef struct
{
  something_bits1_combo_t set1;
  something_bits1_combo_t set2;
  something_bits1_combo_t set3;
} something1_t;

#define DOO_Q(_name) .o.name =_name
#define DOO_BFL(_vt, _prf, _loc, _fld) .flag_bitnum=DUF_FLAG_ ## _loc ## _ ## _fld, .can_no=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

something1_t som1 = {.set3.bits = 0x3f };

#define MAS_OPT_BXFLAG( _name, _set, _fld, _bitnum, _can_no, _oclass, _help ) { \
  .o.name = _name, \
  .o.has_arg = no_argument, \
  .vtype = DUF_OPTION_VTYPE_BXFLAG, \
  .vsize= sizeof(_set._fld), \
  .m_hasoff = 1, \
  .relto = DUF_OFFSET_varptr, \
  .reltoptr = &_set, \
  .m_offset = offsetof( typeof(_set), _fld ), \
  .flag_bitnum = _bitnum, \
  .oclass = DUF_OPTION_CLASS_ ## _oclass, \
  .can_no=_can_no, \
  .help=_help \
}

const duf_longval_extended_table_t optable_test = {                  /* zzzzzz */
  .name = "test",
/* DO_AT_STAGE( SETUP ), */
  .xlist = {
            {
             .o.name = "test-disable-nobflag",
             .flag_bitnum = 1,
             .can_no = 1,
             .o.has_arg = no_argument,
             .vtype = DUF_OPTION_VTYPE_NOBSFLAG,
             .m_hasoff = 1,
             .relto = DUF_OFFSET_varptr,
             .reltoptr = &som1,
             .m_offset = offsetof( something1_t, set2 ),
             .oclass = DUF_OPTION_CLASS_TEST,
             .help = "..."                                           /* */
             },
            MAS_OPT_BXFLAG( "test-q-bflag", som1, set3, 67, 1, TEST, "something to help" ),
            {.o.name = NULL}
            }
};

static const duf_longval_extended_table_t *const optable_test_list[] = { &optable_test, NULL };

SR( TOP, main, int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) )
{
  duf_config_cli_t *clio;

  clio = duf_cli_options_create( argc, argv, optable_test_list, NULL /* config_dir */ ,
                                 NULL /* commands_dir */ , NULL /* varfunc */  );
  fprintf( stderr, "%d OPTEST ~ %lu\n", QERRIND, sizeof( something_bits1_combo_t ) );

  CR( treat_option_stage_ne, clio, DUF_OPTION_STAGE_DEBUG, NULL, NULL, NULL ); /* here to be before following DUF_TRACE's */
  fprintf( stderr, "%d OPTEST\n", QERRIND );

  CR( treat_option_stage_ne, clio, DUF_OPTION_STAGE_BOOT, NULL, NULL, NULL );
  fprintf( stderr, "%d OPTEST\n", QERRIND );

  CR( treat_all_optstages, clio, NULL, NULL, NULL, NULL );
  fprintf( stderr, "%d OPTEST\n", QERRIND );

  mas_error_report_all( 0, stderr, /* duf_verbose ? duf_verbose(  ) : */ 3 );
  fprintf( stderr, "%d OPTEST %8x %8x %8x\n"                         /* */
           , QERRIND                                                 /* */
           , som1.set1.bits                                          /* */
           , som1.set2.bits                                          /* */
           , som1.set3.bits                                          /* */
           );
  duf_cli_options_delete( clio );
  {
    unsigned char v0 = 0;
    unsigned short v1 = 0;
    unsigned v2 = 0;
    unsigned long v3 = 0;
    unsigned long v4 = 0;
    unsigned long long v5 = 1;

  /* v0--; v1--; v2--; v3--; v4--; */
    v0 = ~v0;
    v1 = ~v1;
    v2 = ~v2;
    v3 = ~v3;
    v4 = ~v4;
    {
      int sh = 67;

      v5 = ( ( typeof( v5 ) ) 1 ) << sh;
    }
    fprintf( stderr, "%llx:%llx:%llx:%llx:%llx:%llx\n", ( unsigned long long ) v0, ( unsigned long long ) v1, ( unsigned long long ) v2,
             ( unsigned long long ) v3, ( unsigned long long ) v4, ( unsigned long long ) v5 );
  }
  ER( TOP, main, int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) );
}

int
main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) )
{
  int r;

  r = duf_main( argc, argv );
  return r == 0 ? r : 34;
}
