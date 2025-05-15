/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/15 16:59:18 by mukibrok         ###   ########.fr       */
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

/**
 * fill_redirinfo - Fills redirection information based on operator token
 * @info: Pointer to redirection info structure
 * @op_tok: Operator token (e.g., <, >, >>)
 * @heredoc_flag: Flag indicating if it's a heredoc
 */

static void	fill_redirinfo(
	t_redirinfo *info, t_token op_tok, bool *heredoc_flag)
{
	if (op_tok.type == TOK_LT)
	{
		info->mode = O_RDONLY;
		info->fd = 0;
	}
	else if (op_tok.type == TOK_GT)
	{
		info->mode = O_WRONLY | O_CREAT | O_TRUNC;
		info->fd = 1;
	}
	else if (op_tok.type == TOK_DGT)
	{
		info->mode = O_WRONLY | O_CREAT | O_APPEND;
		info->fd = 1;
	}
	else if (op_tok.type == TOK_DLT)
	{
		info->mode = O_RDONLY;
		info->fd = 0;
		info->heredoc = true;
		*heredoc_flag = true;
	}
}

/**
 * create_redirection - Creates a redirection command
 * @cmd: Original command structure
 * @op_tok: Operator token (e.g., <, >, >>)
 * @file_tok: Filename token
 * @heredoc_flag: Flag indicating if it's a heredoc
 *
 * Returns: New command with redirection or NULL on failure
 *
 * This function creates a new command structure for redirection,
 * filling in the necessary information based on the operator and filename.
 */

t_cmd	*create_redirection(
	t_cmd *cmd, t_token op_tok, t_token file_tok, bool *heredoc_flag)
{
	t_redirinfo	info;

	*heredoc_flag = false;
	ft_memset(&info, 0, sizeof(info));
	fill_redirinfo(&info, op_tok, heredoc_flag);
	info.file = file_tok.start;
	info.efile = file_tok.end;
	return (redircmd(cmd, info));
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

t_cmd	*parseredirs(t_cmd *cmd, t_parserState *ps)
{
	t_token	op_tok;
	t_token	file_tok;
	bool	heredoc;

	heredoc = false;
	while (1)
	{
		op_tok = gettoken(ps);
		if (op_tok.type != TOK_LT && op_tok.type != TOK_GT
			&& op_tok.type != TOK_DGT && op_tok.type != TOK_DLT)
		{
			ps->s = op_tok.start;
			break ;
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
