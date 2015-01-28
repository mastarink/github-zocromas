#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"



#include "duf_prepare_actions.h"
#include "duf_sccb.h"



#include "duf_option_descr.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"
#include "duf_option_restore.h"
#include "duf_option.h"

/* ###################################################################### */
#include "duf_option_help.h"
/* ###################################################################### */


#define DUF_H2C( codename, val ) case DUF_OPTION_HELP_ ## codename: val=DUF_OPTION_CLASS_ ## codename;
/*
 * if arg is help option
 * return class id for options to display the help
 * */
duf_option_class_t
duf_help_option2class( duf_option_code_t codeval )
{
  duf_option_class_t rv = DUF_OPTION_CLASS_BAD;

/*
  DUF_OPTION_CLASS_NONE,

  DUF_OPTION_CLASS_HELP,
  DUF_OPTION_CLASS_SYSTEM,
  DUF_OPTION_CLASS_CONTROL,
  DUF_OPTION_CLASS_REFERENCE,
  DUF_OPTION_CLASS_COLLECT,
  DUF_OPTION_CLASS_SCAN,
  DUF_OPTION_CLASS_UPDATE,
  DUF_OPTION_CLASS_REQUEST,
  DUF_OPTION_CLASS_PRINT,
  
  DUF_OPTION_CLASS_TRACE,
  DUF_OPTION_CLASS_DEBUG,
  DUF_OPTION_CLASS_OBSOLETE,
  DUF_OPTION_CLASS_NODESC,
  DUF_OPTION_CLASS_OTHER,

*/

  switch ( codeval )
  {
    DUF_H2C( NONE, rv );
    break;
    DUF_H2C( HELP, rv );
    break;
    DUF_H2C( SYSTEM, rv );
    break;
    DUF_H2C( CONTROL, rv );
    break;
    DUF_H2C( REFERENCE, rv );
    break;
    DUF_H2C( COLLECT, rv );
    break;
    DUF_H2C( SCAN, rv );
    break;
    DUF_H2C( UPDATE, rv );
    break;
    DUF_H2C( REQUEST, rv );
    break;
    DUF_H2C( PRINT, rv );
    break;
    DUF_H2C( TRACE, rv );
    break;
    DUF_H2C( DEBUG, rv );
    break;
    DUF_H2C( OBSOLETE, rv );
    break;
    DUF_H2C( NODESC, rv );
    break;
    DUF_H2C( OTHER, rv );
    break;
    DUF_H2C( ALL, rv );
    break;
  case DUF_OPTION_SMART_HELP:
    rv = DUF_OPTION_CLASS_ANY;
    break;
  default:
    break;
  }
  return rv;
}

static const char *oclass_titles[DUF_OPTION_CLASS_MAX + 1] = {
  [DUF_OPTION_CLASS_HELP] = "Help",
  [DUF_OPTION_CLASS_NO_HELP] = "No help",
  [DUF_OPTION_CLASS_SYSTEM] = "System",
  [DUF_OPTION_CLASS_CONTROL] = "Control",
  [DUF_OPTION_CLASS_REFERENCE] = "Reference",
  [DUF_OPTION_CLASS_COLLECT] = "Collect",
  [DUF_OPTION_CLASS_SCAN] = "Scan",
  [DUF_OPTION_CLASS_FILTER] = "Filter",
  [DUF_OPTION_CLASS_UPDATE] = "Update",
  [DUF_OPTION_CLASS_REQUEST] = "Request",
  [DUF_OPTION_CLASS_PRINT] = "Print",
  [DUF_OPTION_CLASS_TRACE] = "Trace",
  [DUF_OPTION_CLASS_OBSOLETE] = "Obsolete",
  [DUF_OPTION_CLASS_OTHER] = "Other",
  [DUF_OPTION_CLASS_NONE] = "None",
  [DUF_OPTION_CLASS_DEBUG] = "DEBUG",
  [DUF_OPTION_CLASS_NODESC] = "No desc",
};

void
duf_option_smart_help( duf_option_class_t oclass )
{
  int r = 0;
  int *ashown;
  size_t ss;
  int tbcount;

  tbcount = duf_longindex_extended_count( lo_extended_multi );
  ss = tbcount * sizeof( int );

  ashown = mas_malloc( ss );
  memset( ( void * ) ashown, 0, ss );
  /* for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ ) */
  /* {                                                                                                   */
  /* }                                                                                                   */
  if ( oclass <= DUF_OPTION_CLASS_MAX && oclass_titles[oclass] && *oclass_titles[oclass] )
    DUF_PRINTF( 0, "-=-=-=-=- %s -=-=-=-=-", oclass_titles[oclass] );
  else
    DUF_PRINTF( 0, "-=-=-=-=- <no title set for %d> -=-=-=-=-", oclass );
  for ( int ilong = 0; r >= 0 && duf_config->longopts_table[ilong].name && ilong < tbcount; ilong++ )
  {
    char *s = NULL;
    duf_option_code_t codeval;
    const char *name;
    const duf_longval_extended_t *extd;
    int ie;

    name = duf_config->longopts_table[ilong].name;
    codeval = duf_config->longopts_table[ilong].val;
    /* extended = _duf_find_longval_extended( codeval ); */
    extd = duf_longindex2extended( ilong, &r );
    /* ie = extended ? extended - &lo_extended[0] : -1; */
    ie = ilong;
    if ( r >= 0 )
    {
      int cnd = 0;

      cnd = ( !extd && ( oclass == DUF_OPTION_CLASS_ANY || oclass == DUF_OPTION_CLASS_NODESC ) )
            || ( extd && ( oclass == DUF_OPTION_CLASS_ANY || oclass == extd->oclass ) );
      if ( cnd )
      {
        int shown = -1;

        if ( ie >= 0 )
          shown = ashown[ie];
        if ( shown <= 0 )
        {
          int look = 1;

          /* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, ilong, ie, oclass, name, codeval ); */
          if ( duf_config->help_string )
          {
            char *s = duf_config->help_string;

            look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
          }
          if ( look )
          {
            /* duf_option_class_t hclass; */

            /* hclass = duf_help_option2class( codeval ); */
            s = duf_option_description_d( ilong, "\t", " // ", &r );
            DUF_TEST_R( r );
            /* s = mas_strcat_x( s, " ...................." ); */
            if ( s )
            {
              /* if ( shown >= 0 )                    */
              /*   DUF_PRINTF( 0, " ## %d;", shown ); */

              DUF_PRINTF( 0, "\t%s", s );
              mas_free( s );
            }
            else
            {
              DUF_PRINTF( 0, " ??? %s", name );
            }
          }
        }
        if ( ie >= 0 )
          ashown[ie]++;
      }
    }
  }
  mas_free( ashown );
}

void
duf_option_smart_help_all( duf_option_class_t oclass )
{
  if ( oclass == DUF_OPTION_CLASS_ALL )
    for ( duf_option_class_t oc = DUF_OPTION_CLASS_MIN + 1; oc < DUF_OPTION_CLASS_MAX; oc++ )
      duf_option_smart_help( oc );
}



void
duf_option_help( int argc, char **argv )
{
  int r = 0;

  DUF_PRINTF( 0, "Usage: %s [OPTION]... [PATH]...", argv[0] );
  DUF_PRINTF( 0, "  -h, --help			[%s]", duf_find_longval_help( DUF_OPTION_HELP, &r ) );
  DUF_PRINTF( 0, "  -x, --example			[%s]", duf_find_longval_help( DUF_OPTION_EXAMPLES, &r ) );
  DUF_PRINTF( 0, "  --output-level		[%s]", duf_find_longval_help( DUF_OPTION_OUTPUT_LEVEL, &r ) );
  DUF_PRINTF( 0, "Database ----------" );
  DUF_PRINTF( 0, "  -N, --db-name=%s", duf_config->db.main.name );
  DUF_PRINTF( 0, "  -D, --db-directory=%s", duf_config->db.dir );
  DUF_PRINTF( 0, "  --drop-tables		DANGEROUS!" );
  DUF_PRINTF( 0, "  --remove-database		DANGEROUS!" );
  DUF_PRINTF( 0, "  --create-tables" );
  DUF_PRINTF( 0, "  -V, --vacuum		vacuum" );
  DUF_PRINTF( 0, "Manipulations --------------" );
  DUF_PRINTF( 0, "  --add-path" );
  DUF_PRINTF( 0, "  -R, --recursive		recursive" );
  DUF_PRINTF( 0, "  --uni-scan" );
  DUF_PRINTF( 0, "    --md5" );
  DUF_PRINTF( 0, "    -f, --files" );
  DUF_PRINTF( 0, "    --tree" );
  DUF_PRINTF( 0, "    --print" );
  DUF_PRINTF( 0, "    --sample" );
  DUF_PRINTF( 0, "    --collect" );
  DUF_PRINTF( 0, "Old options" );
  DUF_PRINTF( 0, "  --zero-duplicates" );
  /* DUF_PRINTF(0,  "  --update-duplicates"  ); */
  DUF_PRINTF( 0, "  --zero-filedatas" );
  DUF_PRINTF( 0, "  --update-filedatas" );
  DUF_PRINTF( 0, "  --update-mdpaths" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  --update-exif" );
#endif
  DUF_PRINTF( 0, "  --same-md5" );
  DUF_PRINTF( 0, "Debug ----------" );
  DUF_PRINTF( 0, "  --debug" );
  DUF_PRINTF( 0, "  -v, --verbose=%d", duf_config->cli.dbg.verbose );
  DUF_PRINTF( 0, "  --min-dbg-lines=%lu", duf_config->cli.dbg.max_line );
  DUF_PRINTF( 0, "  --max-dbg-lines=%lu", duf_config->cli.dbg.min_line );
  DUF_PRINTF( 0, "Trace ----------" );
  DUF_PRINTF( 0, "  -A, --trace-action=%d", duf_config->cli.trace.action );
  DUF_PRINTF( 0, "  -C, --trace-current=%d", duf_config->cli.trace.current );
  DUF_PRINTF( 0, "  -S, --trace-scan=%d", duf_config->cli.trace.scan );
  DUF_PRINTF( 0, "  -Q, --trace-sql=%d", duf_config->cli.trace.sql );
  DUF_PRINTF( 0, "  --trace-sample=%d", duf_config->cli.trace.sample );
  DUF_PRINTF( 0, "  --trace-path=%d", duf_config->cli.trace.path );
  DUF_PRINTF( 0, "  -F, --trace-collect=%d", duf_config->cli.trace.collect );
  DUF_PRINTF( 0, "----------------" );
  DUF_TEST_R( r );
}

void
duf_option_examples( int argc, char *const *argv )
{
  DEBUG_START(  );
  DUF_PRINTF( 0, "Examples" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --drop-tables --create-tables" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file" );
  DUF_PRINTF( 0,
              "  run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/  --uni-scan -R --collect --dirent --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/chromium/   --print    -R --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print  --tree  -R --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print   -R --files" );
  DUF_PRINTF( 0, "----------------" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-duplicates" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-exif" );
#endif
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -d -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -d -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample /mnt/new_media/media/down/ --max-seq=10" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample=2 /mnt/new_media/media/down/ --max-seq=10" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -Ri5fd /home/mastar/a/down/ --trace-md5 --trace-new=0 --trace-stdout" );

  DUF_PRINTF( 0, "===== Transition ============================" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  run --update-duplicates --update-mdpath --update-filedata --update-exif" );
#endif
  DUF_PRINTF( 0, "  run --db-name=test`datem`.db  --uni-scan  --mdpath --collect --dirent -dfR  /mnt/new_media/media/down/" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "Collect basic file info recursively to db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db   --uni-scan -R --collect --dirent --files   /home/mastar/a/down/" );
  DUF_PRINTF( 0, "Calculate checksums recursively for files in db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -R --collect --dirent --md5 --files --dirs /home/mastar/a/down/" );
  DUF_PRINTF( 0, "List files recursively from db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan   --print -R -d  --files /home/mastar/a/down/google-chrome/ " );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run --db-name=test.db  -AAPB   /mnt/new_media/media/down/ --summary" );
  DUF_PRINTF( 0, "    same as prev:" );
  DUF_PRINTF( 0, "  run --db-name=test.db --trace-action=2 --create-tables --uni-scan --recursive --collect --dirent"
              " --files --dirs --md5 /mnt/new_media/media/down/ --summary" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan --print  --md5 --noopenat -Rdf --max-dirs=300  --min-dirfiles=5 --min-size=10" );
  DUF_PRINTF( 0, "=============================================" );

  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6 --summary"
              " --format=filesize,seq,filename,md5" );
  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6"
              " --summary --format=filesize,seq,filename,md5 --tree" );
  DUF_PRINTF( 0, "  run  --trace-any=0  --db-name=test20140416.db  --uni-scan   --print  -df --max-seq=76 --max-depth=6 --summary"
              " --format=filesize,seq,filename,nlink,mtime,mode,gid,uid /mnt/new_media/media/down" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test$$.db  --uni-scan --print  --md5 --noopenat  -Rdf --max-dirs=300 --max-items=200 "
              " --min-dirfiles=5 --min-size=10 --noopenat" );

  DUF_PUTS( 0, "" );
  DUF_PUTS( 0, NULL );
  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "=== Working with the tree as for 20140429 ===================" );
  DUF_PRINTF( 0, "  run --remove-database" );
  DUF_PRINTF( 0, "  run -OPA /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run --uni-scan -RdifE5 /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run -RdifE5 /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf" );
  DUF_PRINTF( 0, "========================= as for 20140429 ===================" );
  DUF_PRINTF( 0, "  run --sample=2 -Rdf /home/mastar/a/down/////  --printf --max-seq=2000" );
  DUF_PRINTF( 0, "========================= as for 20140501 ===================" );
  DUF_PRINTF( 0, "  run -PRdifE5 /home/mastar/a/down/chromium/ -SSS" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rd --summary  --tree   /home/mastar/a/down/" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf --max-seq=1000" );
  DUF_PRINTF( 0, "========================= as for 20140504 ===================" );
  DUF_PRINTF( 0, "= full - cycle collect in one command =======================" );
  DUF_PRINTF( 0, "  run  --remove-database -OP  -RdifE5 /home/mastar/a/down/chromium/"
              " --exclude='*.doc' --exclude='*.jpg' --exclude='*.css'" " --exclude='*.html' --exclude='*.png'  --exclude='*.gif'"
              " --exclude='*.js'  --exclude='*.php'" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/" );
  DUF_PRINTF( 0, "=============================================================" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-10 --size=+0" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-1b" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=+1G" );
  DUF_PRINTF( 0, "=============================================================" );
  DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --same=+1 --format=filename,md5id" );
  DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --md5id=1285" );
  DUF_PRINTF( 0, "========================= as for 20140505 ===================" );
  DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/ --md5id=121549" " --format=human,inode=0,dataid=0" );
  DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/" " --format=human,inode=0,dataid=0,seq=0,prefix=0" " --include='*Speckled*'" );
  DUF_PRINTF( 0, "========================= as for 20140506 ===================" );
  DUF_PRINTF( 0, "  run -O -Rdif --mime /mnt/new_media/media/photo --max-seq=2000 (%gsec)", 2.8 );
  DUF_PRINTF( 0, "========================= as for 20140509 ===================" );
  DUF_PRINTF( 0, "  run --remove-database" );
  DUF_PRINTF( 0, "  run   -O						- %s", "creates DB" );
  DUF_PRINTF( 0, "  run   -P     /mnt/new_media/media/photo		- %s", "adds initial path" );
  DUF_PRINTF( 0, "  run   -OP    /mnt/new_media/media/photo		- %s", "creates DB AND adds initial path" );
  DUF_PRINTF( 0, "  run   -RdE   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rd    /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rdf   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rid   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -RidE  /mnt/new_media/media/photo		- %s", "collect directories information" );
  DUF_PRINTF( 0, "  run   -RiDE  /mnt/new_media/media/photo		- %s", "        files data (nameless) information" );
  DUF_PRINTF( 0, "  run   -RinE  /mnt/new_media/media/photo		- %s", "        files names information" );
  DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo		- %s", "        files md5 information (-f pre-open file)" );
  DUF_PRINTF( 0, "  run   -Rife  /mnt/new_media/media/photo		- %s", "        files mime information (-f pre-open file)" );
  DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo		- %s", "print tree w/o files" );
  DUF_PRINTF( 0, "  run   -pRTdf /mnt/new_media/media/photo		- %s", "      tree with files" );
  DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo --max-depth=2	- %s", "      tree with files limited depth" );
  DUF_PRINTF( 0, "  run   -pRdf  /mnt/new_media/media/photo		- %s", "      files (full paths) with dir headers linear" );
  DUF_PRINTF( 0, "  run   -pRd   /mnt/new_media/media/photo		- %s", "      dirs (full paths) linear" );
  DUF_PRINTF( 0, "  run   -pRf   /mnt/new_media/media/photo		- %s", "      files (full paths) linear" );
  DUF_PRINTF( 0, "========================= as for 20140510 ===================" );
  DUF_PRINTF( 0, "  run   -Rdf --sample  --output /mnt/new_media/media/photo/" );
  DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo --progress" );
  DUF_PRINTF( 0, "========================= as for 20140513 ===================" );
  DUF_PRINTF( 0, "  run   -RifX  /mnt/new_media/media/photo		- %s", "        files exif information (-f pre-open file)" );
  DUF_PRINTF( 0, "========================= as for 20140717 ===================" );
  DUF_PRINTF( 0, "  export MSH_DUF_OPTIONS=explain=1			- %s", "" );
  DUF_PRINTF( 0, "  run   -RidDnE  /mnt/new_media/media/photo		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140718 ===================" );
  DUF_PRINTF( 0, "  run   -OPRidDnf5E  /mnt/new_media/media/photo		- %s", "" );


  DUF_PRINTF( 0, "========================= as for 20140818 ===================" );
  DUF_PRINTF( 0, "  run   --help-short 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help       		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-help   		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-system 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-refer		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-collect		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-scan   		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-update 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-request		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-print  		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-nodesc 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-examples		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140828 ===================" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/ -iPO --progress  --md5 --sd5 --crc32 --mime --exif --dirs --files --dirent --filenames --filedata --add-path --create-tables --drop-tables --remove-database --vacuum  --print --integrity --recursive --info  --disable-calculate  --disable-update --summary --version		- %s",
              "" );
  DUF_PRINTF( 0, "  run  -OPRdEifndD -532Xe /mnt/new_media/media/photo/ --trace-options		- %s", "" );
  DUF_PRINTF( 0, "  run --print -Rdf --max-depth=2   /mnt/new_media/media/photo/		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140831 ===================" );

  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/dvd/benq/benq/  -Rpdf --min-size=600000 --min-same=6 --md5id=18468		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf --inode=61080661		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf --nameid=10000		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf --size=181684411		- %s", "" );
  DUF_PRINTF( 0, "  run  -OPRdEifndD -523Xe /mnt/new_media/media/photo/		- %s", "" );


  DUF_PRINTF( 0, "========================= as for 20140901 ===================" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/  -Rpdf --format=human=0,mtime,nameid=0,dirid=0,inode=0,md5=0 "
              "--min-mtime=\"'1999-01-01 00:00:00'\" --max-mtime=\"'2002-08-2 00:00:00'\" 		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140904 ===================" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf --same --glob='*06-unknown.jpeg' 		- %s", "" );
  DUF_PRINTF( 0, "  shn g --memusage  /mnt/new_media/media/photo/  -Rpdf --same --glob='*06-unknown.jpeg' 		- %s", "" );
  DUF_PRINTF( 0, "  run -OPREif -X /mnt/new_media/media/photo/ --progress 		- %s", "" );


  DUF_PRINTF( 0, "========================= as for 20140905.111356 ============" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/   -Rpdf  --output-level=6 "
              " --same-as=/mnt/new_media/media/photo/Pictures/photos/sel/we/we9.jpg 		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140907.070802 ============" );
  DUF_PRINTF( 0, "  run -OPRdEi /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s", "collect dirs from filesystem only" );
  DUF_PRINTF( 0, "  run -OPRdEin /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s",
              "collect dirs and filenames from filesystem only; no file info like permissions, size etc." );
  DUF_PRINTF( 0, "  run -OPRdEiD /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s",
              "collect dirs and file data from filesystem only; no file names" );
  DUF_PRINTF( 0, "  run -OPRdEinD /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s", "collect dirs and files from filesystem only" );
  DUF_PRINTF( 0, "  run -OPRdDnEi -f5 /mnt/new_media/media/photo/Pictures/photos/ --progress	- %s",
              "collect dirs and files from filesystem, count md5" );
  DUF_PRINTF( 0, "  run -OPRdDnEi  -f523Xe --progress /mnt/new_media/media/photo/Pictures/photos/	- %s", "collect everything we can" );

  DUF_PRINTF( 0, "========================= as for 20150124.154800 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -RDdnEiX235fOe   --show-sccbs --nosccbs	- %s", "dbg ..." );
  DUF_PRINTF( 0, "=============================================================" );

  DEBUG_END(  );
}

void
duf_option_version( int argc, char *const *argv )
{
  extern int __MAS_LINK_DATE__, __MAS_LINK_TIME__;
  char *sargv1, *sargv2;

  DEBUG_START(  );

  sargv1 = mas_argv_string( argc, argv, 1 );
  sargv2 = duf_restore_some_options( argv[0] );

  DUF_PRINTF( 0, "CFLAGS:          (%s)", MAS_CFLAGS );
  DUF_PRINTF( 0, "LDFLAGS:         (%s)", MAS_LDFLAGS );
  DUF_PRINTF( 0, "configire        (%s)", MAS_CONFIG_ARGS );
  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "UUID             %s", MAS_UUID );

  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "prefix    [%2lu]   %s", sizeof( MAS_CONFIG_PREFIX ), MAS_CONFIG_PREFIX );
  DUF_PRINTF( 0, "C version:[%2lu]   %lu", sizeof( __STDC_VERSION__ ), __STDC_VERSION__ );
  DUF_PRINTF( 0, "O.        [%2lu]   %s", sizeof( MAS_OSVER ), MAS_OSVER );
  DUF_PRINTF( 0, "U.        [%2lu]   %s", sizeof( MAS_UNAME ), MAS_UNAME );
  DUF_PRINTF( 0, "V.        [%2lu]   %s", sizeof( PACKAGE_STRING ), PACKAGE_STRING );
  DUF_PRINTF( 0, "d.        [%2lu]   %s", sizeof( MAS_C_DATE ), MAS_C_DATE );
  DUF_PRINTF( 0, "Link d.   [%lu+%lu]  %lx.%06lx", sizeof( ( unsigned long ) & __MAS_LINK_DATE__ ),
              sizeof( ( unsigned long ) & __MAS_LINK_TIME__ ), ( unsigned long ) &__MAS_LINK_DATE__, ( unsigned long ) &__MAS_LINK_TIME__ );
  DUF_PRINTF( 0, "DATE/TIME          %s/%s", __DATE__, __TIME__ );
#ifdef MAS_SPLIT_DB
  DUF_PRINTF( 0, "MAS_SPLIT_DB" );
#endif

  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "args:            (%s)", sargv1 );
  DUF_PRINTF( 0, "restored opts:   (%s)", sargv2 );

  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "config from %s ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", duf_config->config_path );
  DUF_PRINTF( 0, "cli.      [%2lu]   %x", sizeof( duf_config->cli.v.sbit ), duf_config->cli.v.sbit );
  DUF_PRINTF( 0, "pu->      [%2lu]   %x", sizeof( duf_config->pu->v.sbit ), duf_config->pu->v.sbit );
  mas_free( sargv2 );
  mas_free( sargv1 );
  DEBUG_END(  );
}

void
duf_option_showflags( int argc, char *const *argv )
{
  DEBUG_START(  );
  {
    unsigned u = duf_config->cli.act.v.bit;

    DUF_PRINTF( 0, "cli.act   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->cli.act.v ), sizeof( typeof( u ) ),
                ( unsigned long ) duf_config->cli.act.v.bit );



      /* *INDENT-OFF*  */
    DUF_PRINTF( 0, "              ┌─  %s", DUF_OPT_FLAG_NAME( DO_SCCBS ) );
    DUF_PRINTF( 0, "              │   ┌─  %s", DUF_OPT_FLAG_NAME( INTERACTIVE ) );
    DUF_PRINTF( 0, "              │   │   ┌─  %s", DUF_OPT_FLAG_NAME( PROGRESS ) );
    DUF_PRINTF( 0, "              │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( COLLECT ) );
    DUF_PRINTF( 0, "              │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( FILENAMES ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( DIRENT ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( DIRS ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( EXIF ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( CRC32 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( SD5 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( PRINT ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", DUF_OPT_FLAG_NAME( INFO ) );
    DUF_PRINTF( 0, "   ┌──────────┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴─┐" );
      /* *INDENT-ON*  */


    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".   │" );
    for ( int i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%s ", u & mask ? "◆" : " " );
      u <<= 1;
    }
    DUF_PRINTF( 0, "│" );
      /* *INDENT-OFF*  */
    DUF_PRINTF( 0, "   └────────────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┘" );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( VACUUM ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( ADD_PATH ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( TREE ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( MD5 ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( MIME ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( MDPATH ) );
    DUF_PRINTF( 0, "                │   │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( FILES ) );
    DUF_PRINTF( 0, "                │   │   │   │   └─ %s", DUF_OPT_FLAG_NAME( FILEDATA ) );
    DUF_PRINTF( 0, "                │   │   │   └─ %s", DUF_OPT_FLAG_NAME( INTEGRITY ) );
    DUF_PRINTF( 0, "                │   │   └─ %s", DUF_OPT_FLAG_NAME( UNI_SCAN ) );
    DUF_PRINTF( 0, "                │   └─ %s", DUF_OPT_FLAG_NAME( SUMMARY ) );
    DUF_PRINTF( 0, "                └─ %s", DUF_OPT_FLAG_NAME( BEGINNING_SQL ) );
      /* *INDENT-ON*  */
  }
  DUF_PRINTF( 0, ">>> %lx", ( ( unsigned long ) 1 ) << ( ( sizeof( unsigned long ) * 8 ) - 1 ) );

  {
    unsigned u = duf_config->pu->v.sbit;

    DUF_PRINTF( 0, "u   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->pu->v ), sizeof( typeof( u ) ), ( unsigned long ) duf_config->pu->v.sbit );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( int i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
  DUF_PRINTF( 0, "                                                                  └─ --recursive" );

  {
    unsigned u = duf_config->cli.disable.sbit;

    DUF_PRINTF( 0, "cli.disable   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->cli.disable ), sizeof( typeof( u ) ),
                ( unsigned long ) duf_config->cli.disable.sbit );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( int i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
  DUF_PRINTF( 0, "                                                              │ │ └─ --disable-calculate" );
  DUF_PRINTF( 0, "                                                              │ └─ --disable-insert" );
  DUF_PRINTF( 0, "                                                              └─ --disable-update" );

  DEBUG_END(  );
}

void
duf_list_sccb( int x )
{
  for ( duf_action_table_t * act = duf_action_table(  ); act->sccb; act++ )
  {
    DUF_PRINTF( 0, "* %s", duf_uni_scan_action_title( act->sccb ) );
  }
}

void
duf_list_targ( int *ptargc, char ***ptargv, long n )
{
  if ( ptargc && ptargv )
  {
    int targc = *ptargc;
    char **targv = *ptargv;

    if ( targc && targv )
      for ( int ia = 0; ia < targc; ia++ )
      {
        DUF_PRINTF( 0, "%s %d. %s", n == ia ? "*" : " ", ia, targv[ia] );
      }
  }
}

void
duf_clear_targ( int *ptargc, char ***ptargv, long n )
{
  if ( ptargc && ptargv && n == 0 )
    *ptargc = mas_argv_delete( *ptargc, *ptargv );
}

void
duf_add_targ( int *ptargc, char ***ptargv, const char *s )
{
  if ( ptargc && ptargv )
    *ptargc = mas_add_argv_arg( *ptargc, ptargv, s );
}


#if 0
static int
duf_test_help( int argc, char **argv, duf_option_class_t oclass )
{
  int r = -1;

  assert( 0 );
  /* duf_option_description( longindex ); */

  /* DUF_PRINTF( 0, "Ooooo: %d",  oclass ); */
  /* DUF_PRINTF( 0, "%d / %c => OC:%d (?%d)", opt, opt > ' ' && opt <= 'z' ? opt : '?', oclass, DUF_OPTION_CLASS_ALL ); */
  if ( oclass == DUF_OPTION_CLASS_ALL )
  {
    for ( duf_option_class_t oc = DUF_OPTION_CLASS_MIN + 1; oc < DUF_OPTION_CLASS_MAX; oc++ )
      duf_option_smart_help( oc );
    r = 0;
  }
  else if (  /* oclass != DUF_OPTION_CLASS_NO_HELP && */ oclass != DUF_OPTION_CLASS_BAD )
  {
    duf_option_smart_help( oclass );
    r = 0;
  }
  else
  {
    /* switch ( oclass )                    */
    /* {                                    */
    /* case DUF_OPTION_CLASS_VERSION:       */
    /*   duf_option_version( argc, argv );  */
    /*   r = 0;                             */
    /*   break;                             */
    /* case DUF_OPTION_CLASS_HELP:          */
    /*   duf_option_help( argc, argv );     */
    /*   r = 0;                             */
    /*   break;                             */
    /* case DUF_OPTION_CLASS_EXAMPLES:      */
    /*   duf_option_examples( argc, argv ); */
    /*   r = 0;                             */
    /*   break;                             */
    /* default:                             */
    /*   break;                             */
    /* }                                    */
  }
  return r;
}
#endif
