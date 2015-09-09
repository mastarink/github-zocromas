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

#if 0
#  define DUF_H2C( codename, val ) case DUF_OPTION_HELP_ ## codename: val=DUF_OPTION_CLASS_ ## codename;
/*
 * if arg is help option
 * return class id for options to display the help
 * */
static duf_option_class_t __attribute__ ( ( unused ) ) duf_help_option2class( duf_option_code_t codeval )
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
#endif
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
duf_option_$_smart_help( duf_option_class_t oclass )
{
  int r$ = 0;
  int *ashown;
  size_t ss;
  int tbcount;

  tbcount = duf_longindex_extended_count( lo_extended_table_multi );
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
  for ( int ilong = 0; DUF_NOERROR( r$ ) && duf_config->longopts_table[ilong].name && ilong < tbcount; ilong++ )
  {

    duf_option_code_t codeval;
    const char *name;
    const duf_longval_extended_t *extd;
    int ie;

    name = duf_config->longopts_table[ilong].name;
    codeval = duf_config->longopts_table[ilong].val;
    /* extended = _duf_find_longval_extended( codeval ); */
    extd = duf_longindex2extended( ilong, NULL, &r$ );
    /* ie = extended ? extended - &lo_extended[0] : -1; */
    ie = ilong;
    if ( codeval && DUF_NOERROR( r$ ) )
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
            char *s = NULL;

            /* duf_option_class_t hclass; */

            /* hclass = duf_help_option2class( codeval ); */
            s = duf_option_description_d( ilong, "\t", " // ", &r$ );
            DUF_TEST_R( r$ );
            /* s = mas_strcat_x( s, " ...................." ); */
            if ( s )
            {
              /* if ( shown >= 0 )                    */
              /*   DUF_PRINTF( 0, " ## %d;", shown ); */

              DUF_PRINTF( 0, "%d. [%u] \t%s", ilong, duf_config->longopts_table[ilong].val, s );
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
duf_option_$_smart_help_all( duf_option_class_t oclass )
{
  if ( oclass == DUF_OPTION_CLASS_ALL )
  {
    for ( duf_option_class_t oc = DUF_OPTION_CLASS_MIN + 1; oc < DUF_OPTION_CLASS_MAX; oc++ )
    {
      duf_option_$_smart_help( oc );
    }
  }
}

void
duf_option_$_help(  /* int argc, char *const *argv */ void )
{
  int r$ = 0;

  DUF_PRINTF( 0, "Usage: %s [OPTION]... [PATH]...", duf_config->carg.argv[0] );
  DUF_PRINTF( 0, "  -h, --help			[%s]", duf_find_longval_help( DUF_OPTION_VAL_HELP, &r$ ) );
  DUF_PRINTF( 0, "  -x, --example			[%s]", duf_find_longval_help( DUF_OPTION_VAL_EXAMPLES, &r$ ) );
  DUF_PRINTF( 0, "  --output-level		[%s]", duf_find_longval_help( DUF_OPTION_VAL_OUTPUT_LEVEL, &r$ ) );
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
  DUF_PRINTF( 0, "  -C, --trace-temporary=%d", duf_config->cli.trace.temporary );
  DUF_PRINTF( 0, "  -S, --trace-scan=%d", duf_config->cli.trace.scan );
  DUF_PRINTF( 0, "  -Q, --trace-sql=%d", duf_config->cli.trace.sql );
  DUF_PRINTF( 0, "  --trace-sample=%d", duf_config->cli.trace.sample );
  DUF_PRINTF( 0, "  --trace-path=%d", duf_config->cli.trace.path );
  DUF_PRINTF( 0, "  -F, --trace-collect=%d", duf_config->cli.trace.collect );
  DUF_PRINTF( 0, "----------------" );
  DUF_TEST_R( r$ );
}

void
duf_option_$_examples(  /* int argc, char *const *argv */ void )
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
  DUF_PRINTF( 0, "========================= as for 20150128.161052 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/ -P --trace-path=3	- %s", "dbg, see human/screen-20150128.160747.png" );


  DUF_PRINTF( 0, "========================= as for 20150506.153411 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --min-size=300M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --min-size=200M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=200M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=20M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=2M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=2K -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=2k -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=1k -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=1k --min-size=1 -T > `datemt`.tt	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150509.152057 ============" );
  DUF_PRINTF( 0, "  run /home/mastar/.mas/lib/mpd/music/  --remove-database  -PRDdnEiX235fOep --progres    	- %s", "" );
  DUF_PRINTF( 0, "  run /home/mastar/.mas/lib/mpd/music/   -Rpdf     --min-same=10 --output-level=4    	- %s", "" );
  DUF_PRINTF( 0, "  run /home/mastar/.mas/lib/mpd/music/   -Rpdf     --same=5 -T    	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150517.113041 ============" );
  DUF_PRINTF( 0, "  run -pdf /mnt/new_media/media/audio/music/best/world/  --max-seq=30 --min-same=9    -R    	- %s", "" );
  DUF_PRINTF( 0, "  run --memusa --flags    	- %s", "" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/     --min-exifdt=20120101 --max-exifdt=20130101  -dfR  --evaluate-sccb=listing    	- %s", "" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --list-sccbs    	- %s", "" );


  DUF_PRINTF( 0, "========================= as for 20150520.114602 ============" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO -dEi   	- %s",
              "insert direct child dirs into db; " " without -i will insert only 'upper' (containted in the path)" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO  --evaluate-sccb=dirs    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO -dEiR   	- %s", "insert directly, recursively dirs into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO -R --evaluate-sccb=dirs  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.121030 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO  -dEiD   	- %s",
              "(-D is for filedata) insert direct child dirs,filedata into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO  --evaluate-sccb=dirs --evaluate-sccb=filedata  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.122922 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO -dEiDn   	- %s",
              "(-n if for filenames) insert direct child dirs,filedata,filenames into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO --evaluate-sccb=dirs,filedata,filenames  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.122922 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO -dEiDn3	- %s",
              "(-3) insert direct child dirs,filedata,filenames,crc32 into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO --evaluate-sccb=dirs,filedata,filenames,crc32	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.175243 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fi3ER	- %s", "(-3) add crc32 data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=crc32    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.175243 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fi2ER	- %s", "(-2) add sd5 data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=sd5    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.220258 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --trace-mod -f --evaluate-sccb=dummy   	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150521.082012 ============" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fieER	- %s", "(-e) add mime data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=mime   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150521.082052 ============" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fiXER	- %s", "(-X) add exif data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=exif   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --remove-database -PO --progress -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif	- %s", "all from scratch..." );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/Pictures/one/ --remove-database -PO --progress -fR "
              " --evaluate-sccb=dirs,filedata,filenames,md5,mime,exif   	- %s", "for faster testing use this path" );

  DUF_PRINTF( 0, "========================= as for 20150522.100438 ============" );
  DUF_PRINTF( 0, "  run  --db-name=temp.db.big20150522.124517 /home/mastar/.maslib/firefox/scrapbook -dfR "
              " --min-same=900 --max-same=3000  --evaluate-sccb=listing    	- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20150526.150510 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/    -dfR -p"
              " --format-dirs-list='-%%_%%f%%_------%%_%%r\\n'"
              " --format-files-list='%%-4S%%5_%%f%%4s%%8z%%4_%%m%%_%%tm%%_/%%_%%T\\n'    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/    -dfR -T" " --glob=IMAG1248.jpg "
              " --format-dirs-tree='%%f\\n' " " --format-files-tree='%%f%%4s%%8z%%4_%%m%%_%%tm%%_/%%_%%T\\n' "
              " --format-prefix-files-tree='%%2_#%%-6M%%6_%%P'    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -fR -p --glob=IMAG1248.jpg "
              " --format-files-list='%%2_=%%-4S%%5_%%5z%%4_%%m%%_%%tm%%_/%%_%%T%%_%%r%%f\\n'    	- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20150528.122815 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf --same --glob-db='*506-unknown.jpeg'    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/    -dfR -T --min-same=10 --exclude-db=Picasa.ini    	- %s", "" );
  DUF_PRINTF( 0, "  run / --same-as-md5=/mnt/new_media/media/photo/Фото0829.jpg  -Rdfp    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf --same-md5 --glob-db='*506-unknown.jpeg'    	- %s", "" );
  DUF_PRINTF( 0, "  run --memusage  --nosccbs --vacuum --flags    	- %s", "" );


  DUF_PRINTF( 0, "========================= as for 20150529.151033 ============" );
  DUF_PRINTF( 0, "  run --progress  -F     /mnt/new_media/media/photo/ --remove-database -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif    	- %s", "" );
  DUF_PRINTF( 0, "  run --progress  --fast /mnt/new_media/media/photo/ --remove-database -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif    	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150603.145115 ============" );
  DUF_PRINTF( 0, "  run --mem --trace-temporary --max-dirfiles=17 --max-dirs=234 --max-files=543 --max-items=951 "
              " --fast  --min-dirfiles=13 --include-fs='*.jpg'  --include-fs='*.png' --exclude-fs='*.avi' --exclude-fs='*.mp4'    	- %s",
              "debug to show some data (temporary); argv-typed options --(in|ex)clude-fs=" );
  DUF_PRINTF( 0, "  run  --memusa --max-rel-depth=? --max-depth=? --output-level=5 --output-level=?    	- %s",
              "show values (? instead of value; esp. for interactive)" );

  DUF_PRINTF( 0, "  run  --open-db --cd=/mnt/new_media/media/photo/     -dfR -p   --md5id=857    	- %s", "" );
  DUF_PRINTF( 0, "  run                 /mnt/new_media/media/photo/     -dfR -p   --md5id=857    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20150604.115535 ============" );
  DUF_PRINTF( 0, "  run -df --interac    	- %s", "\n	tree\n	recursive\n	tree\n	no-recursive\n	tree\n" );

  DUF_PRINTF( 0, "  run --remove-database -O -P /mnt/new_media/media/photo/Pictures/one/    	- %s",
              "remove old and create new DB, add path to DB; -O, -P, --remove-database are flags!" );
  DUF_PRINTF( 0, "  run --remove-database /mnt/new_media/media/photo/Pictures/one/ -OP    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/Pictures/one/ -OP --remove-database    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20150806.154601 ============" );
  DUF_PRINTF( 0, "  ( echo open-db ; echo \"cd=`pwd`/..\" ; echo 'evaluate-sccb=tree' ) | run -dfR   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR < test.cmds    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "     test.cmds:  	- %s  \n"
              "\t\topen-db\n" "\t\tcd=/home/mastar/big/misc/develop/autotools/zoc/projects/bins/zocromas_duf\n"
              "\t\tevaluate-sccb=tree\n", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150807.095509 ============" );
  DUF_PRINTF( 0, "  MSH_DUF_OPTIONS=trace-temp=-3 run  -dfR @tist.cmd  `pwd`/human @test.cmds --tree @tost.cmd   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  MSH_DUF_OPTIONS=trace-temp=1 run @test.cmds   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  cd human && MSH_DUF_OPTIONS=trace-temp=1 run @../test.cmds"
              " --same-as=/home/mastar/big/misc/develop/autotools/zoc/projects/bins/zocromas_duf/test/duf/20150602/out20150602.134103.test.1   	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  cd human && MSH_DUF_OPTIONS=trace-temp=1 run @../test.cmds"
              " --same-as=../test/duf/20150602/out20150602.134103.test.1   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150809.163906 ============" );
  DUF_PRINTF( 0, "  rm 1.tmp ; run  @scan-here.cmds   /home/mastar/tmp . --output-file=1.tmp ; l 1.tmp   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150812.171929 ============" );
  DUF_PRINTF( 0, "  run  -dR --cd=human --tree .auxdir/ human/   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run @test.cmds  /   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  rm  $MSHDIRS_DIR/lib/duf_db/temp.db ; shn m i r  --trace-path=0 --trace-temp=0 --cd=.auxdir/ @scan-here.cmds  human   	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0,
              "  run --db-name=photo.20150812.124329.db   -dfR --lsf "
              " --same-as=/home/mastar/big/misc/media/photo/dvd/benq/benq/saltov/imag0154.jpg /   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.20150812.124329.db -dfR --glob='*.avi' --min-same=2 --tree /   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150820.205622 ============" );
  DUF_PRINTF( 0, "  run --trace-explain=2 --memusage  -dfR --trace-fs=0  --trace-mod=2 " " --include-fs='.*.sw*'  --evaluate-sccb=dummy .   	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run   --trace-scan=0 --trace-fs=0  --drop-tables  -dfR --max-depth=3 "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress .   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --disable-memusage  -dfR  --tree  --max-depth=2 .   	- %s", "new opt: --disable-memusage; --memusage removed" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db /home/mastar/big/misc/media/photo  "
              " -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif  " " --progress   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20150826.112235 ============" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --md5id=854 --disable-fs /home/mastar/big/misc/media/photo  --tree    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --md5id=854 --disable-fs /home/mastar/big/misc/media/photo  --tree "
              " --tag-file=ch-IMAG3115  --evaluate-sccb=tagit   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --with-tag-file=ch-IMAG3115 /home/mastar/big/misc/media/photo  --tree   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --files-tagged-as='ch-IMAG3115' /home/mastar/big/misc/media/photo --tree   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  /home/mastar/big/misc/media/photo/LG_G3/ "
              " --min-same=2 --tag-file=at-LG_G3  --evaluate-sccb=tagit   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR   --files-tagged-as='at-LG_G3' / --tree   	- %s", "-= \"\" =-" );


  DUF_PRINTF( 0, "  run  --db-name=htc_20101224.db  --drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress /home/mastar/big/misc/media/photo/Pictures/HTC_Legend/20101224  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=lg_g3.db  --drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress /home/mastar/big/misc/media/photo/LG_G3/  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=htc.db  --drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress /home/mastar/big/misc/media/photo/Pictures/HTC_Legend/  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR   --files-tagged-as='at-LG_G3' --with-mime-type=application/octet-stream / --ls  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0  --db-name=photo.db -dfR   --files-tagged-as='at-LG_G3' --files-tagged-as='ch-IMAG3115'  / --ls  	- %s",
              "allowed multiple --files-tagged-as options" );

  DUF_PRINTF( 0, "========================= as for 20150827.165852 ============" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='LG-D855'   /home/mastar/big/misc/media/photo --ls  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='LG-'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='EZX'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='KODAK'   /home/mastar/big/misc/media/photo --ls  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='C433'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='2110'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150829.201454 ============" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db --md5id=854  -dfR --use-format=2 /  --tree  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150830.125604 ============" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --min-size=10k --min-same=4 "
              " --with-mime-type=image/jpeg  --save-dir=/tmp --evaluate-sccb=save_to /  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --with-mime-type=image/jpeg --min-size=10k --min-same=4 "
              " --with-camera=2110  --save-dir=/tmp --evaluate-sccb=save_to --max-seq=30 /  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --with-mime-type=image/jpeg --min-size=10k --min-same=4 --with-camera=2110 "
              " --save-dir=/tmp/%%a/%%Tx{%%Y/%%B/%%d/%%H%%M%%S}/%%f --evaluate-sccb=save_to /  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --with-mime-type=image/jpeg --with-camera=D855 "
              " --save-dir=/home/mastar/big/misc/media/photo/save/%%Tx{%%Y}/%%a/%%Tx{%%B/%%d/%%H%%M%%S}/%%f --evaluate-sccb=save_to /  	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --trace-mod=2 --with-mime-type=image/jpeg --with-camera=2110 "
              " --save-dir=save/%%Tx{%%Y}/%%a/%%Tx{%%B/%%d/%%H%%M%%S}/%%M --evaluate-sccb=save_to /home/mastar/big/misc/media/photo  	- %s",
              "-= \"\" =-" );


  DUF_PRINTF( 0, "========================= as for 20150902.181551 ============" );
  DUF_PRINTF( 0, "  run  --db-name=200105.db  --drop-tables  -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif "
              " --progress  /home/mastar/big/misc/media/photo/dvd/dvd1/200105  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=200105.db  -dfR  --with-mime=image/jpeg --ls /home/mastar/big/misc/media/photo/dvd/dvd1/200105  	- %s",
              "-= \"\" =-" );


  DUF_PRINTF( 0, "========================= as for 20150908.155827 ============" );
  DUF_PRINTF( 0, "  run  --trace-path=1 -dfR --trace-options=2 --output-file=/dev/pts/30 --min-same=5 "
              " --cd=/home/mastar/big/ --tree /home/mastar/big/misc/develop/autotools/zoc/projects/ /home/mastar/big/misc 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  rm aaa ; run  --trace-file=aaa  --trace-path=0 -dfR --trace-options=2  --output-file=/dev/pts/30  --min-same=5 "
              " --cd=/home/mastar/big/misc/ --tree ; date > /dev/pts/30 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "=============================================================" );

  DEBUG_END(  );
}

void
duf_option_$_version( void )
{
  extern int __MAS_LINK_DATE__, __MAS_LINK_TIME__;
  char *sargv1, *sargv2;

  DEBUG_START(  );

  sargv1 = mas_argv_string( duf_config->carg.argc, duf_config->carg.argv, 1 );
  sargv2 = duf_restore_some_options( duf_config->carg.argv[0] );

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

duf_option_code_t
duf_flag2code( duf_config_act_flags_combo_t fset )
{
  duf_option_code_t rc = DUF_OPTION_VAL_NONE;

#define CHECK_FLAG_ID_ROW(_l, _u) {{.flag._l = 1}, #_l, DUF_OPTION_VAL_FLAG_ ## _u}
  duf_chk_act_flags_t tab[] DUF_UNUSED = {
    CHECK_FLAG_ID_ROW( info, INFO ),
    CHECK_FLAG_ID_ROW( vacuum, VACUUM ),
    CHECK_FLAG_ID_ROW( remove_database, REMOVE_DATABASE ),
    CHECK_FLAG_ID_ROW( drop_tables, DROP_TABLES ),
    CHECK_FLAG_ID_ROW( vacuum, VACUUM ),
    CHECK_FLAG_ID_ROW( remove_database, REMOVE_DATABASE ),
    CHECK_FLAG_ID_ROW( drop_tables, DROP_TABLES ),
    CHECK_FLAG_ID_ROW( create_tables, CREATE_TABLES ),
    CHECK_FLAG_ID_ROW( add_path, ADD_PATH ),

    /* CHECK_FLAG_ID_ROW( sd5_obs, SD5 ),     */
    /* CHECK_FLAG_ID_ROW( md5_obs, MD5 ),     */
    /* CHECK_FLAG_ID_ROW( crc32_obs, CRC32 ), */
    /* CHECK_FLAG_ID_ROW( mime_obs, MIME ),   */
    /* CHECK_FLAG_ID_ROW( exif_obs, EXIF ),   */

    CHECK_FLAG_ID_ROW( mdpath_obs, MDPATH ),

    /* 20150907.122929 renamed  FILES -> ALLOW_FILES */
    CHECK_FLAG_ID_ROW( allow_dirs, ALLOW_DIRS ),
    /* 20150907.122929 renamed  DIRS -> ALLOW_DIRS */
    CHECK_FLAG_ID_ROW( allow_files, ALLOW_FILES ),

    /* CHECK_FLAG_ID_ROW( dirent, DIRENT ), */
    /* CHECK_FLAG_ID_ROW( filedata, FILEDATA ), */
    /* CHECK_FLAG_ID_ROW( filenames, FILENAMES ), */

    CHECK_FLAG_ID_ROW( integrity, INTEGRITY ),
    CHECK_FLAG_ID_ROW( collect_obs, COLLECT ),

    CHECK_FLAG_ID_ROW( progress, PROGRESS ),
    CHECK_FLAG_ID_ROW( use_binformat, USE_BINFORMAT ),

    CHECK_FLAG_ID_ROW( summary, SUMMARY ),
    CHECK_FLAG_ID_ROW( interactive, INTERACTIVE ),
    CHECK_FLAG_ID_ROW( do_sccbs, DO_SCCBS ),
    CHECK_FLAG_ID_ROW( fast, FAST ),
  };
#if 0
  for ( typeof( fset.bit ) uf = 1; uf != 0; uf = uf << 1 )
  {
    duf_config_act_flags_combo_t tst;

    tst.bit = uf;
    /* if ( tst.bit == fset.bit ) */
    {
      DUF_PRINTF( 0, "%lx/%lx - %lx", ( unsigned long ) uf, ( unsigned long ) tst.bit, ( unsigned long ) fset.bit );
    }
  }
#endif
  for ( int i = 0; i < sizeof( tab ) / sizeof( tab[0] ); i++ )
  {
    if ( tab[i].test.bit == fset.bit )
      rc = tab[i].id;
  }
  return rc;
}

duf_option_code_t
duf_uflag2code( unsigned long ufset )
{
  duf_config_act_flags_combo_t fset;
  duf_option_code_t id = DUF_OPTION_VAL_NONE;

  fset.bit = ufset;
  id = duf_flag2code( fset );
  return id;
}

const char *
duf_uflag2cnames( unsigned long ufset )
{
  duf_option_code_t id = DUF_OPTION_VAL_NONE;

  id = duf_uflag2code( ufset );
  return id == DUF_OPTION_VAL_NONE ? "" : duf_option_cnames_tmp( -1, id, NULL );
}

const char *
duf_unflag2cnames( unsigned unfset )
{
  return duf_uflag2cnames( 1 << unfset );
}

void
duf_option_$_showflags(  /* int argc, char *const *argv */ void )
{
  DEBUG_START(  );
  {
    unsigned u = duf_config->cli.act.v.bit;

    DUF_PRINTF( 0, "cli.act   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->cli.act.v ), sizeof( typeof( u ) ),
                ( unsigned long ) duf_config->cli.act.v.bit );


      /* *INDENT-OFF*  */
    DUF_PRINTF( 0, "              ┌─  %s", duf_unflag2cnames( 26 ) );
    DUF_PRINTF( 0, "              │   ┌─  %s", duf_unflag2cnames( 24 ) );
    DUF_PRINTF( 0, "              │   │   ┌─  %s", duf_unflag2cnames( 22 ) );
    DUF_PRINTF( 0, "              │   │   │   ┌─  %s", duf_unflag2cnames( 20 ) );
    DUF_PRINTF( 0, "              │   │   │   │   ┌─  %s", duf_unflag2cnames( 18 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 16 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 14 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 12 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 10 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 8 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 6 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 4 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 2 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 0 ) );
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
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 1) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 3) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 5) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 7) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 9) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 11) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 13) );
    DUF_PRINTF( 0, "                │   │   │   │   │   └─ %s", duf_unflag2cnames( 15) );
    DUF_PRINTF( 0, "                │   │   │   │   └─ %s", duf_unflag2cnames( 17) );
    DUF_PRINTF( 0, "                │   │   │   └─ %s", duf_unflag2cnames( 19) );
    DUF_PRINTF( 0, "                │   │   └─ %s", duf_unflag2cnames( 21) );
    DUF_PRINTF( 0, "                │   └─ %s", duf_unflag2cnames( 23) );
    DUF_PRINTF( 0, "                └─ %s", duf_unflag2cnames( 25) );
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
#if 0
  {
    duf_option_code_t id = DUF_OPTION_VAL_NONE;

    duf_config_act_flags_combo_t vv = {
      .flag.progress = 1
    };
    id = duf_flag2code( vv );
    DUF_PRINTF( 0, "id:%u - %s", id, duf_option_cnames_tmp( -1, id, NULL ) );
    for ( unsigned u = 0; u < 30; u++ )
    {
      DUF_PRINTF( 0, "====================== %u. %s", u, duf_unflag2cnames( u ) );
    }
  }
#endif
  DEBUG_END(  );
}

void
duf_option_$_list_targ( int *ptargc, char ***ptargv, long n )
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
duf_option_$_clear_targ( int *ptargc, char ***ptargv, long n )
{
  if ( ptargc && ptargv && n == 0 )
    *ptargc = mas_argv_delete( *ptargc, *ptargv );
}

void
duf_option_$_add_targ( int *ptargc, char ***ptargv, const char *s )
{
  if ( ptargc && ptargv )
    *ptargc = mas_add_argv_arg( *ptargc, ptargv, s );
}

void
duf_option_$_list_options( long n )
{
  int ntable = 0;
  int tbcount = 0;

  for ( const duf_longval_extended_table_t ** xtables = lo_extended_table_multi; *xtables; xtables++, ntable++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    for ( const duf_longval_extended_t * xtended = xtable->table; xtended->o.name; xtended++, tbcount++ )
    {
      char *s = NULL;
      int r$ = 0;

      if ( xtended->o.val )
        s = duf_option_description_xd( xtended, "\t", " // ", &r$ );
      DUF_TEST_R( r$ );

      DUF_TRACE( options, 5, "@li2ex %d [%s]", ntable, xtended->o.name );
      /* DUF_PRINTF( 0, "[%ld] %3d (%2d) %4d %d:%d\t--%-40s", n, tbcount, ntable, xtended->o.val, xtended->stage.min, xtended->stage.max, xtended->o.name ); */
      DUF_PRINTF( 0, "%3d (%2d) %4d %d:%d\t--%-40s - %s", tbcount, ntable, xtended->o.val, xtended->stage.min, xtended->stage.max, xtended->o.name,
                  s ? s : "" );
      mas_free( s );
    }
  }
}
