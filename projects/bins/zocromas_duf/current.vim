function! MasIncludeComment1( hfil, cmt )
  let mas_timefmt="%Y%m%d.%H%M%S"
  let suff="		(" . a:hfil . ":" . strftime(mas_timefmt) . ") "
  let suff=" ▤"
" exec "%s/^\\s*\\(#\\s*include\\s\\+<\[^>\]\\+\\/"  . a:hfil .  "\.h>\\)\\s*\\(\\/\\*.*\\*\\/\\|\\)$/\\1  \\/\\* [" . a:cmt  .  "]		(" . strftime(mas_timefmt) . ") " . "\\*\\/" . "/"
  exec "%s/^\\s*\\(#\\s*include\\s\\+<mastar\\/\\([^>]\\+\\/\\|\\)"  . a:hfil .  "\.h>\\).*$/\\1  \\/\\* " . a:cmt  .  suff . "\\*\\/" . "/e"
endfunction
function! MasIncludeComment2( hfil, cmt )
  let mas_timefmt="%Y%m%d.%H%M%S"  
  let suff="		(" . a:hfil . ":" . strftime(mas_timefmt) . ") "
  let suff=" ✗"
" exec "%s/^\\s*\\(#\\s*include\\s*\"" . a:hfil . "\.h\"\\)\\s*\\(\\/\\*.*\\*\\/\\|\\)$/\\1  \\/\\* (" . strftime(mas_timefmt) . "): " . a:cmt . " \\*\\//"
  exec "%s/^\\s*\\(#\\s*include\\s*\"" . a:hfil .  "\.h\"\\).*$/\\1  \\/\\* " . a:cmt  .  suff . "\\*\\/" . "/e"
endfunction
function! MasIncludeComments()
" call MasIncludeComment0("assert", "assert")
  call MasIncludeComment1("mas_arg_tools", "mas_strcat_x; etc.")
  call MasIncludeComment1("mas_convert", "mas_strtol_suff; mas_strtoll_suff; etc.")
  call MasIncludeComment1("mas_error_types", "mas_error_code_t")
  call MasIncludeComment1("mas_memory", "mas_(malloc|free|strdup); etc.")
  call MasIncludeComment1("mas_time", "mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|(|t)strflocal|strfgm)time ; strtime2long; etc.")
  call MasIncludeComment1("mas_utils_path", "mas_normalize_path; mas_pathdepth; mas_realpath etc.")
  call MasIncludeComment2("duf_config_act_types", "duf_config_act_flags(_combo|_enum|)_t; duf_config_opt_actions_t")
  call MasIncludeComment2("duf_config_cli_types", "duf_config_cli_t")
  call MasIncludeComment2("duf_config_defs", "DUF_CONF...")
  call MasIncludeComment2("duf_config_disable_types", "duf_config_opt_disable_flags*_t;")
  call MasIncludeComment2("duf_config", "duf_get_config")
  call MasIncludeComment2("duf_config_flow_types", "duf_config_flow*_t ( verbose; flow flags);")
  call MasIncludeComment2("duf_config_output_types", "duf_config_output_t")
  call MasIncludeComment2("duf_config_trace", "duf_trace_file_c; duf_trace_output_force_color_c; duf_trace_output_no_color_c")
  call MasIncludeComment2("duf_config_types", "duf_config_t; duf_config_save_t; duf_config_cfg_t; duf_config_vars_t")
  call MasIncludeComment2("duf_config_util", "duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset)")
  call MasIncludeComment2("duf_debug_defs", "DUF_WRAPSTATIC; DUF_WRAPPED ... ")
  call MasIncludeComment2("duf_dh", "duf_openat_dh; duf_open_dh; duf_opened_dh; duf_close_dh; duf_statat_dh; etc.")
  call MasIncludeComment2("duf_dodefs", "DOR")
  call MasIncludeComment2("duf_errorn_defs", "DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ..." )
  call MasIncludeComment2("duf_evsql_begfin", "duf_eval_sqlsq()")
  call MasIncludeComment2("duf_expandable", "duf_expandable_string_t; duf_string_expanded")
  call MasIncludeComment2("duf_fun_types", "duf_void_voidp_func_t etc. etc. ...")
  call MasIncludeComment2("duf_hook_types", "duf_action_table_t")
  call MasIncludeComment2("duf_levinfo_credel", "duf_levinfo_create; duf_levinfo_delete")
  call MasIncludeComment2("duf_levinfo_dirhandle_types", "duf_dirhandle_t")
  call MasIncludeComment2("duf_levinfo", "duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc.")
  call MasIncludeComment2("duf_levinfo_init", "duf_levinfo_init_level; duf_levinfo_init_level_d; etc.")
  call MasIncludeComment2("duf_levinfo_ref", "duf_levinfo_*; etc.")
  call MasIncludeComment2("duf_levinfo_stmt_types", "duf_stmt_ident_t; duf_idstmt_t;")
  call MasIncludeComment2("duf_levinfo_types", "duf_levinfo_t")
  call MasIncludeComment2("duf_maindb", "duf_main_db; duf_main_db_open; duf_main_db_close")
  call MasIncludeComment2("duf_maindb_info", "duf_main_db_info()")
  call MasIncludeComment2("duf_optimpl_defs", "DUF_UG_FLAG; DUF_ACT_FLAG etc.")
  call MasIncludeComment2("duf_optimpl_enum", "duf_option_code_t")
  call MasIncludeComment2("duf_optimpl_val", "duf_codeval2string")
  call MasIncludeComment2("duf_option_cfind", "duf_coption_xfind_at_stdx;  duf_coption_*  etc...")
  call MasIncludeComment2("duf_option_class", "duf_optclass2string")
  call MasIncludeComment2("duf_option_config", "duf_get_cli_options_trace_config")
  call MasIncludeComment2("duf_option_data_types", "duf_xclarifier_t etc...")
  call MasIncludeComment2("duf_option_extended", "duf_longindex_extended_count etc.")
  call MasIncludeComment2("duf_option_longopts", "duf_options_create_longopts_table")
  call MasIncludeComment2("duf_option_names", "duf_coption_names_d etc...")
  call MasIncludeComment2("duf_option_source", "duf_optsource_name")
  call MasIncludeComment2("duf_option_source_types", "duf_option_source_t; duf_option_source_code_t")
  call MasIncludeComment2("duf_option_stage", "duf_optstage_name")
  call MasIncludeComment2("duf_option_stage_types", "duf_option_stage_t")
  call MasIncludeComment2("duf_option_types", "duf_option_t; duf_option_gen_code_t; duf_longval_extended_t; duf_longval_extended_vtable_t")
  call MasIncludeComment2("duf_option_vtype", "duf_optvtype2string")
  call MasIncludeComment2("duf_path2db", "duf_real_path2db; etc.")
  call MasIncludeComment2("duf_path2dirid", "duf_dirid2path; etc.")
  call MasIncludeComment2("duf_pathinfo_credel", "duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc.")
  call MasIncludeComment2("duf_pathinfo", "duf_pi_clear*; duf_pi_levinfo_set; duf_pi_set_max_rel_depth; etc.")
  call MasIncludeComment2("duf_pathinfo_types", "duf_pathinfo_t")
  call MasIncludeComment2("duf_pdi_credel", "duf_pdi_create; duf_pdi_kill")
  call MasIncludeComment2("duf_pdi", "duf_pdi_init; duf_pdi_shut; duf_pdi_close")
  call MasIncludeComment2("duf_pdi_filters", "duf_pdi_pu; etc.")
  call MasIncludeComment2("duf_pdi_global", "duf_pdi_global()")
  call MasIncludeComment2("duf_pdi_stmt", "duf_pdi_find_statement_by_id; etc.")
  call MasIncludeComment2("duf_pdi_types", "duf_depthinfo_t")
  call MasIncludeComment2("duf_printn_defs", "DUF_PRINTF etc.")
  call MasIncludeComment2("duf_scan_types", "duf_node_type_t; duf_scanstage_t")
  call MasIncludeComment2("duf_sccb_types", "duf_scan_callbacks_t")
  call MasIncludeComment2("duf_se", "DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc.")
  call MasIncludeComment2("duf_se_only", "Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc.")
  call MasIncludeComment2("duf_sql_bind", "duf_sql_... for DUF_SQL_BIND_... etc.")
  call MasIncludeComment2("duf_sql_defs", "DUF_SQL_IDFIELD etc.")
  call MasIncludeComment2("duf_sql_field", "__duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc.")
  call MasIncludeComment2("duf_sql_open", "duf_sql_open()")
  call MasIncludeComment2("duf_sql_positional", "duf_sql_column_long_long etc.")
  call MasIncludeComment2("duf_sql_prepared", "duf_sql_(prepare|step|finalize)")
  call MasIncludeComment2("duf_sql_se_stmt_defs", "DUF_SQL_SE_BIND_S_OPT etc.")
  call MasIncludeComment2("duf_sql_stmt_defs", "DUF_SQL_BIND_S_OPT etc.")
  call MasIncludeComment2("duf_sql_types", "duf_stmnt_t")
  call MasIncludeComment2("duf_start_end", "DUF_STARTR ; DUF_ENDR")
  call MasIncludeComment2("duf_status_types", "duf_status_t; duf_status_db_t; duf_status_dh_t; duf_status_scanner_t; ")
  call MasIncludeComment2("duf_sys_wrap", "duf_unlink()")
  call MasIncludeComment2("duf_tmp_types", "duf_tmp_t")
  call MasIncludeComment2("duf_tracen_defs_preset", "MAST_TRACE_CONFIG; etc.")
  call MasIncludeComment2("duf_tracen_defs", "T; TT; TR")
  call MasIncludeComment2("duf_ufilter_types", "duf_ufilter_t; duf_yfilter_t; etc.")
  call MasIncludeComment2("duf_utils", "duf_percent;  etc.")
  call MasIncludeComment2("sql_beginning_tables", "DUF_SQL_TABLES... etc.")
endfunction
function! MasCurrent()
  call MasIncludeComments()
endfunction
