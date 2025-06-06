/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/06/06 17:14:45 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parseredirs - Parses redirections in command line
 * @cmd: Command structure to fill
 * @ps: Parser state to track position
 *
 * Returns: Command structure with redirections attached, or NULL on error
 *
 */
int	validate_filename_token(t_token file_tok, t_token op_tok)
{
	if (file_tok.type != TOK_WORD)
	{
		// Provide specific error message for heredocs
		if (op_tok.type == TOK_DLT)
		{
			ft_putstr_fd("\x1b[31msadaf: syntax error: expected delimiter after '<<'\n", STDERR_FILENO);
			return (-1);
		}
		else
		{
			ft_putstr_fd("\x1b[31msadaf: syntax error: expected filename after redirection\n", STDERR_FILENO);
			return (-1);
		}
	}
	return (0);
}

t_cmd	*create_and_validate_redirection(t_cmd *cmd, t_token op_tok,
									t_token file_tok, bool *heredoc)
{
	t_cmd	*new_redir;

	new_redir = create_redirection(cmd, op_tok, file_tok, heredoc);
	if (!new_redir)
	{
		ft_putstr_fd("\x1b[31msadaf: error: failed to create redirection command\n", STDERR_FILENO);
		return (NULL);
	}
	return (new_redir);
}

t_cmd	*handle_redirection_override(t_cmd *cmd, t_cmd *new_redir)
{
	t_redircmd	*redir_cmd;

	redir_cmd = (t_redircmd *)new_redir;
	if (should_override_redirection(redir_cmd->fd, cmd))
		return (override_input_redirection(cmd, new_redir));
	else
		return (new_redir);
}

t_cmd	*process_single_redirection(t_cmd *cmd, t_parserState *ps,
								t_token op_tok, bool *heredoc)
{
	t_token	file_tok;
	t_cmd	*new_redir;

	file_tok = gettoken(ps);
	if (validate_filename_token(file_tok, op_tok) < 0)
	{
		free_cmd(cmd);
		return (NULL);
	}
	
	new_redir = create_and_validate_redirection(cmd, op_tok, file_tok, heredoc);
	if (!new_redir)
	{
		free_cmd(cmd);
		return (NULL);
	}
	
	return (handle_redirection_override(cmd, new_redir));
}

t_cmd	*parseredirs(t_cmd *cmd, t_parserState *ps)
{
	t_token	op_tok;
	bool	heredoc;

	heredoc = false;
	while (1)
	{
		op_tok = gettoken(ps);
		if (!is_redirection_token(op_tok.type))
		{
			ps->s = op_tok.start;
			break ;
		}
		cmd = process_single_redirection(cmd, ps, op_tok, &heredoc);
		if (!cmd)
			return (NULL);  // Error occurred, return NULL
	}
	return (cmd);
}
