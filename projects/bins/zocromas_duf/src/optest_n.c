/* #undef MAS_TRACING */
#include <assert.h>
#include <stddef.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/trace/mas_trace_credel.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include <mastar/multiconfig/muc_se.h>

#include <mastar/multiconfig/muc_option_config_credel.h>
#include <mastar/multiconfig/muc_options_all_stages.h>

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
#define DOO_BFL(_vt, _prf, _loc, _fld) .flag_bitnum=MUC_FLAG_ ## _loc ## _ ## _fld, .can_no=1, DOO_A_N, DOO_OC(_vt, _prf._loc )

something1_t som1 = {.set3.bits = 0x3f };

#define MAS_OPT_BXFLAG( _name, _set, _fld, _bitnum, _can_no, _oclass, _help ) { \
  .o.name      = _name, \
  .o.has_arg   = no_argument, \
  .vtype       = MUC_OPTION_VTYPE_BXFLAG, \
  .vsize       = sizeof(_set._fld), \
  .m_hasoff    = 1, \
  .relto       = MUC_OFFSET_varptr, \
  .reltoptr    = &_set, \
  .m_offset    = offsetof( typeof(_set), _fld ), \
  .flag_bitnum = _bitnum, \
  .oclass      = MUC_OPTION_CLASS_ ## _oclass, \
  .can_no      = _can_no, \
  .help        = _help \
}

const muc_longval_extended_table_t optable_test = {                  /* zzzzzz */
  .name = "test",
/* DO_AT_STAGE( SETUP ), */
  .xlist = {
            {
             .o.name = "test-disable-nobflag",
             .flag_bitnum = 1,
             .can_no = 1,
             .o.has_arg = no_argument,
             .vtype = MUC_OPTION_VTYPE_NOBSFLAG,
             .m_hasoff = 1,
             .relto = MUC_OFFSET_varptr,
             .reltoptr = &som1,
             .m_offset = offsetof( something1_t, set2 ),
             .oclass = MUC_OPTION_CLASS_TEST,
             .help = "..."                                           /* */
             },
            MAS_OPT_BXFLAG( "test-q-bflag", som1, set3, 2, 1, TEST, "something to help" ),
            {.o.name = NULL}
            }
};

static const muc_longval_extended_table_t *const optable_test_list[] = { &optable_test, NULL };

static mas_config_trace_t *config_trace = NULL;
mas_config_trace_t *
duf_get_trace_config( void )
{
  return config_trace;
}

int
mas_output_nocolor( void )
{
  return 0;
}

int
mas_output_force_color( void )
{
  return 0;
}

muc_SR( TOP, main, int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) )
{
  muc_config_cli_t *clio;

  config_trace = mas_config_trace_create( 100 );
  config_trace->class_levels[DUF_TRACE_LEVEL_temp] += 2;
  config_trace->class_levels[DUF_TRACE_LEVEL_errors] += 2;

  clio = muc_cli_options_create( argc, argv, optable_test_list, 0 /* mandatory_config */, NULL /* config_dir */ ,
                                 NULL /* commands_dir */ , NULL /* varfunc */ , config_trace );
  muc_QT( "@@@@@%d OPTEST ~ %lu", QERRIND, sizeof( something_bits1_combo_t ) );

  muc_CR( treat_option_stage_ne, clio, MUC_OPTION_STAGE_DEBUG, NULL, NULL, NULL ); /* here to be before following MAST_TRACE's */
  muc_QT( "@@@@@%d OPTEST", QERRIND );

  muc_CR( treat_option_stage_ne, clio, MUC_OPTION_STAGE_BOOT, NULL, NULL, NULL );
  muc_QT( "@@@@@%d OPTEST", QERRIND );

  muc_CR( treat_all_optstages, clio, NULL, NULL, NULL, NULL );
  muc_QT( "@@@@@%d OPTEST", QERRIND );

  mas_error_report_all( config_trace, 0 /*index */ , 0, stderr, /* mas_verbose ? mas_verbose(  ) : */ 3 );
  muc_QT( "@@@@@%d OPTEST %8x %8x %8x"                                    /* */
     , QERRIND                                                       /* */
     , som1.set1.bits                                                /* */
     , som1.set2.bits                                                /* */
     , som1.set3.bits                                                /* */
           );
  muc_cli_options_delete( clio );
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
    muc_QT( "@%llx:%llx:%llx:%llx:%llx:%llx", ( unsigned long long ) v0, ( unsigned long long ) v1, ( unsigned long long ) v2,
       ( unsigned long long ) v3, ( unsigned long long ) v4, ( unsigned long long ) v5 );
  }
  mas_config_trace_delete( config_trace );
  config_trace = NULL;
  muc_ER( TOP, main, int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) );
}

int
main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ) )
{
  int r;

  r = muc_main( argc, argv );
  return r == 0 ? r : 34;
}
