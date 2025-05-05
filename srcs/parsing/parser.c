#include "../../includes/sadaf.h"

static int	skip_whitespace(char **ps, char *es)
{
	char	*s;
	const char	*whitespace = " \t\r\n\v";

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	*ps = s;
	return (0);
}

static int	get_simple_token(char *s, char **ps)
{
	int	ret;

	ret = *s;
	if (*s == '|' || *s == '(' || *s == ')' || *s == ';' || *s == '&' || *s == '<')
		s++;
	else if (*s == '>')
	{
		s++;
		if (*s == '>')
		{
			ret = '+';
			s++;
		}
	}
	*ps = s;
	return (ret);
}

static void	handle_single_quote(char **s, char **q, char **eq, char *es)
{
	**s += 1;
	*q = *s;
	while (*s < es && **s != '\'')
		(*s)++;
	if (*s < es && **s == '\'')
	{
		*eq = *s;
		(*s)++;
	}
	else
		*eq = *s;
}

static void	handle_double_quote(char **s, char **q, char **eq, char *es)
{
	**s += 1;
	*q = *s;
	while (*s < es && **s != '"')
		(*s)++;
	if (*s < es && **s == '"')
	{
		*eq = *s;
		(*s)++;
	}
	else
		*eq = *s;
}

static void	handle_normal_token(char **s, char *es)
{
	const char	*whitespace = " \t\r\n\v";
	const char	*symbols = "<|>&;()";

	while (*s < es && !strchr(whitespace, **s)
		&& !strchr(symbols, **s) && **s != '\'' && **s != '"')
		(*s)++;
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	skip_whitespace(ps, es);
	s = *ps;
	if (q)
		*q = s;
	ret = *s;
	if (*s == 0)
		return (ret);
	if (strchr("<|>&;()", *s) || *s == '>')
		ret = get_simple_token(s, &s);
	else if (*s == '\'')
	{
		handle_single_quote(&s, q, eq, es);
		ret = 'a';
	}
	else if (*s == '"')
	{
		handle_double_quote(&s, q, eq, es);
		ret = 'a';
	}
	else
	{
		handle_normal_token(&s, es);
		if (eq)
			*eq = s;
		ret = 'a';
	}
	skip_whitespace(&s, es);
	*ps = s;
	return (ret);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	skip_whitespace(ps, es);
	s = *ps;
	return (*s && strchr(toks, *s));
}

static int	handle_syntax_error(char **buf, char *es, t_cmd *cmd)
{
	peek(buf, es, "");
	if (*buf != es)
	{
		ft_putstr_fd("sadaf: syntax error\n", 2);
		free_cmd(cmd);
		return (1);
	}
	return (0);
}

t_cmd	*parsecmd(char *buf, t_shell *shell)
{
	char	*es;
	t_cmd	*cmd;

	es = buf + strlen(buf);
	cmd = parseline(&buf, es, shell);
	if (handle_syntax_error(&buf, es, cmd))
		return (NULL);
	cmd = nulterminate(cmd);
	return (cmd);
}

static t_cmd	*handle_background(t_cmd *cmd, char **ps, char *es, t_shell *shell)
{
	while (peek(ps, es, "&"))
	{
		gettoken(ps, es, 0, 0);
		cmd = backcmd(cmd);
	}
	return (cmd);
}

static t_cmd	*handle_semicolon(t_cmd *cmd, char **ps, char *es, t_shell *shell)
{
	if (peek(ps, es, ";"))
	{
		gettoken(ps, es, 0, 0);
		cmd = listcmd(cmd, parseline(ps, es, shell));
	}
	return (cmd);
}

t_cmd	*parseline(char **ps, char *es, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parsepipe(ps, es, shell);
	cmd = handle_background(cmd, ps, es, shell);
	cmd = handle_semicolon(cmd, ps, es, shell);
	return (cmd);
}

t_cmd	*parsepipe(char **ps, char *es, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parseexec(ps, es, shell);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es, shell));
	}
	return (cmd);
}

static t_cmd	*parse_heredoc(t_cmd *cmd, char **ps, char *es, t_shell *shell)
{
	char	*q;
	char	*eq;
	char	*delim;
	int		fd;

	gettoken(ps, es, 0, 0);
	if (gettoken(ps, es, &q, &eq) != 'a')
	{
		ft_putstr_fd("sadaf: syntax error: missing delimiter for heredoc\n", 2);
		free_cmd(cmd);
		return (NULL);
	}
	delim = ft_substr(q, 0, eq - q);
	fd = handle_heredoc(delim, shell);
	free(delim);
	if (fd < 0)
	{
		free_cmd(cmd);
		return (NULL);
	}
	cmd = redircmd(cmd, NULL, eq, O_RDONLY, 0);
	((t_redircmd *)cmd)->fd = fd;
	return (cmd);
}

static t_cmd	*handle_normal_redir(t_cmd *cmd, int tok, char **ps, char *es)
{
	char	*q;
	char	*eq;

	if (gettoken(ps, es, &q, &eq) != 'a')
	{
		ft_putstr_fd("sadaf: syntax error: missing file for redirection\n", 2);
		free_cmd(cmd);
		return (NULL);
	}
	if (tok == '<')
		cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_RDONLY, 0);
	else if (tok == '>')
		cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
	else if (tok == '+')
		cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_WRONLY | O_CREAT | O_APPEND, 1);
	return (cmd);
}

t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es, t_shell *shell)
{
	int	tok;

	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (tok == '<' && peek(ps, es, "<"))
			cmd = parse_heredoc(cmd, ps, es, shell);
		else
			cmd = handle_normal_redir(cmd, tok, ps, es);
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

static t_cmd	*handle_missing_closing(t_cmd *cmd)
{
	ft_putstr_fd("sadaf: syntax error: missing )\n", 2);
	free_cmd(cmd);
	return (NULL);
}

t_cmd	*parseblock(char **ps, char *es, t_shell *shell)
{
	t_cmd	*cmd;

	if (!peek(ps, es, "("))
	{
		ft_putstr_fd("sadaf: syntax error\n", 2);
		return (NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseline(ps, es, shell);
	if (!peek(ps, es, ")"))
		return (handle_missing_closing(cmd));
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es, shell);
	return (cmd);
}

static int	handle_parse_error(t_cmd *ret)
{
	ft_putstr_fd("sadaf: syntax error\n", 2);
	free_cmd(ret);
	return (1);
}

static int	handle_too_many_args(t_cmd *ret)
{
	ft_putstr_fd("sadaf: too many arguments\n", 2);
	free_cmd(ret);
	return (1);
}

static int	add_argument(t_execcmd *cmd, char *q, char *eq, int argc)
{
	cmd->argv[argc] = ft_strndup(q, eq - q);
	cmd->eargv[argc] = eq;
	return (argc + 1);
}

static t_cmd	*parseexec_loop(t_cmd *ret, char **ps, char *es, t_shell *shell)
{
	t_execcmd	*cmd;
	int			argc;
	int			tok;
	char		*q;
	char		*eq;

	cmd = (t_execcmd *)ret;
	argc = 0;
	while (!peek(ps, es, "|)&;"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break;
		if (tok != 'a')
			return (NULL);
		argc = add_argument(cmd, q, eq, argc);
		if (argc >= MAXARGS)
			return (NULL);
		ret = parseredirs(ret, ps, es, shell);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
}

t_cmd	*parseexec(char **ps, char *es, t_shell *shell)
{
	t_cmd	*ret;

	if (peek(ps, es, "("))
		return (parseblock(ps, es, shell));
	ret = execcmd();
	ret = parseredirs(ret, ps, es, shell);
	ret = parseexec_loop(ret, ps, es, shell);
	if (!ret)
	{
		if (ps && *ps)
			return ((void *)1);
		return (NULL);
	}
	return (ret);
}

static void	process_exec_cmd(t_execcmd *ecmd)
{
	int	i;

	for (i = 0; ecmd->argv[i]; i++)
		*ecmd->eargv[i] = 0;
}

static void	process_cmd_type(t_cmd *cmd)
{
	if (cmd->type == EXEC)
		process_exec_cmd((t_execcmd *)cmd);
	else if (cmd->type == REDIR)
	{
		nulterminate(((t_redircmd *)cmd)->cmd);
		*((t_redircmd *)cmd)->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
		nulterminate(((t_pipecmd *)cmd)->left);
		nulterminate(((t_pipecmd *)cmd)->right);
	}
}

static void	process_list_back_cmd(t_cmd *cmd)
{
	if (cmd->type == LIST)
	{
		nulterminate(((t_listcmd *)cmd)->left);
		nulterminate(((t_listcmd *)cmd)->right);
	}
	else if (cmd->type == BACK)
		nulterminate(((t_backcmd *)cmd)->cmd);
}

t_cmd	*nulterminate(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	process_cmd_type(cmd);
	process_list_back_cmd(cmd);
	return (cmd);
}
