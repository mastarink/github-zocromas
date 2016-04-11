#ifndef MAS_DUF_MOD_DEFS_H
# define MAS_DUF_MOD_DEFS_H

# define DUF_MOD_DECLARE_ALL_FUNCS( _modname ) \
/* static int MAS_UNUSED _modname ## _init(              duf_stmnt_t * pstmt_x, duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh ); */ \
static int MAS_UNUSED _modname ## _init(                                     duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh ); \
static int MAS_UNUSED _modname ## _node_before2(                             duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _node_before2_del(                         duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _node_after2(                              duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _node_after2_del(                          duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _node_middle2(                             duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _node_middle2_del(                         duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _de_content2(                              duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _de_content2_del(                          duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _leaf2(                                    duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _leaf2_del(                                duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _de_file_before2(                          duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh); \
static int MAS_UNUSED _modname ## _de_dir_before2(                           duf_depthinfo_t * pdi, duf_sccb_handle_t *sccbh);

#endif
