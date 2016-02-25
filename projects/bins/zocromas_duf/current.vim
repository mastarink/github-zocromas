function! MasIncludeComment1( hfil, cmt )
  let mas_timefmt="%Y%m%d.%H%M%S"
  let suff="		(" . a:hfil . ":" . strftime(mas_timefmt) . ") "
  let suff=" ♣"
" exec "%s/^\\s*\\(#\\s*include\\s\\+<\[^>\]\\+\\/"  . a:hfil .  "\.h>\\)\\s*\\(\\/\\*.*\\*\\/\\|\\)$/\\1  \\/\\* [" . a:cmt  .  "]		(" . strftime(mas_timefmt) . ") " . "\\*\\/" . "/"
  exec "%s/^\\s*\\(#\\s*include\\s\\+<mastar\\/\\([^>]\\+\\/\\|\\)"  . a:hfil .  "\.h>\\).*$/\\1  \\/\\* " . a:cmt  .  suff . "\\*\\/" . "/e"
endfunction
function! MasIncludeComment2( hfil, cmt )
  let mas_timefmt="%Y%m%d.%H%M%S"  
  let suff="		(" . a:hfil . ":" . strftime(mas_timefmt) . ") "
  let suff=" ♠"
" exec "%s/^\\s*\\(#\\s*include\\s*\"" . a:hfil . "\.h\"\\)\\s*\\(\\/\\*.*\\*\\/\\|\\)$/\\1  \\/\\* (" . strftime(mas_timefmt) . "): " . a:cmt . " \\*\\//"
  exec "%s/^\\s*\\(#\\s*include\\s*\"" . a:hfil .  "\.h\"\\).*$/\\1  \\/\\* " . a:cmt  .  suff . "\\*\\/" . "/e"
endfunction
function! MasIncludeComments()
" call MasIncludeComment0("assert", "assert")
  call MasIncludeComment1("mas_arg_tools", "mas_strcat_x; etc.")
  call MasIncludeComment1("mas_memory", "mas_(malloc|free|strdup); etc.")
  call MasIncludeComment1("mas_utils_path", "mas_normalize_path etc.")
  call MasIncludeComment2("duf_config_act_types", "duf_config_act_flags(_combo|_enum|)_t; duf_config_opt_actions_t")
  call MasIncludeComment2("duf_config_cli_types", "duf_config_cli_t")
  call MasIncludeComment2("duf_dodefs", "DOR")
  call MasIncludeComment2("duf_errorn_defs", "DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ..." )
  call MasIncludeComment2("duf_expandable", "duf_expandable_string_t; duf_string_expanded")
  call MasIncludeComment2("duf_levinfo_dirhandle_types", "duf_dirhandle_t")
  call MasIncludeComment2("duf_levinfo_types", "duf_levinfo_t")
  call MasIncludeComment2("duf_option_cfind", "duf_coption_xfind_at_stdx;  duf_coption_*  etc...")
  call MasIncludeComment2("duf_option_class", "duf_optclass2string")
  call MasIncludeComment2("duf_option_config", "duf_get_cli_options_trace_config")
  call MasIncludeComment2("duf_option_data_types", "duf_xclarifier_t etc...")
  call MasIncludeComment2("duf_option_extended", "duf_longindex_extended_count etc.")
  call MasIncludeComment2("duf_option_longopts", "duf_options_create_longopts_table")
  call MasIncludeComment2("duf_option_names", "duf_coption_names_d etc...")
  call MasIncludeComment2("duf_options_enum", "duf_option_code_t")
  call MasIncludeComment2("duf_option_source", "duf_optsource_name")
  call MasIncludeComment2("duf_option_source_types", "duf_option_source_t; duf_option_source_code_t")
  call MasIncludeComment2("duf_option_stage", "duf_optstage_name")
  call MasIncludeComment2("duf_option_stage_types", "duf_option_stage_t")
  call MasIncludeComment2("duf_option_types", "duf_longval_extended_t; duf_longval_extended_vtable_t")
  call MasIncludeComment2("duf_option_val", "duf_codeval2string")
  call MasIncludeComment2("duf_option_vtype", "duf_optvtype2string")
  call MasIncludeComment2("duf_pathinfo_types", "duf_pathinfo_t")
  call MasIncludeComment2("duf_pdi_types", "duf_depthinfo_t")
  call MasIncludeComment2("duf_printn_defs", "DUF_PRINTF etc.")
  call MasIncludeComment2("duf_scan_types", "duf_node_type_t; duf_scanstage_t")
  call MasIncludeComment2("duf_sccb_types", "duf_scan_callbacks_t")
  call MasIncludeComment2("duf_se", "DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc.")
  call MasIncludeComment2("duf_sql_bind", "duf_sql_... for DUF_SQL_BIND_... etc.")
  call MasIncludeComment2("duf_sql_stmt_defs", "DUF_SQL_BIND_S_OPT etc.")
  call MasIncludeComment2("duf_sql_types", "duf_stmnt_t")
  call MasIncludeComment2("duf_config_util", "duf_get_trace_config")
  call MasIncludeComment2("duf_sql_defs", "DUF_SQL_IDFIELD etc.")
  call MasIncludeComment2("duf_start_end", "DUF_STARTR ; DUF_ENDR")
  call MasIncludeComment2("duf_tracen_defs", "DUF_TRACE")
  call MasIncludeComment2("duf_config", "duf_get_config")
  call MasIncludeComment2("duf_utils", "duf_strtol_suff duf_strtoll_suff etc.")
  call MasIncludeComment2("sql_beginning_tables", "DUF_SQL_TABLES... etc.")
endfunction
