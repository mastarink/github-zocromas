#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H

#  define DUF_FUNN(af) duf_dbg_funname( ( duf_anyhook_t ) af )

/* #  include "duf_levinfo_types.h" */


typedef enum
{
  DUF_NODE_LEAF = 100,
  DUF_NODE_NODE,
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
