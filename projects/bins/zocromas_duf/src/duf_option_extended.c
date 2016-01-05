#include <string.h>
#include <stddef.h>


#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_option_stage.h"   /* duf_optstage_name */

#include "duf_xtended_table.h"

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */

static void
duf_print_stage( int use_stage, int use_stage_mask, duf_limits_stage_t stage, unsigned long stage_mask, unsigned eol )
{
  if ( use_stage || use_stage_mask )
  {
    DUF_PRINTF( 0, ".  " );
    if ( use_stage )
    {
      DUF_PRINTF( 0, ".stage(%s(%d),%s(%d)) ", duf_optstage_name( stage.min ), stage.min, duf_optstage_name( stage.max ), stage.max );
    }
    if ( use_stage_mask )
    {
      unsigned long msk = stage_mask;
      unsigned n = 0;

      DUF_PRINTF( 0, ".mask(%lu; { ", stage_mask );
      while ( msk )
      {
        if ( msk & 1 )
        {
          DUF_PRINTF( 0, ".%s; ", duf_optstage_name( n ) );
        }
        n++;
        msk = msk >> 1;
      }
      DUF_PRINTF( 0, ".} )" );
    }
    if ( eol )
      DUF_PUTSL( 0 );
  }
}

const char *
duf_extended_code2string( duf_option_code_t code )
{
  const char *rs = NULL;

  switch ( code )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAPP(_n, _rf, _rf2)       case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAP_VP(_n, _v, _rf, _rf2) case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#include "duf_options_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAPP
#undef ENUM_WRAP_V
#undef ENUM_WRAP_VP
  }
  return rs;
}

const char *
duf_extended_class2string( duf_option_class_t oclass )
{
  const char *rs = NULL;

  switch ( oclass )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_CLASS_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_CLASS_ ## _n: rs= #_n; break;
#include "duf_option_class_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

const char *
duf_extended_vtype2string( duf_option_vtype_t ovtype )
{
  const char *rs = NULL;

  switch ( ovtype )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_VTYPE_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_VTYPE_ ## _n: rs= #_n; break;
#include "duf_option_vtype_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

const char *
duf_offset2stringid( unsigned offset, duf_offset_to_t relto )
{
  const char *rs = NULL;

  if ( relto == DUF_OFFSET_config )
  {
    switch ( offset )
    {
    case offsetof( duf_config_t, help_string ):
      rs = "help_string";
      break;
    case offsetof( duf_config_t, loadtime ):
      rs = "loadtime";
      break;
    case offsetof( duf_config_t, config_dir ):
      rs = "config_dir";
      break;
    case offsetof( duf_config_t, cmds_dir ):
      rs = "cmds_dir";
      break;
    case offsetof( duf_config_t, config_file_path ):
      rs = "config_file_path";
      break;
    case offsetof( duf_config_t, dir_priority ):
      rs = "dir_priority";
      break;
    case offsetof( duf_config_t, db.dir ):
      rs = "db.dir";
      break;
    case offsetof( duf_config_t, db.subdir ):
      rs = "db.subdir";
      break;
    case offsetof( duf_config_t, db.adm.name ):
      rs = "db.adm.name";
      break;
    case offsetof( duf_config_t, db.tempo.name ):
      rs = "db.tempo.name";
      break;
    case offsetof( duf_config_t, db.selected.name ):
      rs = "db.selected.name";
      break;
    case offsetof( duf_config_t, db.main.name ):
      rs = "db.main.name";
      break;
    case offsetof( duf_config_t, tag.dir ):
      rs = "tag.dir";
      break;
    case offsetof( duf_config_t, tag.file ):
      rs = "tag.file";
      break;
    case offsetof( duf_config_t, save.path ):
      rs = "save.path";
      break;
    case offsetof( duf_config_t, opt.option_delimiter ):
      rs = "opt.option_delimiter";
      break;
    case offsetof( duf_config_t, opt.act.v ):
      rs = "opt.act.v";
      break;
    case offsetof( duf_config_t, opt.v ):
      rs = "opt.v";
      break;
    case offsetof( duf_config_t, opt.output ):
      rs = "opt.output";
      break;
    case offsetof( duf_config_t, opt.output.fun_width ):
      rs = "opt.output.fun_width";
      break;
    case offsetof( duf_config_t, opt.output.header_tty ):
      rs = "opt.output.header_tty";
      break;
    case offsetof( duf_config_t, opt.output.level ):
      rs = "opt.output.level";
      break;
    case offsetof( duf_config_t, opt.disable ):
      rs = "opt.disable";
      break;
      /* case offsetof( duf_config_t, opt.output.history_filename ): */
      /*   rs = "opt.output.history_filename";                       */
      /*   break;                                                    */

      /* case offsetof( duf_config_t, cli ):  */
      /*   break;                             */
      /* case offsetof( duf_config_t, opt ):  */
      /*   break;                             */
      /* case offsetof( duf_config_t, db ):   */
      /*   break;                             */
      /* case offsetof( duf_config_t, save ): */
      /*   break;                             */
    default:
      if ( offset > offsetof( duf_config_t, cli ) && offset < offsetof( duf_config_t, opt ) )
      {
      }
      else if ( offset >= offsetof( duf_config_t, opt ) && offset < offsetof( duf_config_t, db ) )
      {
        /* {                                                                                                 */
        /*   if ( offsetof( duf_config_opt_t, option_delimiter ) == offset - offsetof( duf_config_t, opt ) ) */
        /*     rs = "opt.option_delimiter";                                                                  */
        /* }                                                                                                 */
      }
      else if ( offset >= offsetof( duf_config_t, db ) && offset < offsetof( duf_config_t, save ) )
      {
      }
      else if ( offset >= offsetof( duf_config_t, save ) && offset < offsetof( duf_config_t, config_dir ) )
      {
      }
      else if ( offset >= offsetof( duf_config_t, scn ) && offset < offsetof( duf_config_t, dh ) )
      {
      }
      else if ( offset >= offsetof( duf_config_t, dh ) && offset < offsetof( duf_config_t, tag ) )
      {
      }
      break;
    }
  }
  else if ( relto == DUF_OFFSET_ufilter )
  {
    switch ( offset )
    {
    case offsetof( duf_ufilter_t, globx.include_fs_files ):
      rs = "globx.include_fs_files";
      break;
    case offsetof( duf_ufilter_t, globx.exclude_fs_files ):
      rs = "globx.exclude_fs_files";
      break;
    case offsetof( duf_ufilter_t, max_rel_depth ):
      rs = "max_rel_depth";
      break;
    case offsetof( duf_ufilter_t, max_seq ):
      rs = "max_seq";
      break;
    case offsetof( duf_ufilter_t, same.md5 ):
      rs = "same.md5";
      break;
    case offsetof( duf_ufilter_t, same.sha1 ):
      rs = "same.sha1";
      break;
    case offsetof( duf_ufilter_t, same.exif ):
      rs = "same.exif";
      break;
    case offsetof( duf_ufilter_t, same.mime ):
      rs = "same.mime";
      break;
    case offsetof( duf_ufilter_t, same.data ):
      rs = "same.data";
      break;
    case offsetof( duf_ufilter_t, same.size ):
      rs = "same.size";
      break;
    case offsetof( duf_ufilter_t, size ):
      rs = "size";
      break;
    case offsetof( duf_ufilter_t, nameid ):
      rs = "nameid";
      break;
    case offsetof( duf_ufilter_t, dataid ):
      rs = "dataid";
      break;
    case offsetof( duf_ufilter_t, dirid ):
      rs = "dirid";
      break;
    case offsetof( duf_ufilter_t, mtime ):
      rs = "mtime";
      break;
    case offsetof( duf_ufilter_t, atime ):
      rs = "atime";
      break;
    case offsetof( duf_ufilter_t, ctime ):
      rs = "ctime";
      break;
    case offsetof( duf_ufilter_t, exifdt ):
      rs = "exifdt";
      break;
    case offsetof( duf_ufilter_t, inode ):
      rs = "inode";
      break;
    case offsetof( duf_ufilter_t, md5id ):
      rs = "md5id";
      break;
    case offsetof( duf_ufilter_t, sha1id ):
      rs = "sha1id";
      break;
    case offsetof( duf_ufilter_t, sd5id ):
      rs = "sd5id";
      break;
    case offsetof( duf_ufilter_t, crc32id ):
      rs = "crc32id";
      break;
    case offsetof( duf_ufilter_t, mimeid ):
      rs = "mimeid";
      break;
    case offsetof( duf_ufilter_t, exifid ):
      rs = "exifid";
      break;
    case offsetof( duf_ufilter_t, filename ):
      rs = "filename";
      break;
    case offsetof( duf_ufilter_t, glob_db ):
      rs = "glob_db";
      break;
    case offsetof( duf_ufilter_t, glob_db_include ):
      rs = "glob_db_include";
      break;
    case offsetof( duf_ufilter_t, glob_db_exclude ):
      rs = "glob_db_exclude";
      break;
    case offsetof( duf_ufilter_t, tag.dir ):
      rs = "tag.dir";
      break;
    case offsetof( duf_ufilter_t, exif.camera ):
      rs = "exif.camera";
      break;
    case offsetof( duf_ufilter_t, mime.type ):
      rs = "mime.type";
      break;
    case offsetof( duf_ufilter_t, same_as.md5 ):
      rs = "same_as.md5";
      break;
    case offsetof( duf_ufilter_t, same_as.sha1 ):
      rs = "same_as.sha1";
      break;
    case offsetof( duf_ufilter_t, same_as.exif ):
      rs = "same_as.exif";
      break;
    case offsetof( duf_ufilter_t, dirfiles.min ):
      rs = "dirfiles.min";
      break;
    case offsetof( duf_ufilter_t, dirfiles.max ):
      rs = "dirfiles.max";
      break;
    case offsetof( duf_ufilter_t, maxitems.dirs ):
      rs = "maxitems.dirs";
      break;
    case offsetof( duf_ufilter_t, maxitems.files ):
      rs = "maxitems.files";
      break;
    case offsetof( duf_ufilter_t, maxitems.total ):
      rs = "maxitems.total";
      break;
    case offsetof( duf_ufilter_t, v ):
      rs = "v";
      break;
    }
  }
  return rs;
}

void
duf_extended_table_print( const duf_longval_extended_table_t * xtable, const char *name )
{
  DUF_PRINTF( 0, ".@@@ [%s]", xtable->name );
  duf_print_stage( xtable->use_stage, xtable->use_stage_mask, xtable->stage, xtable->stage_mask, 0 );
  DUF_PUTSL( 0 );

  for ( const duf_longval_extended_t * xtended = xtable->table; xtended->o.name; xtended++ )
  {
    if ( !name || strstr( xtended->o.name, name ) )
    {
      DUF_PRINTF( 0, "cmd %s", xtended->o.name );
      DUF_PRINTF( 0, "{" );
      if ( xtended->o.has_arg == 0 )
        DUF_PRINTF( 0, "  optarg: %s", "no" );
      else if ( xtended->o.has_arg == 1 )
        DUF_PRINTF( 0, "  optarg: %s", "required" );
      else if ( xtended->o.has_arg == 2 )
        DUF_PRINTF( 0, "  optarg: %s", "optional" );
      else
        DUF_PRINTF( 0, "  optarg: %s", "error" );
      if ( xtended->can_no || xtended->invert || xtended->m_hasoff )
      {
        DUF_PRINTF( 0, ".%s", "  " );
        DUF_PRINTF( 0, ".%s", xtended->can_no ? "can_no " : "" );
        DUF_PRINTF( 0, ".%s", xtended->invert ? "invert " : "" );
        if ( xtended->m_hasoff )
        {
          const char *srelto = NULL;

          switch ( xtended->relto )
          {
          case DUF_OFFSET_config:
            srelto = "config";
            break;
          case DUF_OFFSET_ufilter:
            srelto = "ufilter";
            break;
          }
          DUF_PRINTF( 0, ".offset( %lu, %s(%d), %s ) ", xtended->m_offset, srelto, xtended->relto,
                      duf_offset2stringid( xtended->m_offset, xtended->relto ) );
        }
        DUF_PUTSL( 0 );
      }
      duf_print_stage( xtended->use_stage, xtended->use_stage_mask, xtended->stage, xtended->stage_mask, 1 );
      {
        DUF_PRINTF( 0, ".%s", "  " );
        DUF_PRINTF( 0, ".code:%s(%d)", duf_extended_code2string( xtended->o.val ), xtended->o.val );
        DUF_PUTSL( 0 );
      }
      DUF_PRINTF( 0, "  class:%s(%d)", duf_extended_class2string( xtended->oclass ), xtended->oclass );
      DUF_PRINTF( 0, "  vtype:%s(%d)", duf_extended_vtype2string( xtended->vtype ), xtended->vtype );
      if ( xtended->call.funcname )
        DUF_PRINTF( 0, "  funcname:%s", xtended->call.funcname );
      switch ( xtended->vtype )
      {
      case DUF_OPTION_VTYPE_EIA_CALL:
        DUF_PRINTF( 0, "  func:%p(%d)", xtended->call.fdesc.eia.func, xtended->call.fdesc.eia.arg );
        break;
      case DUF_OPTION_VTYPE_EV_CALL:
        DUF_PRINTF( 0, "  func:%p( )", xtended->call.fdesc.ev.func );
        break;
      case DUF_OPTION_VTYPE_SAS_CALL:
        DUF_PRINTF( 0, "  func:%p(%s)", xtended->call.fdesc.sas.func, xtended->call.fdesc.sas.arg );
        break;
      case DUF_OPTION_VTYPE_SAN_CALL:
        DUF_PRINTF( 0, "  func:%p(%ld)", xtended->call.fdesc.san.func, xtended->call.fdesc.san.arg );
        break;
      case DUF_OPTION_VTYPE_A_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_carg(  )->argc,duf_cli_options_get_carg(  )->argv)", xtended->call.fdesc.a.func );
        break;
      case DUF_OPTION_VTYPE_N_CALL:
        /* int rxx DUF_UNUSED = 0; */
        /* DUF_PRINTF( 0, "  func:%p(%ld)", xtended->call.fdesc.n.func, duf_strtol_suff( optargg, &rxx ) ); */
        DUF_PRINTF( 0, "  func:%p(duf_strtol_suff( optargg, &r ))", xtended->call.fdesc.n.func );
        break;
      case DUF_OPTION_VTYPE_S_CALL:
        DUF_PRINTF( 0, "  func:%p(optargg)", xtended->call.fdesc.s.func );
        break;
      case DUF_OPTION_VTYPE_AA_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_carg(  ))", xtended->call.fdesc.aa.func );
        break;
      case DUF_OPTION_VTYPE_TN1_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_targ(  ), duf_strtol_suff( optargg, &r ))", xtended->call.fdesc.tn1.func );
        break;
      case DUF_OPTION_VTYPE_TS1_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_targ(  ), optargg)", xtended->call.fdesc.ts1.func );
        break;
      case DUF_OPTION_VTYPE_TS2_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), optargg)", xtended->call.fdesc.ts2.func );
        break;
      default:
        break;
      }
      if ( xtended->help )
        DUF_PRINTF( 0, "  help(%s)", xtended->help );

      DUF_PRINTF( 0, ".{.o = {" );
      DUF_PRINTF( 0, ".  DO_Q( \"%s\" )", xtended->o.name );
      {
        char c;

        if ( xtended->o.has_arg == 0 )
          c = 'N';
        else if ( xtended->o.has_arg == 1 )
          c = 'R';
        else if ( xtended->o.has_arg == 2 )
          c = 'O';
        else
          c = '_';
        DUF_PRINTF( 0, "., DO_A_%c", c );
      }
      DUF_PRINTF( 0, "., DO_V( %s )", duf_extended_code2string( xtended->o.val ) );
      DUF_PRINTF( 0, ".  }" );
      DUF_PRINTF( 0, "., DO_CL( %s )", duf_extended_class2string( xtended->oclass ) );
      if ( xtended->m_hasoff )
      {
        switch ( xtended->relto )
        {
        case DUF_OFFSET_config:
          DUF_PRINTF( 0, "., DO_OC( %s,%s )", duf_extended_vtype2string( xtended->vtype ), duf_offset2stringid( xtended->m_offset, xtended->relto ) );
          if ( xtended->afl.bit )
            DUF_PRINTF( 0, "., DO_FL( %s,%s )", "??", "??" );
          break;
        case DUF_OFFSET_ufilter:
          DUF_PRINTF( 0, "., DO_OU( %s,%s )", duf_extended_vtype2string( xtended->vtype ), duf_offset2stringid( xtended->m_offset, xtended->relto ) );
          if ( xtended->afl.bit )
            DUF_PRINTF( 0, "., DO_FL( %s,%s )", "??", "??" );
          break;
        }
      }

      if ( xtended->help )
        DUF_PRINTF( 0, "., DO_H( %s )", xtended->help );
      DUF_PRINTF( 0, "}," );
      DUF_PRINTF( 0, "}" );
    }
  }
}

void
duf_extended_table_multi_print( const duf_longval_extended_table_t ** xtables, const char *name )
{
  for ( const duf_longval_extended_table_t ** xt = xtables; xt && *xt; xt++ )
  {
    char *se = NULL;
    char *matchtab = NULL;
    char *matchopt = NULL;

    if ( name )
    {
      se = strchr( name, ':' );
      if ( se )
      {
        matchtab = mas_strndup( name, se - name );
        matchopt = mas_strdup( se + 1 );
      }
      else
        matchtab = mas_strdup( name );
    }
    /* if ( ( !matchtab || !*matchtab || 0 == strncmp( ( *xt )->name, matchtab, strlen( matchtab ) ) ) ) */
    if ( ( !matchtab || !*matchtab || strstr( ( *xt )->name, matchtab ) ) )
      duf_extended_table_print( *xt, matchopt );
    mas_free( matchtab );
    mas_free( matchopt );
  }
}

void
duf_extended_table_multi_std_print( const char *name )
{
  duf_extended_table_multi_print( duf_extended_table_multi(  ), name );
}

#if 0
const char *
duf_longindex_extended_name( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.val : 0;
}
#endif
static const duf_longval_extended_t *
_duf_longindex2extended( int longindex, const duf_longval_extended_table_t * xtable, int *ptbcount, int no )
{
  const duf_longval_extended_t *extended = NULL;

  for ( const duf_longval_extended_t * xtended = xtable->table; *ptbcount <= longindex && xtended->o.name; xtended++ )
  {
    DUF_TRACE( options, +500, "li2ex %d [%s]", *ptbcount, xtended->o.name );
    if ( no )
    {
      if ( xtended->can_no )
      {
        if ( *ptbcount == longindex )
        {
          extended = xtended;
          break;
        }
        ( *ptbcount )++;
      }
    }
    else
    {
      if ( *ptbcount == longindex )
      {
        extended = xtended;
        /* break; */
      }
      ( *ptbcount )++;
    }
  }

  return extended;
}

static void
duf_xtable2options( duf_option_t ** plongopts_ptr, const duf_longval_extended_t * xtended, int no )
{
  while ( xtended->o.name )
  {
    if ( no )
    {
      if ( xtended->can_no )
      {
        char *s = NULL;

        *( *plongopts_ptr ) = xtended->o;
        s = mas_strdup( "no-" );
        s = mas_strcat_x( s, ( *plongopts_ptr )->name );
        ( *plongopts_ptr )->name = s;
        /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
        /* ( *plongopts_ptr )->val = xtended->o.val;         */
        ( *plongopts_ptr )++;
      }
    }
    else
    {
      *( *plongopts_ptr ) = xtended->o;
      ( *plongopts_ptr )->name = mas_strdup( ( *plongopts_ptr )->name );
      /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
      /* ( *plongopts_ptr )->val = xtended->o.val;         */
      ( *plongopts_ptr )++;
    }
    xtended++;
  }
}

void
duf_options_delete_longopts_table( duf_option_t * longopts )
{
  for ( duf_option_t * o = longopts; o->name; o++ )
    mas_free( ( char * ) o->name );
  mas_free( longopts );
}

duf_option_t *
duf_options_create_longopts_table( const duf_longval_extended_table_t ** pxtable )
{
  duf_option_t *longopts = NULL;

  {
    duf_option_t *longopts_ptr;
    size_t tbsize = 0;

    tbsize = duf_longindex_extended_count( pxtable ) * ( sizeof( duf_longval_extended_t ) + 1 );

    longopts = longopts_ptr = mas_malloc( tbsize );
    memset( longopts, 0, tbsize );

    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 0 );
    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 1 );
  }
  return longopts;
}

const duf_longval_extended_t *
duf_longindex2extended( int longindex, const duf_longval_extended_table_t ** pxtable, int *pno )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

  for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); tbcount <= longindex && *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

#if 0

    for ( const duf_longval_extended_t * xtended = xtable->table; tbcount <= longindex && xtended->o.name; xtended++ )
    {
      DUF_TRACE( options, +150, "@li2ex %d [%s]", tbcount, xtended->o.name );
      if ( tbcount == longindex )
      {
        extended = xtended;
        if ( pxtable )
          *pxtable = xtable;
        /* break; */
      }
      tbcount++;
    }
#else
    extended = _duf_longindex2extended( longindex, xtable, &tbcount, 0 /* no */  );
    if ( extended )
    {
      if ( pxtable )
        *pxtable = xtable;
      break;
    }
#endif
  }
  if ( !extended )
  {
    /* continue with tbcount */
    for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); tbcount <= longindex && *xtables; xtables++ )
    {
      const duf_longval_extended_table_t *xtable = *xtables;

      extended = _duf_longindex2extended( longindex, xtable, &tbcount, 1 /* no */  );
      if ( extended )
      {
        if ( pxtable )
          *pxtable = xtable;
        if ( pno )
          *pno = !*pno;
        break;
      }
    }
  }

  return extended;
}

int
duf_longindex_extended_count( const duf_longval_extended_table_t ** xtables )
{
  int xcount = 0;

#if 0
  const duf_longval_extended_table_t *xtable;

  while ( ( xtable = *xtables++ ) )
  {
    const duf_longval_extended_t *xtended = xtable->table;

    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#else
  for ( const duf_longval_extended_table_t ** pxtables = xtables; *pxtables; pxtables++ )
  {
    const duf_longval_extended_t *xtended = NULL;

    xtended = ( *pxtables )->table;
    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#endif
  return xcount;
}
