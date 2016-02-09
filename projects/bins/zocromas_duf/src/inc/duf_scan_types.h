#ifndef MAS_DUF_SCAN_TYPES_H
#  define MAS_DUF_SCAN_TYPES_H

typedef enum
{
  DUF_SCANSTAGE_NONE,
  DUF_SCANSTAGE_MIN,
  DUF_SCANSTAGE_FS_ITEMS = DUF_SCANSTAGE_MIN,
  DUF_SCANSTAGE_NODE_BEFORE,
  DUF_SCANSTAGE_DB_LEAVES_NOFD,
  DUF_SCANSTAGE_DB_LEAVES_FD,
  DUF_SCANSTAGE_NODE_MIDDLE,
  DUF_SCANSTAGE_DB_SUBNODES,
  DUF_SCANSTAGE_NODE_AFTER,
  DUF_SCANSTAGE_MAX = DUF_SCANSTAGE_NODE_AFTER
} duf_scanstage_t;

typedef enum
{
  DUF_NODE_NONE,
  DUF_NODE_MIN,
  DUF_NODE_LEAF = DUF_NODE_MIN,
  DUF_NODE_NODE,
  DUF_NODE_FS,
  DUF_NODE_FS_FILE,
  DUF_NODE_FS_DIR,
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
