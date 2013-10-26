/* #include <stdio.h> */
/* #include <stdlib.h> */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"
#include "mas_runonce_pid.h"



#include "mas_runonce_section_pid.h"





int
runonce_section_get_pids( config_section_t * sect, runonce_flags_t flags )
{
  int npids = 0;
  int targc = 0;
  char **targv = NULL;

  if ( sect->values[RUNONCE_OPTS4PID] && sect->values[RUNONCE_LAUNCHER] && sect->values[RUNONCE_OPTIONS] )
  {
    targc = mas_add_argv_args( targc, &targv, sect->values[RUNONCE_LAUNCHER], 0 );
    targc = mas_add_argv_args( targc, &targv, sect->values[RUNONCE_OPTIONS], 0 );
  }
  {
    pid_t pids[512];

    if ( !npids )
    {
      char *pref = sect->values[RUNONCE_PREFIX];
      char *prog = sect->values[RUNONCE_LAUNCHER];
      char *path = sect->values[RUNONCE_PATH];
      char *process = sect->values[RUNONCE_PROCESS];
      char *psname = sect->values[RUNONCE_PSNAME];
      int different = ( !prog || !psname || 0 != strcmp( prog, psname ) );

      if ( !pref )
      {
        pref = psname;
        prog = NULL;
      }
      npids = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), pref, prog, path, targc, targv, flags );
      if ( flags.verbose )
        printf( "(%2d) %5s pref:%18s; prog:%18s; path:%5s; psname:%18s; process: %-25s\n", npids,
                different ? "!!!!" : "", pref ? pref : "-", prog ? prog : "-", path ? path : "-", psname, process ? process : "-" );
    }
    sect->instances = npids;
    mas_free( sect->pids );
    sect->pids = NULL;
    sect->pids = mas_malloc( npids * sizeof( pid_t ) );
    memcpy( sect->pids, pids, npids * sizeof( pid_t ) );
    sect->npids = npids;
    mas_del_argv( targc, targv, 0 );
  }
  return sect->npids;
}
