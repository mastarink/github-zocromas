#ifndef MAS_DUF_SCAN_TYPES_H
# define MAS_DUF_SCAN_TYPES_H
typedef enum
{
  DUF_SCANSTAGE_N_NONE = 0,
  DUF_SCANSTAGE_N_MIN = 1,
  DUF_SCANSTAGE_N_FS_ITEMS = DUF_SCANSTAGE_N_MIN,
  DUF_SCANSTAGE_N_NODE_BEFORE,
  DUF_SCANSTAGE_N_DB_LEAVES_PACK,
  DUF_SCANSTAGE_N_DB_LEAVES,
  DUF_SCANSTAGE_N_DB_LEAVES_NOFD,
  DUF_SCANSTAGE_N_DB_LEAVES_FD,
  DUF_SCANSTAGE_N_NODE_MIDDLE,
  DUF_SCANSTAGE_N_DB_SUBNODES,
  DUF_SCANSTAGE_N_NODE_AFTER,
  DUF_SCANSTAGE_N_MAX = DUF_SCANSTAGE_N_NODE_AFTER
} duf_scanstage_n_t;

typedef enum
{
  DUF_SCANSTAGE_NONE = 0,
  DUF_SCANSTAGE_MIN = /*              */ 1 << DUF_SCANSTAGE_N_MIN,
  DUF_SCANSTAGE_FS_ITEMS = /*         */ 1 << DUF_SCANSTAGE_N_FS_ITEMS,
  DUF_SCANSTAGE_NODE_BEFORE = /*      */ 1 << DUF_SCANSTAGE_N_NODE_BEFORE,
  DUF_SCANSTAGE_DB_LEAVES = /*        */ 1 << DUF_SCANSTAGE_N_DB_LEAVES,
  DUF_SCANSTAGE_DB_LEAVES_NOFD = /*   */ 1 << DUF_SCANSTAGE_N_DB_LEAVES_NOFD,
  DUF_SCANSTAGE_DB_LEAVES_FD = /*     */ 1 << DUF_SCANSTAGE_N_DB_LEAVES_FD,
  DUF_SCANSTAGE_DB_LEAVES_PACK = /*   */ 1 << DUF_SCANSTAGE_N_DB_LEAVES_PACK,
  DUF_SCANSTAGE_NODE_MIDDLE = /*      */ 1 << DUF_SCANSTAGE_N_NODE_MIDDLE,
  DUF_SCANSTAGE_DB_SUBNODES = /*      */ 1 << DUF_SCANSTAGE_N_DB_SUBNODES,
  DUF_SCANSTAGE_NODE_AFTER = /*       */ 1 << DUF_SCANSTAGE_N_NODE_AFTER,
  DUF_SCANSTAGE_MAX = DUF_SCANSTAGE_NODE_AFTER
} duf_scanstage_t;

typedef enum
{
  DUF_NODE_N_NONE = 0,
  DUF_NODE_N_MIN = 1,
  DUF_NODE_N_LEAF = DUF_NODE_N_MIN,
  DUF_NODE_N_NODE,
  DUF_NODE_N_FS,
  DUF_NODE_N_FS_FILE,
  DUF_NODE_N_FS_DIR,
  DUF_NODE_N_MAX = DUF_NODE_N_FS_DIR,
} duf_node_type_n_t;

typedef enum
{
  DUF_NODE_NONE = 0,
  DUF_NODE_MIN = 1 << DUF_NODE_N_MIN,
  DUF_NODE_LEAF = 1 << DUF_NODE_N_LEAF,
  DUF_NODE_NODE = 1 << DUF_NODE_N_NODE,
  DUF_NODE_FS = 1 << DUF_NODE_N_FS,
  DUF_NODE_FS_FILE = 1 << DUF_NODE_N_FS_FILE,
  DUF_NODE_FS_DIR = 1 << DUF_NODE_N_FS_DIR,
  DUF_NODE_MAX = DUF_NODE_FS_DIR,
} duf_node_type_t;

/* typedef struct stat duf_stat_t; */

/* typedef struct                    */
/* {                                 */
/*   unsigned long long sd5id;       */
/*   unsigned long long md5id;       */
/*   unsigned long long crc32id;     */
/*   unsigned long long mimeid;      */
/*   unsigned long long exifid;      */
/*   unsigned long long size;        */
/*   unsigned long long nduplicates; */
/* } md5_std_data_t;                 */
/*                                   */


#endif

/*
vi: ft=c
*/
