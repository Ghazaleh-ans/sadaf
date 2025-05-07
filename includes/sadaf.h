/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:48:31 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/06 17:18:37 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "libft/libft.h"
# ifndef PATH_MAX
# define PATH_MAX 1024
# endif

# define EXEC  1
# define REDIR 2
# define PIPE  3
# define LIST  4
# define BACK  5
# define HEREDOC 6

# define MAXARGS 100

extern int g_signal_received;

enum TokenType {
	TOK_EOF,
	TOK_WORD,
	TOK_PIPE,
	TOK_AND,
	TOK_SEQ,
	TOK_LT,
	TOK_GT,
	TOK_DGT,
	TOK_DLT,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_UNKNOWN
};

typedef struct s_token {
	enum TokenType type;
	char *start;
	char *end;
} t_token;

typedef struct s_cmd {
	int type;
} t_cmd;

typedef struct {
  char *s;
  char *end;
} ParserState;


typedef struct s_execcmd {
	int type;
	char *argv[MAXARGS];
	char *eargv[MAXARGS];
} t_execcmd;

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
	char *name;
	char *value;
	struct s_env *next;
} t_env;

typedef struct s_shell {
	t_env *env_list;
	int exit_status;
	int in_heredoc;
} t_shell;

/* Main functions */
t_shell	*init_shell(char **envp);
char	*getcmd(void);
int		process_command(char *buf, t_shell *shell);

// cleaner functions
void	free_cmd(t_cmd *cmd);
void	free_env_list(t_env *env_list);
void	free_shell(t_shell *shell);
void	ft_exit(char *msg);
/* Parsing */
t_cmd   *parsecmd(char *buf);
t_cmd	*parseline(ParserState *ps);
t_cmd	*parsepipe(ParserState *ps);
t_cmd	*parseexec(ParserState *ps);
t_cmd   *parseredirs(t_cmd *cmd, ParserState *ps);
t_cmd	*parseblock(ParserState *ps);
t_token	gettoken(ParserState *ps);
t_cmd	*nulterminate(t_cmd *cmd);
t_cmd	*set_heredoc(t_cmd *subcmd, char *file, char *efile);

/* Command constructors */
t_cmd   *execcmd(void);
t_cmd   *redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd   *pipecmd(t_cmd *left, t_cmd *right);
t_cmd   *listcmd(t_cmd *left, t_cmd *right);
t_cmd   *backcmd(t_cmd *subcmd);

/* Environment handling */
t_env	*init_envp(char **envp);
t_env	*create_env_node(char *name, char *value);

/* Environment utils */
void	add_env_node(t_shell *shell, t_env *new_node);
char	**env_to_array(t_env *env_list);

/* Builtins */
int		is_builtin(char *cmd);
int		exec_builtin(t_execcmd *ecmd, t_shell *shell);
int		builtin_echo(t_execcmd *ecmd, t_shell *shell);
int		builtin_cd(t_execcmd *ecmd, t_shell *shell);
int		builtin_pwd(t_execcmd *ecmd, t_shell *shell);
int		builtin_export(t_execcmd *ecmd, t_shell *shell);
int		builtin_unset(t_execcmd *ecmd, t_shell *shell);
int		builtin_env(t_execcmd *ecmd, t_shell *shell);
int		builtin_exit(t_execcmd *ecmd, t_shell *shell);

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

/* Command execution */
void    runcmd(t_cmd *cmd, t_shell *shell);
void    execute_command(t_execcmd *ecmd, t_shell *shell);
void    handle_redirections(t_redircmd *rcmd, t_shell *shell);
void    handle_pipe(t_pipecmd *pcmd, t_shell *shell);
void    handle_list(t_listcmd *lcmd, t_shell *shell);
void    handle_background(t_backcmd *bcmd, t_shell *shell);

/* Utils */
void    ft_error(char *msg);
void    ft_perror(char *msg);
int		protected_fork(void);
char    *ft_getenv(char *name, t_shell *shell);
void    expand_variables(t_execcmd *ecmd, t_shell *shell);
void    cleanup_tokens(char **tokens);
int		fork1(void);
void	print_cmd(t_cmd *cmd);
t_cmd *nulterminate(t_cmd *cmd);

#endif
