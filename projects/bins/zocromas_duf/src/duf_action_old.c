/* tables drop       */
/* tables check      */
/* paths add ...     */
/* md5 update        */
/* exif update       */
/* duplicates update */
/* mdline update     */
/* duplicates  print */

/*
 * run tables drop
 * run tables check
 * run paths add <dir path>
 * sqf "select * from duf_group;"
 * -- groups argument,updated --
 * run paths print [+-]<group>
 * run paths id <dir path>
 * TODO : run paths update <path> - to make "add" not updating
 * TODO : run files print <pathgroup> - add pathgroup
 * run md5 update
 * run md5 same
 * run md5 print
 * run group <dir path>
 * */


int
duf_action( int argc, char **argv )
{
  int r = 0;

  for ( int ia = 1; ia < argc; ia++ )
  {
    /* fprintf( stderr, "ia:%d of %d\n\n\n", ia, argc ); */
    if ( 0 )
    {
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "data" ) && 0 == strcmp( argv[ia + 1], "prepare" ) )
    {
      ia++;
      duf_check_tables(  );
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        duf_add_path( argv[ia], "argument" );
        duf_update_path_down( argv[ia], 0 /* parentid */ , DUF_RECURSIVE_YES, DUF_TRUE /* dofiles */  );
      }
      duf_update_md5_path( NULL, DUF_RECURSIVE_NO );
      duf_update_duplicates(  );
      duf_update_mdpaths(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "tables" ) && 0 == strcmp( argv[ia + 1], "drop" ) )
    {
      ia++;
      duf_clear_tables(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "tables" ) && 0 == strcmp( argv[ia + 1], "check" ) )
    {
      ia++;
      duf_check_tables(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "add" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
        duf_add_path( argv[ia], "argument" );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
        duf_update_path_down( argv[ia], 0 /* parentid */ , DUF_RECURSIVE_YES, DUF_TRUE /* dofiles */  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      if ( ia + 1 < argc )
        duf_print_paths( argv[++ia] );
      else
        duf_print_paths( NULL );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "id" ) )
    {
      unsigned long long prev = 0;
      char *notfound = NULL;

      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        unsigned long long pathid = duf_path_to_pathid_x( argv[ia], &prev, &notfound );
        char *path = duf_pathid_to_path( pathid );
        char *lastpath = duf_pathid_to_path( prev );

        if ( pathid )
        {
          printf( "#%lld:\t\t'%s' => '%s'\nprev: #%lld =>\t'%s'\t\t; n/f '%s'\n", pathid, argv[ia], path ? path : "", prev, lastpath,
                  notfound ? notfound : "" );
        }
        else
        {
        }
        mas_free( path );
        mas_free( lastpath );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "files" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        const char *path = argv[ia];

        /* fprintf( stderr, "argv[%d]='%s'\n", ia, argv[ia] ); */
        if ( *path == '+' )
          duf_print_files( ++path, DUF_RECURSIVE_YES );
        else if ( *path == '-' )
          duf_print_files( ++path, DUF_RECURSIVE_NO );
        else
          duf_print_files( path, DUF_RECURSIVE_NO );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "dirs" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        const char *path = argv[ia];

        /* fprintf( stderr, "argv[%d]='%s'\n", ia, argv[ia] ); */
        if ( *path == '+' )
          duf_print_dirs( ++path, DUF_RECURSIVE_YES );
        else if ( *path == '-' )
          duf_print_dirs( ++path, DUF_RECURSIVE_NO );
        else
          duf_print_dirs( path, DUF_RECURSIVE_NO );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "files" ) && 0 == strcmp( argv[ia + 1], "same" ) )
    {
      ia++;
      if ( ia + 1 >= argc )
        fprintf( stderr, "argument needed %d %d\n", ia, argc );
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        duf_print_files_same( argv[ia] );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      duf_print_md5( 0 );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      ia++;
      /* fprintf( stderr, "ia:%d argc:%d\n", ia, argc ); */
      if ( ia + 1 >= argc )
        duf_update_md5_path( NULL, DUF_RECURSIVE_NO );
      else
        for ( ia = ia + 1; ia < argc; ia++ )
          duf_update_md5_path( argv[ia], DUF_RECURSIVE_NO );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "same" ) )
    {
      unsigned long long limit = 0;

      ia++;
      /* fprintf( stderr, "ia:%d of %d\n", ia, argc ); */
      if ( ia + 1 >= argc || sscanf( argv[ia + 1], "%llu", &limit ) )
        duf_print_md5_same( 1, limit );
      ia++;
    }
    /* else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "group" ) ) */
    /* {                                                                 */
    /*   ia += 1;                                                        */
    /*   if ( ia < argc )                                                */
    /*   {                                                               */
    /*     char *group;                                                  */
    /*                                                                   */
    /*     group = argv[ia++];                                           */
    /*     for ( ia = ia; ia < argc; ia++ )                              */
    /*     {                                                             */
    /*       const char *path = argv[ia];                                */
    /*                                                                   */
    /*       duf_paths_group( group, path );                             */
    /*     }                                                             */
    /*   }                                                               */
    /* }                                                                 */
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "exif" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      duf_update_exif(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "zero" ) )
    {
      duf_zero_duplicates(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      duf_update_duplicates(  );
      ia++;
    }
    /* else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "print" ) ) */
    /* {                                                                                                              */
    /*   print_duplicates( 5000 );                                                                                    */
    /*   ia++;                                                                                                        */
    /* }                                                                                                              */
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "mdline" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      duf_update_mdpaths(  );
      ia++;
    }
    else
    {
      fprintf( stderr, "wrong command\n" );
    }
  }
  fprintf( stderr, "\n\n\n" );
  return r;
}
