#ifndef MAS_RUNONCE_CONFIG_TYPES_H
#  define MAS_RUNONCE_CONFIG_TYPES_H

typedef struct
{
  int id;
  pid_t *pids;
  int instances;
  char *name;
  char *launcher;
  int largc;
  char **largv;
  int qargc;
  char **qargv;
  int sig;
  char **values;
} config_section_t;

typedef struct
{
  int pos;
  char *name;
  char *filename;
  int num_sections;
  config_section_t *sections;
} config_group_t;

typedef struct
{
  unsigned strict:1;
  unsigned dry:1;
  unsigned list_zero:1;
  unsigned list_one:1;
  unsigned list_multiple:1;
} runonce_flags_t;

typedef enum
{
  ROACTION_NONE,
  ROACTION_LAUNCH,
  ROACTION_STOP,
  ROACTION_EXIT,
  ROACTION_TERMINATE,
} roaction_t;
typedef struct
{
  roaction_t action;
  char *directory;
  int numgroups;
  config_group_t *groups;
} config_t;

typedef struct
{
  pid_t ppid;
  char *progname;
  int argc;
  char **argv;
} prec_t;

typedef struct
{
  prec_t *array;
  size_t size;
} pids_t;

/* typedef struct            */
/* {                         */
/*   config_t configuration; */
/*   pids_t pids;            */
/* } runonce_t;              */

typedef const char *( fun_t ) ( pid_t pid, const char *line, const char *eline );

extern config_t configuration;
extern pids_t pids;

typedef enum
{
  RUNONCE_NONE,
  /* + */
  RUNONCE_LAUNCHER,
  /* + */
  RUNONCE_NOLAUNCH,
  /* + */
  RUNONCE_NOSTOP,
  /* + */
  RUNONCE_NOEXIT,
  /* + */
  RUNONCE_NOTERMINATE,
  /* + */
  RUNONCE_NOGROUP,
  /* + */
  RUNONCE_NONICE,
  /* + */
  RUNONCE_PREFIX,
  RUNONCE_OPTS4PID,
  /* + */
  RUNONCE_PATH,
  RUNONCE_PSNAME,
  RUNONCE_COMMAND,
  /* + */
  RUNONCE_NICE,
  /* LAUNCHER < OPTIONS !!! */
  RUNONCE_OPTIONS,
  /* + */
  RUNONCE_PFIN,
  RUNONCE_PROCESS,
  /* - */
  RUNONCE_WINDOWRE,
  /* + */
  RUNONCE_XGROUP,
  /* + */
  RUNONCE_QUITTER,
  /* - */
  RUNONCE_RESTARTER,
  /* - */
  RUNONCE_RELOADER,
  /* - */
  RUNONCE_TURN_ONER,
  /* - */
  RUNONCE_TURN_OFFER,
  /* - */
  RUNONCE_DISABLER,
  /* - */
  RUNONCE_ENABLER,
  /* - */
  RUNONCE_TOGGLER,
  RUNONCE_AWAIT,
  RUNONCE_RWAIT,
  RUNONCE_MAX
} item_id_t;

#endif
