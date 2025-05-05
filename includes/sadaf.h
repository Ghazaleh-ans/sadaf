#ifndef SADAF_H
# define SADAF_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include "./libft/libft.h"

# define EXEC  1
# define REDIR 2
# define PIPE  3
# define LIST  4
# define BACK  5

# define MAXARGS 10

extern int g_signal_received;

typedef struct s_cmd {
    int type;
} t_cmd;

typedef struct s_execcmd {
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
}	t_execcmd;

typedef struct s_redircmd {
    int type;
    struct s_cmd *cmd;
    char *file;
    char *efile;
    int mode;
    int fd;
} t_redircmd;

typedef struct s_pipecmd {
    int type;
    struct s_cmd *left;
    struct s_cmd *right;
} t_pipecmd;

typedef struct s_listcmd {
    int type;
    struct s_cmd *left;
    struct s_cmd *right;
} t_listcmd;

typedef struct s_backcmd {
    int type;
    struct s_cmd *cmd;
} t_backcmd;

typedef struct s_env {
	char		*name;
	char		*value;
	struct s_env	*next;
} t_env;

typedef struct s_shell {
	t_env	*env_list;
	int		exit_status;
	int		in_heredoc;
}	t_shell;

/* Main functions */
t_shell *init_shell(char **envp);
void    free_shell(t_shell *shell);
int     sadaf_prompt(t_shell *shell);
int     process_command(char *buf, t_shell *shell);

/* Command execution */
void    runcmd(t_cmd *cmd, t_shell *shell);
void    execute_command(t_execcmd *ecmd, t_shell *shell);
void    handle_redirections(t_redircmd *rcmd, t_shell *shell);
void    handle_pipe(t_pipecmd *pcmd, t_shell *shell);
void    handle_list(t_listcmd *lcmd, t_shell *shell);
void    handle_background(t_backcmd *bcmd, t_shell *shell);

/* Parsing */
t_cmd   *parsecmd(char *buf, t_shell *shell);
t_cmd   *parseline(char **ps, char *es, t_shell *shell);
t_cmd   *parsepipe(char **ps, char *es, t_shell *shell);
t_cmd   *parseexec(char **ps, char *es, t_shell *shell);
t_cmd   *parseredirs(t_cmd *cmd, char **ps, char *es, t_shell *shell);
t_cmd   *parseblock(char **ps, char *es, t_shell *shell);
int     peek(char **ps, char *es, char *toks);
int     gettoken(char **ps, char *es, char **q, char **eq);
t_cmd   *nulterminate(t_cmd *cmd);

/* Command constructors */
t_cmd   *execcmd(void);
t_cmd   *redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd   *pipecmd(t_cmd *left, t_cmd *right);
t_cmd   *listcmd(t_cmd *left, t_cmd *right);
t_cmd   *backcmd(t_cmd *subcmd);

/* Environment handling */
t_env   *create_env_node(char *name, char *value);
void    add_env_node(t_shell *shell, t_env *new_node);
void    free_env_list(t_env *env_list);
t_env   *parse_env(char **envp);
char    **env_to_array(t_env *env_list);

/* Builtins */
int     is_builtin(char *cmd);
int     exec_builtin(t_execcmd *ecmd, t_shell *shell);
int     builtin_echo(t_execcmd *ecmd, t_shell *shell);
int     builtin_cd(t_execcmd *ecmd, t_shell *shell);
int     builtin_pwd(t_execcmd *ecmd, t_shell *shell);
int     builtin_export(t_execcmd *ecmd, t_shell *shell);
int     builtin_unset(t_execcmd *ecmd, t_shell *shell);
int     builtin_env(t_execcmd *ecmd, t_shell *shell);
int     builtin_exit(t_execcmd *ecmd, t_shell *shell);

/* Heredoc handling */
int     handle_heredoc(char *delimiter, t_shell *shell);
void    process_heredoc_input(int fd, char *delimiter, t_shell *shell);

/* Path handling */
char    *find_command_path(char *cmd, t_shell *shell);
char    **parse_path(t_env *env_list);
char    *build_path(char *dir, char *cmd);

/* Signals */
void    setup_signals(int mode);
void    handle_sigint(int sig);
void    handle_sigquit(int sig);

/* Utils */
void    ft_error(char *msg);
void    ft_perror(char *msg);
int     protected_fork(void);
void    free_cmd(t_cmd *cmd);
char    *ft_getenv(char *name, t_shell *shell);
void    expand_variables(t_execcmd *ecmd, t_shell *shell);
void    cleanup_tokens(char **tokens);

#endif
