/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseexec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:41:05 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/13 18:07:59 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parseexec - Handles regular commands and their parts
 * @ps: Tracks position in the command string
 * 
 * What it does:
 * - Handles normal commands (like "ls" or "grep")
 * - Can start a parenthesized block if seeing '('
 * - Collects command arguments (like "-l" in "ls -l")
 * - Handles file redirections (< > >>)
 * - Stops at special characters (| & ; ) EOF)
 * 
 * Throws errors if:
 * - Unexpected symbols appear in command arguments
 * - Too many arguments (more than MAXARGS)
 * - Memory allocation fails
 * - Redirection errors occur
 * 
 * Returns: Command structure ready for execution
 */

static bool	is_stop_token(t_token tok)
{
	return (tok.type == TOK_PIPE || tok.type == TOK_AND
		|| tok.type == TOK_SEQ || tok.type == TOK_RPAREN
		|| tok.type == TOK_EOF);
}

/**
 * init_exec_cmd - Initializes the command structure for execution
 * @ps: Parser state to track position
 * @cmd: Pointer to the command structure to fill
 *
 * Returns: Pointer to the initialized command structure
 *
 * This function initializes the command structure for execution,
 * setting up redirection and handling errors.
 */

static t_cmd	*init_exec_cmd(ParserState *ps, t_execcmd **cmd)
{
	t_cmd	*ret;

	ret = execcmd();
	if (!ret)
		ft_exit("execcmd: ft_calloc failed");
	*cmd = (t_execcmd *)ret;
	ret = parseredirs(ret, ps);
	return (ret);
}

/**
 * handle_token_and_redir - Handles token parsing and redirection
 * @tok: Token to process
 * @cmd: Command structure to fill
 * @argc: Argument count
 * @ret: Pointer to command structure
 * @ps: Parser state to track position
 *
 * Returns: 1 on success, 0 on failure
 *
 * This function processes tokens and handles redirection.
 * It checks for unexpected tokens and manages argument count.
 */

static int	handle_token_and_redir(t_token tok, t_parsectx *ctx)
{
	if (tok.type != TOK_WORD)
	{
		free_cmd(*ctx->ret);
		ft_exit("syntax error: unexpected token\n");
		return (0);
	}
	if (*ctx->argc >= MAXARGS)
	{
		ft_exit("too many args");
		return (0);
	}
	ctx->cmd->argv[*ctx->argc] = tok.start;
	ctx->cmd->eargv[*ctx->argc] = tok.end;
	(*ctx->argc)++;
	if (tok.type != TOK_LT)
	{
		*ctx->ret = parseredirs(*ctx->ret, ctx->ps);
		if (!*ctx->ret)
		{
			ft_exit("error parsing redirection\n");
			return (0);
		}
	}
	return (1);
}

/**
 * parse_arguments - Parses command arguments and redirections
 * @cmd: Command structure to fill
 * @ps: Parser state to track position
 * @ret: Pointer to command structure
 *
 * Returns: Number of arguments parsed
 *
 * This function handles the parsing of command arguments,
 * including redirections and special tokens.
 */

static int	parse_arguments(t_execcmd *cmd, ParserState *ps, t_cmd **ret)
{
	t_token		tok;
	int			argc;
	t_parsectx	ctx;

	ctx.cmd = cmd;
	ctx.argc = &argc;
	ctx.ret = ret;
	ctx.ps = ps;
	argc = 0;
	while (1)
	{
		tok = gettoken(ps);
		if (is_stop_token(tok))
		{
			ps->s = tok.start;
			break ;
		}
		if (!handle_token_and_redir(tok, &ctx))
			return (-1);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (argc);
}

/**
 * parseexec - Parses and constructs command execution structure
 * @ps: Pointer to parser state
 *
 * Returns: Pointer to command structure ready for execution
 *
 * This function handles the parsing of command execution,
 * including arguments and redirections.
 */

t_cmd	*parseexec(ParserState *ps)
{
	t_token		tok;
	t_cmd		*ret;
	t_execcmd	*cmd;

	tok = gettoken(ps);
	if (tok.type == TOK_LPAREN)
		return (parseblock(ps));
	ps->s = tok.start;
	ret = init_exec_cmd(ps, &cmd);
	parse_arguments(cmd, ps, &ret);
	return (ret);
}
