#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_trace.h"

#include "duf_optable_def.h"

#include "duf_dbg.h"
#include "duf_debug_defs.h"

#include "duf_utils_print.h"

#include "duf_config_ref.h"
#include "duf_options.h"
#include "duf_option_cmd.h"
#include "duf_option_help.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_interactive.h"
/* ###################################################################### */


extern duf_scan_callbacks_t duf_integrity_callbacks __attribute( ( weak ) ),
      duf_directories_callbacks __attribute( ( weak ) ),
      duf_filedata_callbacks __attribute( ( weak ) ),
      duf_filenames_callbacks __attribute( ( weak ) ),
      duf_collect_openat_crc32_callbacks __attribute( ( weak ) ),
      duf_collect_openat_sd5_callbacks __attribute( ( weak ) ),
      duf_collect_openat_md5_callbacks __attribute( ( weak ) ),
      duf_collect_mime_callbacks __attribute( ( weak ) ), duf_collect_exif_callbacks __attribute( ( weak ) ),
      /* duf_collect_mdpath_callbacks __attribute( ( weak ) ), */
      /* duf_print_md5_callbacks __attribute( ( weak ) ),      */
 
      duf_print_tree_callbacks __attribute( ( weak ) ),
      duf_print_dir_callbacks __attribute( ( weak ) ),
      duf_bubububububububububububububububububububububububububububububububububububu __attribute( ( weak ) );



/* static const duf_longval_extended_t _lo_extended_i[] = {                                                                                       */
/*   {.o = {DO_Q( "version" ), (*        *) DO_A_N, (* *) DO_V( VERSION )} (*       *) , DO_CL( REFERENCE ) (* *) ,                               */
/*    (*      *) DO_T( AFUN ), DO_AFUN( duf_option_version ),.stage = 1 (*                                     *) , DO_H( version info ) (* *) }, */
/*   {.o = {DO_Q( "quit" ), (*   *) DO_A_N, (* *) DO_VF( INTERACTIVE )} (*          *) , DO_CL( CONTROL ) (*   *) ,                               */
/*    (*      *) DO_O( NOFLAG, cli.act.v ), DO_FL( act, interactive ),.stage = 1 (*                            *) , DO_H( get dir info ) (* *) }, */
/*   {.o = {DO_Q( "cd" ), (*        *) DO_A_O, (* *) DO_V( CD )},.stage = 1 (*      *) , DO_CL( CONTROL ) (*   *) ,                               */
/*    (*      *) DO_O( PDISTR, pdi ) (*                                                                        *) , DO_H( pdi cd ) (*       *) }, */
/*   {.o = {DO_Q( "lsfiles" ), (*           *) DO_A_O, (* *) DO_V( SCCB )} (*       *) , DO_CL( CONTROL ) (*   *) ,                               */
/*    (*      *) DO_O( PDISCCB, pdi ),.stage = 1,.call = {.fdesc =                                                                                */
/*                                                        {.hi = {.sccb = &duf_print_dir_callbacks}}} (*       *) , DO_H( pdi cd ) (*       *) }, */
/*                                                                                                                                                */
/*   {0}                                                                                                                                          */
/* };                                                                                                                                             */
/*                                                                                                                                                */
/* const duf_longval_extended_t *lo_extended_i = _lo_extended_i;                                                                                  */
/* unsigned lo_extended_i_count = sizeof( _lo_extended_i ) / sizeof( _lo_extended_i[0] );                                                         */

int
duf_interactive( void )
{
  int r = 0;

  /* DUF_UNUSED duf_depthinfo_t di = {.depth = -1, */
  /*   .seq = 0,                                   */
  /*   .levinfo = NULL,                            */
  /*   .u = duf_config->u,                         */
  /* };                                            */
  /* r = duf_pdi_init_wrap( &di, real_path, 0 );   */

  /* rl_generic_bind( ISMACR, "\x1bq", ( char * ) "server exit\n", rl_get_keymap(  ) );     */
  /* rl_generic_bind( ISMACR, "\x1b[21~", ( char * ) "server exit\n", rl_get_keymap(  ) );  */
  /* rl_generic_bind( ISMACR, "\x1bi", ( char * ) "get server info\n", rl_get_keymap(  ) ); */
  /* rl_generic_bind( ISMACR, "\x1b" "b", ( char * ) "bye\n", rl_get_keymap(  ) );          */

  /* snprintf( rl_prompt, sizeof( rl_prompt ), "(bye to force exit) (%u) %% ", ctrl.restart_cnt ); */
  /* rh = read_history( fpath ); */

  /* duf_pdi_close( &duf_config->di ); */
  assert( duf_config->pdi );
  assert( duf_config->pdi->levinfo );
  /* if ( r >= 0 ) */
  {
    char *duf_rl_buffer = NULL;
    static char rl_prompt[256 * 10] = "";

    DUF_E_MAX( 5, DUF_ERROR_MAX_DEPTH );

    if ( !*rl_prompt )
      snprintf( rl_prompt, sizeof( rl_prompt ), "%s> ", "CMD" );

    add_history( "cd /mnt/new_media/media/photo/Pictures/photos/" );
    add_history( "quit" );
    add_history( "pwd" );
    add_history( "max-depth" );
    add_history( "max-seq" );

    while ( r >= 0 && DUF_ACT_FLAG( interactive ) )
    {
      r = 0;
      DOR( r, duf_pdi_reinit_oldpath( duf_config->pdi, DUF_U_FLAG( recursive ) ) );

      if ( duf_config && duf_config->pdi )
        snprintf( rl_prompt, sizeof( rl_prompt ), "%s:%s> ", "db", duf_levinfo_path( duf_config->pdi ) );
      while ( !duf_rl_buffer )
        duf_rl_buffer = readline( rl_prompt );
      mas_chomp( duf_rl_buffer );
      if ( *duf_rl_buffer )
      {
        if ( duf_rl_buffer && *duf_rl_buffer )
          add_history( duf_rl_buffer );
        /* DOR( r, duf_execute_cmd_long( duf_rl_buffer, lo_extended_i, lo_extended_i_count, ' ', 1 ) ); */
        DOR( r, duf_execute_cmd_long_std( duf_rl_buffer, ' ', 1 ) );
      }
      /* DUF_TEST_R( r ); */
      DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_MULTIPLE, DUF_ERROR_OPTION, DUF_ERROR_OPTION_VALUE, DUF_ERROR_PATH, DUF_ERROR_MAX_SEQ_REACHED );
      free( duf_rl_buffer );
      duf_rl_buffer = NULL;
      fprintf( stderr, "\n" );
    }
  }
  return r;
}
