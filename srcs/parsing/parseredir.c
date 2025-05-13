/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/13 14:30:59 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * create_redirection - Creates a redirection command
 * @cmd: Original command structure
 * @op_tok: Operator token (e.g., <, >, >>)
 * @file_tok: Filename token
 * @heredoc_flag: Flag indicating if it's a heredoc
 */

t_cmd *create_redirection(t_cmd *cmd, t_token op_tok, t_token file_tok, bool *heredoc_flag)
{
	int	mode;
	int	fd;

	mode = 0;
	fd = 0;
	*heredoc_flag = false;
	if (op_tok.type == TOK_LT)
	{
		mode = O_RDONLY;
		fd = 0;
	}
	else if (op_tok.type == TOK_GT)
	{
		mode = O_WRONLY | O_CREAT | O_TRUNC;
		fd = 1;
	}
	else if (op_tok.type == TOK_DGT)
	{
		mode = O_WRONLY | O_CREAT | O_APPEND;
		fd = 1;
	}
	else if (op_tok.type == TOK_DLT)
	{
		mode = O_RDONLY;
		fd = 0;
		*heredoc_flag = true;
	}
	return (redircmd(cmd, file_tok.start, file_tok.end, mode, fd, *heredoc_flag));
}

/**
 * parseredirs - Parses redirections in command line
 * @cmd: Command structure to fill
 * @ps: Parser state to track position
 *
 * Returns: Command structure with redirections attached
 *
 * This function handles the parsing of file redirections in a command line.
 * It processes operators (<, >, >>, <<) and associates them with filenames.
 * It creates new redirection commands and links them to the original command.
 */

t_cmd	*parseredirs(t_cmd *cmd, ParserState *ps)
{
	t_token	op_tok;
	t_token	file_tok;
	bool	heredoc;

	heredoc = false;
	while (1)
	{
		op_tok = gettoken(ps);
		if (op_tok.type != TOK_LT && op_tok.type != TOK_GT &&
			op_tok.type != TOK_DGT && op_tok.type != TOK_DLT)
		{
			ps->s = op_tok.start;
			break;
		}
		file_tok = gettoken(ps);
		if (file_tok.type != TOK_WORD)
			ft_exit("Syntax error: Expected filename after redirection\n");
		cmd = create_redirection(cmd, op_tok, file_tok, &heredoc);
		if (!cmd)
			ft_exit("Error: Failed to create redirection command\n");
	}
	return (cmd);
}
