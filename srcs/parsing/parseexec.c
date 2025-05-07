/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseexec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:41:05 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 17:42:51 by mukibrok         ###   ########.fr       */
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

t_cmd	*parseexec(ParserState *ps)
{
	t_execcmd	*cmd;
	t_cmd		*ret;
	t_token		tok;
	int			argc;

	tok = gettoken(ps);
	if (tok.type == TOK_LPAREN)
		return (parseblock(ps));
	ps->s = tok.start;
	ret = execcmd();
	if (!ret)
		ft_exit("execcmd: ft_calloc failed");
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps);
	while (1)
	{
		tok = gettoken(ps);
		if (tok.type == TOK_PIPE || tok.type == TOK_AND
			|| tok.type == TOK_SEQ || tok.type == TOK_RPAREN
			|| tok.type == TOK_EOF)
		{
			ps->s = tok.start;
			break ;
		}
		if (tok.type != TOK_WORD)
		{
			free_cmd(ret);
			ft_exit("syntax error: unexpected token\n");
		}
		cmd->argv[argc] = tok.start;
		cmd->eargv[argc] = tok.end;
		argc++;
		if (argc >= MAXARGS)
			ft_exit("too many args");
		ret = parseredirs(ret, ps);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
}