SUBDIRS= inc
bin_PROGRAMS=zocromas_duf
if  MAS_TEST
noinst_PROGRAMS=mtest mjtest motestn
# motest_SOURCES=optest.c duf_options_all_stages.c duf_options.c duf_option_stage.c duf_option_config_credel.c duf_option_config.c duf_option_clarify_new.c duf_options_env.c duf_options_file.c duf_options_cli.c duf_options_interactive.c duf_option_source.c duf_option_longopts.c duf_option_clarify.c duf_option_clarify_batch.c duf_option_cfind.c duf_option_extended.c duf_option_typed.c duf_option_lfind.c duf_option_typed_call.c duf_option_typed_gen.c duf_option_names.c duf_option_descr.c
motestn_SOURCES=optest_n.c
#duf_options.c duf_optmod_tmpdb.c
#  duf_config_util_z.c duf_config_util.c
# motest_CFLAGS=@AM_CFLAGS@ -pthread @DEPSS_CFLAGS@   -include motest_config.h
# motest_LDFLAGS=@AM_LDFLAGS@  @DEPSS_LIBS@   @READLINE_LIBS@

motestn_CFLAGS=@AM_CFLAGS@ -pthread @DEPSS_CFLAGS@ @DEPSN_CFLAGS@  -include motest_config.h
motestn_LDFLAGS=@AM_LDFLAGS@  @DEPSS_LIBS@ @DEPSN_LIBS@  @READLINE_LIBS@


mtest_SOURCES=duftest.c 
mtest_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@  @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@
mtest_LDFLAGS=@AM_LDFLAGS@        @DEPS_LIBS@    @LIBCRYPTO_LIBS@    @SQLITE3_LIBS@   @LIBMAGIC_LIBS@

mjtest_SOURCES=dufjtest.c
mjtest_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @LIBEXIF_CFLAGS@
mjtest_LDFLAGS=@AM_LDFLAGS@                     @LIBEXIF_LIBS@


else

endif


# lib_LTLIBRARIES = libzocromas_mas_multiconfig.la
# libzocromas_mas_multiconfig_la_SOURCES = duf_options_all_stages.c duf_options.c duf_option_stage.c duf_option_config_credel.c duf_option_config.c duf_option_clarify_new.c duf_options_env.c duf_options_file.c duf_options_cli.c duf_options_interactive.c duf_option_source.c duf_option_longopts.c duf_option_clarify.c duf_option_clarify_batch.c duf_option_cfind.c duf_option_extended.c duf_option_typed.c duf_option_lfind.c duf_option_typed_call.c duf_option_typed_gen.c duf_option_names.c duf_option_descr.c 
# libzocromas_mas_multiconfig_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@


#noinst_HEADERS=....cdef ....cdef

zocromas_duf_SOURCES= \
	duf.c \
	data_optable_debug.c \
	data_optable_experimental.c \
	data_optable_fs.c \
	data_optable_interactive.c \
	data_optable_redo_and_obsolete.c \
	data_optable_sflag.c \
	data_optable_options.c \
	data_optable_uplus.c \
	duf_action_table.c \
	duf_mod_handle.c \
	 \
	 \
	duf_cfg_credel.c \
	duf_cfg_output_util.c \
	duf_config.c \
	duf_config_credel.c \
	duf_config_db.c duf_config_db_get.c \
	duf_config_output_util.c \
	duf_config_pointers.c \
	duf_config_trace.c \
	duf_config_util.c \
	duf_context.c \
	duf_dh.c \
	duf_dirent.c \
	duf_evsql_begfin.c \
	duf_evsql_selector_new.c \
	duf_filedata.c \
	duf_fileinfo.c \
	duf_levinfo.c \
	duf_levinfo_count.c \
	duf_levinfo_credel.c \
	duf_levinfo_dbinit.c \
	duf_levinfo_init.c \
	duf_levinfo_openclose.c \
	duf_levinfo_ref.c \
	duf_levinfo_stat.c \
	duf_levinfo_updown.c \
	duf_li.c \
	duf_li_credel.c \
	duf_maindb.c \
	duf_maindb_info.c \
	duf_match.c \
	duf_optimpl_fs.c \
	duf_optimpl_help.c \
	duf_optimpl_example.c \
	duf_optimpl_misc.c \
	duf_optimpl_sccb.c \
	duf_optimpl_oclass_help.c \
	duf_optimpl_set_help.c \
	duf_optimpl_version.c \
	duf_optimpl_extended2string.c \
	duf_optimpl_val.c \
	duf_optimpl_class.c \
	\
	\
	duf_optmod_tmpdb.c \
	\
	duf_path2db.c \
	duf_path2dirid.c \
	duf_pathinfo.c \
	duf_pathinfo_credel.c \
	duf_pathinfo_depth.c \
	duf_pathinfo_ref.c \
	 \
	duf_pdi_attach.c \
	duf_pdi.c \
	duf_pdi_context.c \
	duf_pdi_credel.c \
	duf_pdi_depth.c \
	duf_pdi_filters.c \
	duf_pdi_global.c \
	duf_pdi_pi_ref.c \
	duf_pdi_ref.c \
	duf_pdi_reinit.c \
	duf_pdi_sccb_eval.c \
	duf_pdi_stmt.c \
	duf_print.c \
	duf_sccb_begfin.c \
	duf_sccb.c \
	duf_sccb_eval.c \
	duf_sccb_eval_std.c \
	duf_sccb_handle.c \
	duf_sccb_row.c \
	duf_sccbh_ref.c \
	duf_sccbh_row.c \
	duf_sccbh_pstmt.c \
	duf_sccbh_eval_all.c \
	duf_sccbh_eval.c \
	duf_sccbh_scanner.c \
	duf_sccbh_eval_fs.c \
	duf_sccbh_eval_leaf.c \
	duf_sccbh_eval_node.c \
	duf_sccbh_eval_sql_set.c \
	duf_sccb_scanstage.c \
	duf_nodetype.c \
	duf_sel_cb_leaf.c \
	duf_sel_cb_node.c \
	duf_service.c \
	duf_sql_bind.c \
	duf_sql_error.c \
	duf_sql_field.c \
	duf_sql_open.c \
	duf_sql_positional.c \
	duf_sql_prepared.c \
	duf_status.c \
	duf_sys_wrap.c \
	duf_tags.c \
	duf_tracen_config.c \
	duf_ufilter_bind.c \
	duf_ufilter.c \
	duf_ufilter_ref.c \
	duf_utils.c \
	duf_xtended_table.c \
	mod_dummy_uni.c \
	sql_beginning_common.c \
	sql_beginning_create.c \
	sql_beginning_selected.c \
	sql_beginning_tables.c \
	sql_beginning_vacuum.c \
	sql_update_selected.c \
	std_fieldsets.c \
	std_mod_sets.c \
	std_selectors.c

#       duf_option_clarify_batch.c 
#       duf_option_clarify.c 
#       duf_option_clarify_new.c 
#       duf_option_class.c 
#       duf_option_config.c 
#       duf_option_config_credel.c 
#       duf_option_descr.c 
#       duf_option_extended.c 
#       duf_option_lfind.c 
#	duf_option_cfind.c 
#       duf_option_longopts.c 
#       duf_option_names.c 
#       duf_options_all_stages.c 
#       duf_options.c 
#       duf_options_cli.c 
#       duf_options_env.c 
#       duf_options_file.c 
#       duf_options_interactive.c 
#       
#       duf_option_source.c 
#       duf_option_stage.c 
#       duf_option_typed.c 
#       duf_option_typed_call.c 
#       duf_option_typed_gen.c 
#       duf_option_vtype.c

############################################################
############################################################
############################################################


#	duf_item_match2.c
#	duf_file_pathid2.c
#	mod_print_md5_uni.c
#	duf_dirent_scan1.c
#	duf_dir_scan1.c
#	duf_file_pathid1.c
#	duf_item_scan1.c
#	duf_sql_field1.c
#	duf_uni_scan1.c

zocromas_duf_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @DEPSN_CFLAGS@ @LIBCRYPTO_CFLAGS@  @SQLITE3_CFLAGS@  @LIBEXIF_CFLAGS@
zocromas_duf_LDFLAGS=@AM_LDFLAGS@        @DEPS_LIBS@ @DEPSN_LIBS@     @LIBCRYPTO_LIBS@    @SQLITE3_LIBS@    @LIBEXIF_LIBS@    @LIBMAGIC_LIBS@ @ZLIB_LIBS@ @READLINE_LIBS@ -ldl





# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@
#   template_LDFLAGS = -Wl,--export-dynamic
