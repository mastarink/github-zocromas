#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_utils.h"

#include "duf_config_ref.h"

#include "duf_option_defs.h"
#include "duf_option_acquire.h"
#include "duf_option_find.h"
#include "duf_option_descr.h"
#include "duf_option_extended.h"
#include "duf_option_typed.h"

/* ###################################################################### */
#include "duf_option_old.h"
/* ###################################################################### */



int
duf_parse_option_long_old( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                           const duf_longval_extended_table_t * xtable )
{
  DEBUG_STARTR( r );



  DUF_TRACE( options, 2, "(x%d) to parse %s (%s)  (%d)%s  cv:%d (F:%d)", extended ? 1 : 0, duf_option_description_x_tmp( -1, extended, NULL ),
             extended->o.name, r, duf_error_name( r ), extended->o.val, DUF_OPTION_VAL_BFORMAT );

  if ( DUF_OPTION_CHECK_STATE( istage, extended, xtable ) )
  {
    switch ( extended->o.val )
    {
    case DUF_OPTION_VAL_NONE:
      break;
    case DUF_OPTION_VAL_TEST:
      DUF_PRINTF( 0, "This is test option output; optargg:%s", optargg ? optargg : "-" );
      break;
#if 0
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXDIRFILES, /*        */ dirfiles.max /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MINDIRFILES, /*        */ dirfiles.min /*     */  );
#endif
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXSEQ, (*             *) maxseq (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXRELDEPTH, (*           *) max_rel_depth (*     *)  ); */

#if 0
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXITEMS, /*           */ maxitems.total /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXITEMS_FILES, /*     */ maxitems.files /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXITEMS_DIRS, /*      */ maxitems.dirs /*     */  );
#endif

#if 0
      DUF_OPTION_CASE_ACQUIRE_U_ARG( GLOB_INCL_FS_FILES, /* */ globx.include_fs_files /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_ARG( GLOB_EXCL_FS_FILES, /* */ globx.exclude_fs_files /*     */  );
#endif

/* i/o */

    case DUF_OPTION_VAL_TRACE_STDERR:
      if ( duf_config->cli.trace.output.out )
      {
        if ( duf_config->cli.trace.output.out != stderr && duf_config->cli.trace.output.out != stdout )
          fclose( duf_config->cli.trace.output.out );
        duf_config->cli.trace.output.out = NULL;
      }
      if ( duf_config->cli.trace.output.file )
      {
        mas_free( duf_config->cli.trace.output.file );
        duf_config->cli.trace.output.file = NULL;
      }
      duf_config->cli.trace.output.out = stderr;
      r = 0;
      break;
    case DUF_OPTION_VAL_TRACE_STDOUT:
      if ( duf_config->cli.trace.output.out )
      {
        if ( duf_config->cli.trace.output.out != stderr && duf_config->cli.trace.output.out != stdout )
          fclose( duf_config->cli.trace.output.out );
        duf_config->cli.trace.output.out = NULL;
      }
      if ( duf_config->cli.trace.output.file )
      {
        mas_free( duf_config->cli.trace.output.file );
        duf_config->cli.trace.output.file = NULL;
      }
      duf_config->cli.trace.output.out = stdout;
      r = 0;
      break;

/* combined */
    case DUF_OPTION_VAL_ALL_TRACE:
      if ( optargg && *optargg )
        duf_config->cli.trace.sql = duf_config->cli.trace.select = duf_config->cli.trace.insert = duf_config->cli.trace.update =
              duf_config->cli.trace.collect = duf_config->cli.trace.dirent = duf_config->cli.trace.sd5 = duf_config->cli.trace.md5 =
              duf_config->cli.trace.crc32 = duf_config->cli.trace.mime = duf_config->cli.trace.exif = duf_config->cli.trace.sample =
              duf_config->cli.trace.deleted = duf_config->cli.trace.scan = duf_config->cli.trace.temp = strtol( optargg, NULL, 10 );
      else
      {
        duf_config->cli.trace.sql++;
        duf_config->cli.trace.select++;
        duf_config->cli.trace.insert++;
        duf_config->cli.trace.update++;
        duf_config->cli.trace.collect++;
        duf_config->cli.trace.sd5++;
        duf_config->cli.trace.md5++;
        duf_config->cli.trace.crc32++;
        duf_config->cli.trace.mime++;
        duf_config->cli.trace.exif++;
        duf_config->cli.trace.dirent++;
        duf_config->cli.trace.sample++;
        duf_config->cli.trace.deleted++;
        duf_config->cli.trace.scan++;
        duf_config->cli.trace.temp++;
      }
      break;
#if 0
    case DUF_OPTION_VAL_FLAG_ZERO_DB:
      DUF_OPTION_ACQUIRE_FLAG( create_tables, cli.act );
/* no break here ! */
    case DUF_OPTION_VAL_TREE_TO_DB:
      /* -ORifd5 
       * i.e.
       *  --create-tables --uni-scan --recursive ...
       *  */
      DUF_ACT_FLAG( create_tables ) = /* */
            DUF_ACT_FLAG( add_path ) = /* */
            /* DUF_ACT_FLAG( uni_scan ) = (* *) */
            DUF_U_FLAG( recursive ) = /* */
            DUF_ACT_FLAG( files ) = /* */
            DUF_ACT_FLAG( dirs ) = /* */
            DUF_ACT_FLAG( dirent ) = /* */
            DUF_ACT_FLAG( sd5 ) = /* */
            DUF_ACT_FLAG( md5 ) = /* */
            DUF_ACT_FLAG( crc32 ) = /* */
            DUF_ACT_FLAG( filedata ) = /* */
            DUF_ACT_FLAG( filenames ) = /* */
            1;
      break;
#endif
/* specific */
    case DUF_OPTION_VAL_BFORMAT:
      {
        char *coptarg, *poptarg;
        unsigned nvalue;
        char *value;

        DUF_TRACE( options, 3, "really to parse %s = %s (%s)  %s", duf_option_description_x_tmp( -1, extended, NULL ), optargg, extended->o.name,
                   duf_error_name( r ) );

        /* duf_config->cli.format.seq = (* *)            */
        /*       duf_config->cli.format.dirid = (* *)    */
        /*       duf_config->cli.format.inode = (* *)    */
        /*       duf_config->cli.format.mode = (* *)     */
        /*       duf_config->cli.format.nlink = (* *)    */
        /*       duf_config->cli.format.uid = (* *)      */
        /*       duf_config->cli.format.gid = (* *)      */
        /*       duf_config->cli.format.prefix = (* *)   */
        /*       duf_config->cli.format.suffix = (* *)   */
        /*       duf_config->cli.format.filesize = (* *) */
        /*       duf_config->cli.format.mtime = (* *)    */
        /*       duf_config->cli.format.filename = (* *) */
        /*       duf_config->cli.format.md5 = (* *)      */
        /*       duf_config->cli.format.md5id = (* *)    */
        /*       duf_config->cli.format.nsame = 0;       */


        char *const tokens[] = {
          [DUF_FORMAT_DATAID] = "dataid",
          [DUF_FORMAT_DIRID] = "dirid",
          [DUF_FORMAT_DIRID_SPACE] = "dirid_space",
          [DUF_FORMAT_NFILES] = "nfiles",
          [DUF_FORMAT_NFILES_SPACE] = "nfiles_space",
          [DUF_FORMAT_NDIRS] = "ndirs",
          [DUF_FORMAT_NDIRS_SPACE] = "ndirs_space",
          [DUF_FORMAT_FILENAME] = "filename",
          [DUF_FORMAT_FILESIZE] = "filesize",
          [DUF_FORMAT_HUMAN] = "human",
          [DUF_FORMAT_INODE] = "inode",
          [DUF_FORMAT_SD5ID] = "sd5id",
          [DUF_FORMAT_SD5] = "sd5",
          [DUF_FORMAT_MD5ID] = "md5id",
          [DUF_FORMAT_MD5] = "md5",
          [DUF_FORMAT_CRC32ID] = "crc32id",
          [DUF_FORMAT_CRC32] = "crc32",
          [DUF_FORMAT_NAMEID] = "nameid",
          [DUF_FORMAT_MIME] = "mime",
          [DUF_FORMAT_MIMEID] = "mimeid",
          [DUF_FORMAT_EXIFID] = "exifid",
          [DUF_FORMAT_EXIFDT] = "exifdt",
          [DUF_FORMAT_MODE] = "mode",
          [DUF_FORMAT_MTIME] = "mtime",
          [DUF_FORMAT_NLINK] = "nlink",
          [DUF_FORMAT_NSAME] = "nsame",
          [DUF_FORMAT_OFFSET] = "offset",
          [DUF_FORMAT_PREFIX] = "prefix",
          [DUF_FORMAT_SUFFIX] = "suffix",
          [DUF_FORMAT_REALPATH] = "realpath",
          [DUF_FORMAT_DEPTH] = "depth",
          [DUF_FORMAT_SEQ] = "seq",
          [DUF_FORMAT_SEQ_NODE] = "seq-node",
          [DUF_FORMAT_SEQ_LEAF] = "seq-leaf",[DUF_FORMAT_GROUP] = "group",[DUF_FORMAT_USER] = "user",[DUF_FORMAT_MAX] = NULL,
        };
        poptarg = coptarg = mas_strdup( optargg );
        /* coptarg = mas_strdup( optargg ); */
        DUF_TRACE( action, 0, "--format=%s", coptarg );
        while ( poptarg && *poptarg )
        {
          char *hlp;
          int rs = 0;

          hlp = poptarg;
          DUF_TRACE( any, 0, "hlp:%s", hlp );
          value = NULL;
          DUF_TRACE( options, 2, "really to parse format item [%s]     value:%s", poptarg, value );
          rs = getsubopt( &poptarg, tokens, &value );
          DUF_TRACE( options, 2, "%d: (%s) '%s'", rs, hlp, value ? value : "nil" );
          nvalue = value ? strtol( value, NULL, 10 ) : -1;
          switch ( rs )
          {
          case DUF_FORMAT_DEPTH:
            DUF_FORMAT_FLAG_NUM( depth ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SEQ:
            DUF_FORMAT_FLAG_NUM( seq ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SEQ_NODE:
            DUF_FORMAT_FLAG_NUM( seq_node ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SEQ_LEAF:
            DUF_FORMAT_FLAG_NUM( seq_leaf ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_PREFIX:
            DUF_FORMAT_FLAG_NUM( prefix ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SUFFIX:
            DUF_FORMAT_FLAG_NUM( suffix ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DIRID:
            DUF_FORMAT_FLAG_NUM( dirid ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DIRID_SPACE:
            DUF_FORMAT_FLAG_NUM( dirid_space ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NFILES:
            DUF_FORMAT_FLAG_NUM( nfiles ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NFILES_SPACE:
            DUF_FORMAT_FLAG_NUM( nfiles_space ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NDIRS:
            DUF_FORMAT_FLAG_NUM( ndirs ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NDIRS_SPACE:
            DUF_FORMAT_FLAG_NUM( ndirs_space ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_REALPATH:
            DUF_FORMAT_FLAG_NUM( realpath ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_INODE:
            DUF_FORMAT_FLAG_NUM( inode ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MODE:
            DUF_FORMAT_FLAG_NUM( mode ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NLINK:
            DUF_FORMAT_FLAG_NUM( nlink ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_USER:
            DUF_FORMAT_FLAG_NUM( user ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_GROUP:
            DUF_FORMAT_FLAG_NUM( group ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_FILESIZE:
            DUF_FORMAT_FLAG_NUM( filesize ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MTIME:
            DUF_FORMAT_FLAG_NUM( mtime ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_FILENAME:
            DUF_FORMAT_FLAG_NUM( filename ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SD5:
            DUF_FORMAT_FLAG_NUM( sd5 ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SD5ID:
            DUF_FORMAT_FLAG_NUM( sd5id ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MD5:
            DUF_FORMAT_FLAG_NUM( md5 ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MD5ID:
            DUF_FORMAT_FLAG_NUM( md5id ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_CRC32:
            DUF_FORMAT_FLAG_NUM( crc32 ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_CRC32ID:
            DUF_FORMAT_FLAG_NUM( crc32id ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NAMEID:
            DUF_FORMAT_FLAG_NUM( nameid ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MIME:
            DUF_FORMAT_FLAG_NUM( mime ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MIMEID:
            DUF_FORMAT_FLAG_NUM( mimeid ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_EXIFID:
            DUF_FORMAT_FLAG_NUM( exifid ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_EXIFDT:
            DUF_FORMAT_FLAG_NUM( exifdt ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_HUMAN:
            DUF_FORMAT_FLAG_NUM( human ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DATAID:
            DUF_FORMAT_FLAG_NUM( dataid ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NSAME:
            DUF_FORMAT_NUM( nsame ) = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_OFFSET:
            DUF_FORMAT_NUM( offset ) = value == NULL ? 0 : nvalue;
            break;
          }
          DUF_TRACE( options, 2, "rs:%d [%s:%s:%s] FORMAT bits: %llx", rs, poptarg, hlp, value ? value : "nil", duf_config->cli.bformat.v.bit );
          /* DUF_TRACE( any, 0, "rs:%d", rs ); */
          if ( rs < 0 )
          {
            DUF_MAKE_ERROR( r, DUF_ERROR_SUBOPTION );
            DUF_TEST_R( r );
            break;
          }
          else
            r = 0;
        }
        mas_free( coptarg );
      }

      DUF_TEST_R( r );
      break;
    case DUF_OPTION_VAL_MEMUSAGE:
      {
        extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

        if ( &mas_mem_disable_print_usage )
        {
          mas_mem_disable_print_usage = 0;
        }
      }
      break;
    default:
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION );
      break;
    }
  }
  else
  {
    /* r = DUF_ERROR_OPTION_NOT_FOUND; */
  }

  DEBUG_ENDR( r );
  /* DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND ); */
}
