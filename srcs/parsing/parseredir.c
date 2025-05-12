/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/12 11:42:46 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parseredirs - Handles file redirections for commands
 * @cmd: Original command structure
 * @ps: Tracks position in the command string
 *
 * What it does:
 * - Processes < > >> operators
 * - Links files to standard input/output
 * - Creates new redirection commands when needed
 * - Validates proper filename format after operators
 *
 * Throws errors if:
 * - Missing filename after < > >>
 * - Memory allocation fails
 * - Invalid redirection operator
 *
 * Returns: Command structure with redirections attached
 */

t_cmd *parseredirs(t_cmd *cmd, ParserState *ps)
{
	t_token	op_tok;
	t_token	file_tok;
	t_cmd	*newcmd;
	int		mode;
	int		fd;
	bool	heredoc;

	heredoc = false;
	while (1)
	{
		op_tok = gettoken(ps);
		if (op_tok.type != TOK_LT && op_tok.type != TOK_GT && op_tok.type != TOK_DGT && op_tok.type != TOK_DLT) {
			ps->s = op_tok.start;
			break;
		}
		file_tok = gettoken(ps);
		if (file_tok.type != TOK_WORD)
			ft_exit("Syntax error: Expected filename after redirection\n");
		switch (op_tok.type)
		{
			case TOK_LT:
				mode = O_RDONLY;
				fd = 1;
				break;
			case TOK_GT:
				mode = O_WRONLY | O_CREAT | O_TRUNC;
				fd = 1;
				break;
			case TOK_DGT:
				mode = O_WRONLY | O_CREAT | O_APPEND;
				fd = 1;
				break;
			case TOK_DLT:
				mode = O_RDONLY;
				fd = 0;
				heredoc = true;
				break;
			default:
				break;
		}
		newcmd = redircmd(cmd, file_tok.start, file_tok.end, mode, fd, heredoc);
		if (!newcmd)
			ft_exit("Error: Failed to create redirection command\n");
		cmd = newcmd;
	}
	return (cmd);
}
